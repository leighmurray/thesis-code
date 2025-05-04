#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <QNEthernet.h>

using namespace qindesign::network;

constexpr uint32_t kDHCPTimeout = 10000;  // 10 seconds
constexpr uint16_t dataPort = 4464;

EthernetUDP udp;

uint8_t buf[Ethernet.mtu() - 20 - 8];  // Maximum UDP payload size
                                       // 20-byte IP, 8-byte UDP header

uint8_t mac[6];

// GUItool: begin automatically generated code
AudioPlayQueue           outputBuffer;         //xy=389.00000762939453,208.0000057220459
AudioOutputI2S           outputDeviceI2S; //xy=576.0000991821289,210.0000457763672
AudioConnection          patchCord2(outputBuffer, 0, outputDeviceI2S, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=301.00000762939453,315.0000114440918
// GUItool: end automatically generated code

const int myInput = AUDIO_INPUT_LINEIN;
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
  AudioMemory(60);
  sgtl5000_1.enable();
  sgtl5000_1.volume(1.0);
}

void loop() {
  uint16_t size = udp.parsePacket();
  if (0 < size && size <= sizeof(buf)) {
    udp.read(buf, size);
    memcpy(outputBuffer.getBuffer(), &buf[0], size); // we should receive the right buffer size
    outputBuffer.playBuffer();
  }
}
