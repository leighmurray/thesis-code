#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            inputDeviceI2S; //xy=253,223
AudioMixer4              mixer1;         //xy=430,236
AudioRecordQueue         inputBufferLeft;    //xy=592,213
AudioPlayQueue           outputBufferLeft;    //xy=592,213
AudioOutputI2S           outputDeviceI2S; //xy=947,216
AudioConnection          patchCord1(inputDeviceI2S, 0, mixer1, 0);
AudioConnection          patchCord2(outputBufferLeft, 0, outputDeviceI2S, 0);
AudioConnection          patchCord3(mixer1, inputBufferLeft);
AudioControlSGTL5000     sgtl5000_1;     //xy=653,345
// GUItool: end automatically generated code


const int myInput = AUDIO_INPUT_LINEIN;

void setup() {
  // put your setup code here, to run once:
  AudioMemory(8);
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(myInput);
  sgtl5000_1.volume(1.0);
  mixer1.gain(0, 2);
  inputBufferLeft.begin();
}

void printSamples(){
  while (inputBufferLeft.available() >= 1){
    int16_t leftChannelSamples[AUDIO_BLOCK_SAMPLES];
    memcpy(leftChannelSamples, inputBufferLeft.readBuffer(), AUDIO_BLOCK_SAMPLES*2);
    memcpy(outputBufferLeft.getBuffer(), leftChannelSamples, AUDIO_BLOCK_SAMPLES*2);
    outputBufferLeft.playBuffer();
    inputBufferLeft.freeBuffer();
  }
}

void loop() {
  printSamples();
}
