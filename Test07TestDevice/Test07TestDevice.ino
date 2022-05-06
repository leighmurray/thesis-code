#include <QNEthernet.h>
#include <QNDNSClient.h>

using namespace qindesign::network;

constexpr uint32_t kDHCPTimeout = 10000;  // 10 seconds
constexpr uint16_t dataPort = 4464;

EthernetUDP udp;

uint8_t buf[Ethernet.mtu() - 20 - 8];  // Maximum UDP payload size
                                       // 20-byte IP, 8-byte UDP header

uint8_t mac[6];
uint8_t macAddressUSBHeader[6] = {0x04, 0xe9, 0xe5, 0x0c, 0xec, 0x21};
uint8_t macAddressNoUSBHeader[6] = {0x04, 0xe9, 0xe5, 0x11, 0x22, 0x7c};

uint8_t* serverMacAddress = macAddressNoUSBHeader;
uint8_t* clientMacAddress = macAddressUSBHeader;
const int testPacketSize = Ethernet.mtu() - 20 - 8;
uint8_t dataPacket[testPacketSize];

IPAddress remoteIP;

const int measureIntervalMicros = 1000000;
volatile uint32_t bytesReceived = 0;
IntervalTimer measurementTimer;
volatile uint32_t measuredBytesReceived = 0;

bool isServer(){
  for (int i=0; i<6; i++){
    if (serverMacAddress[i] != mac[i]){
      return false;
    }
  }
  return true;
}

void setupNetwork () {
  Serial.println("Setting up network");
  Ethernet.macAddress(mac);

  Serial.printf("MAC = %02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.println("Setting up static IP");
  IPAddress subnetMask{255, 255, 255, 0};
  IPAddress serverIP{192, 168, 1, 1};
  IPAddress clientIP{192, 168, 1, 2};
  
  if (isServer()){
    Serial.println("We're the server");
    Ethernet.begin(serverIP, subnetMask, clientIP);
    remoteIP = clientIP;
  } else {
    Serial.println("We're the client");
    Ethernet.begin(clientIP, subnetMask, serverIP);
    remoteIP = serverIP;
  }
  
  Serial.print("Establishing Link: ");
  bool linkEstablished = Ethernet.waitForLink(10000);
  Serial.println(linkEstablished);
  
  // Listen on port
  udp.begin(dataPort);
}

void measure() {
  noInterrupts();
  measuredBytesReceived = bytesReceived;
  bytesReceived = 0;
  interrupts();
}

void setup() {
  Serial.begin(115200);
  memset(dataPacket, 0, testPacketSize);
  // put your setup code here, to run once:
  setupNetwork();
  measurementTimer.begin(measure, measureIntervalMicros);
}

void loop() {
  uint16_t size = udp.parsePacket();
  if (0 < size && size <= sizeof(buf)) {
    udp.read(buf, size);
    bytesReceived += size;
  }
  udp.send(remoteIP, dataPort, dataPacket, testPacketSize);
  if (measuredBytesReceived){
    Serial.print(String(measuredBytesReceived) + ",");
    measuredBytesReceived = 0;
  }
}
