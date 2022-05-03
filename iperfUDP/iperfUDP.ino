#include <NativeEthernet.h>
#include <NativeEthernetUdp.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
uint8_t mac[6];

unsigned int localPort = 5001;      // local port to listen on

// buffers for receiving and sending data
//char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  // buffer to hold incoming packet,
char packetBuffer[64];
// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void SetupSerial(){
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void SetMacAddress() {
  for(uint8_t by=0; by<2; by++) mac[by]=(HW_OCOTP_MAC1 >> ((1-by)*8)) & 0xFF;
  for(uint8_t by=0; by<4; by++) mac[by+2]=(HW_OCOTP_MAC0 >> ((3-by)*8)) & 0xFF;
  Serial.printf("MAC: %02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void SetupEthernetAndUDP(){
  Ethernet.setStackHeap(1024 * 128); //Set stack size to 128k
  Ethernet.setSocketSize(1024 * 4); //Set buffer size to 4k
  Ethernet.setSocketNum(6); //Change number of allowed sockets to 6
  Ethernet.begin(mac);
  
  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start UDP
  Udp.begin(localPort);
  
  Serial.print("IP Address: ");
  Serial.println(Ethernet.localIP());
}

void setup() {
  // Open serial communications and wait for port to open.
  // Do this first so other functions can use it.
  SetupSerial();

  // Setup teensy specific mac address
  SetMacAddress();

  // Setup Networking
  SetupEthernetAndUDP();
}

void ReadUDP() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  ReadUDP();
}
