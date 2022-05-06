#include <QNEthernet.h>
#include <QNDNSClient.h>

using namespace qindesign::network;

const int numberOfPackets = 100000;

constexpr uint32_t kDHCPTimeout = 10000;  // 10 seconds
constexpr uint16_t dataPort = 4464;

EthernetUDP udp;

uint8_t mac[6];

const int testPacketSize = Ethernet.mtu() - 20 - 8;
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
}

void setup() {
  Serial.begin(115200);
  memset(dataPacket, 0, testPacketSize);
  // put your setup code here, to run once:
  setupNetwork();
}

void loop() {
  Serial.print("Press ENTER to begin: ");
  while(!Serial.available()) {
    Ethernet.loop();
  }
  Serial.read();
  Serial.println("Starting!");
  for (int i=0; i<numberOfPackets; i++) {
    udp.send(remoteIP, dataPort, dataPacket, testPacketSize);
    Ethernet.loop();
  }
  Serial.println("\nDone!");
}
