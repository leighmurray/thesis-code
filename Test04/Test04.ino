#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <QNEthernet.h>
#include <QNDNSClient.h>

using namespace qindesign::network;

constexpr uint32_t kDHCPTimeout = 10000;  // 10 seconds
constexpr uint16_t kAudioPort = 4464;

EthernetUDP udp;

uint8_t buf[Ethernet.mtu() - 20 - 8];  // Maximum UDP payload size
                                       // 20-byte IP, 8-byte UDP header

uint8_t macAddressUSBHeader[6] = {0x04, 0xe9, 0xe5, 0x0c, 0xec, 0x21};
uint8_t macAddressNoUSBHeader[6] = {0x04, 0xe9, 0xe5, 0x11, 0x22, 0x7c};

uint8_t* serverMacAddress = macAddressNoUSBHeader;
uint8_t* clientMacAddress = macAddressUSBHeader;
uint8_t mac[6];

// GUItool: begin automatically generated code
AudioInputI2S            inputDeviceI2S;           //xy=92,219.0000057220459
AudioRecordQueue         inputBuffer;         //xy=243.00000381469727,212.0000057220459
AudioPlayQueue           outputBuffer;         //xy=389.00000762939453,208.0000057220459
AudioOutputI2S           outputDeviceI2S; //xy=576.0000991821289,210.0000457763672
AudioConnection          patchCord1(inputDeviceI2S, 0, inputBuffer, 0);
AudioConnection          patchCord2(outputBuffer, 0, outputDeviceI2S, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=301.00000762939453,315.0000114440918
// GUItool: end automatically generated code

const int myInput = AUDIO_INPUT_LINEIN;
IPAddress remoteIP;
bool bIsServer;
uint16_t numberOfSampleBytes = AUDIO_BLOCK_SAMPLES * 2;

bool isServer(){
  for (int i=0; i<6; i++){
    if (serverMacAddress[i] != mac[i]){
      return false;
    }
  }
  return true;
}

void setupNetwork () {
  Serial.begin(115200);
  
  Ethernet.macAddress(mac);
  bIsServer = isServer();
  Serial.printf("MAC = %02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.println("Setting up static IP");
  IPAddress subnetMask{255, 255, 255, 0};
  IPAddress serverIP{192, 168, 1, 1};
  IPAddress clientIP{192, 168, 1, 2};
  
  if (bIsServer){
    Serial.println("We're the server");
    Ethernet.begin(serverIP, subnetMask, clientIP);
    remoteIP = clientIP;
  } else {
    Serial.println("We're the client");
    Ethernet.begin(clientIP, subnetMask, serverIP);
    remoteIP = serverIP;
  }

  // Listen on port
  udp.begin(kAudioPort);
}

void setup() {
  // put your setup code here, to run once:
  setupNetwork();
  AudioMemory(4);
  sgtl5000_1.enable();
  if (bIsServer) {
    sgtl5000_1.inputSelect(myInput);
    inputBuffer.begin();
  }
  sgtl5000_1.volume(1.0);
  
}

void loop() {
  if (bIsServer) {
    if (inputBuffer.available() >= 1){
      uint8_t audioPacket[numberOfSampleBytes];
      memcpy(&audioPacket[0], inputBuffer.readBuffer(), numberOfSampleBytes);
      udp.send(remoteIP,  kAudioPort, audioPacket, numberOfSampleBytes);
      inputBuffer.freeBuffer();
    }
  }
  else {
    uint16_t size = udp.parsePacket();
    if (0 < size && size <= sizeof(buf)) {
      udp.read(buf, size);
      memcpy(outputBuffer.getBuffer(), &buf[0], numberOfSampleBytes);
      outputBuffer.playBuffer();
    }
  }
}
