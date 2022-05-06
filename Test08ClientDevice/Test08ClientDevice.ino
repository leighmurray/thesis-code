#include <QNEthernet.h>
#include <QNDNSClient.h>

using namespace qindesign::network;

constexpr uint32_t kDHCPTimeout = 10000;  // 10 seconds
constexpr uint16_t dataPort = 4464;

EthernetUDP udp;

uint8_t buf[Ethernet.mtu() - 20 - 8];  // Maximum UDP payload size
                                       // 20-byte IP, 8-byte UDP header

uint8_t mac[6];

IPAddress remoteIP;

void setupNetwork () {
  Serial.println("Setting up network");
  Ethernet.macAddress(mac);

  Serial.printf("MAC = %02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.println("Setting up static IP");
  IPAddress subnetMask{255, 255, 255, 0};
  IPAddress serverIP{192, 168, 1, 1};
  IPAddress clientIP{192, 168, 1, 2};
  Ethernet.begin(clientIP, subnetMask, serverIP);
  remoteIP = serverIP;
  
  // Listen on port
  udp.begin(dataPort);
}

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  setupNetwork();
}

void loop() {
  uint16_t size = udp.parsePacket();
  if (0 < size && size <= sizeof(buf)) {
    udp.read(buf, size);
    udp.send(remoteIP, dataPort, buf, size);
  }
}
