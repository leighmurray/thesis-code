#include <QNEthernet.h>

using namespace qindesign::network;

const uint8_t numberOfTests = 100;
const int testInterval = 1000;

constexpr uint32_t kDHCPTimeout = 10000;  // 10 seconds
constexpr uint16_t dataPort = 4464;

EthernetUDP udp;

uint8_t buf[Ethernet.mtu() - 20 - 8];  // Maximum UDP payload size
                                       // 20-byte IP, 8-byte UDP header

uint8_t mac[6];

const int testPacketSize = 8;
uint8_t dataPacket[testPacketSize];

IPAddress remoteIP;

void setupNetwork () {
  Serial.println("Setting up network");
  Ethernet.macAddress(mac);

  Serial.printf("MAC = %02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.println("Setting up static IP");
  IPAddress subnetMask{255, 255, 255, 0};
  IPAddress serverIP{192, 168, 1, 1};
  IPAddress clientIP{192, 168, 1, 2};
  
  Ethernet.begin(serverIP, subnetMask, clientIP);

  Serial.print("Establishing Link: ");
  bool linkEstablished = Ethernet.waitForLink(10000);
  Serial.println(linkEstablished);
  
  remoteIP = clientIP;
  
  // Listen on port
  udp.begin(dataPort);
}

void setup() {
  Serial.begin(115200);
  memset(dataPacket, 0, testPacketSize);
  // put your setup code here, to run once:
  setupNetwork();
}

void loop() {
  Serial.print("Press ENTER to begin testing packet size " + String(testPacketSize) + " bytes: ");
  while(!Serial.available());
  Serial.read();
  Serial.println("Starting!");
  for (int j=0; j<100; j++) {
    Ethernet.loop();
  }
  for (uint8_t i=0; i<numberOfTests; i++) {
    uint16_t storedLatency = 0;
    elapsedMicros ethernetDelay;
    dataPacket[0] = i;
    udp.send(remoteIP, dataPort, dataPacket, testPacketSize);
    while(storedLatency == 0){
      uint16_t size = udp.parsePacket();
      if (0 < size && size <= sizeof(buf)) {
        storedLatency = ethernetDelay;
        udp.read(buf, size);
        if (buf[0] != i) {
          Serial.println("Incorrect packet received");
        }
      }
    }
    Serial.print(String(storedLatency) + ",");
    delay(testInterval);
  }
  Serial.println("\nDone!");
}
