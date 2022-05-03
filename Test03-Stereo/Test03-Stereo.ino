#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            inputDeviceI2S;           //xy=92,219.0000057220459
AudioRecordQueue         inputBufferLeft;         //xy=243.00000381469727,212.0000057220459
AudioPlayQueue           outputBufferLeft;         //xy=389.00000762939453,208.0000057220459

AudioRecordQueue         inputBufferRight;         //xy=243.00000381469727,212.0000057220459
AudioPlayQueue           outputBufferRight;         //xy=389.00000762939453,208.0000057220459

AudioOutputI2S           outputDeviceI2S; //xy=576.0000991821289,210.0000457763672
AudioConnection          patchCord1(inputDeviceI2S, 0, inputBufferLeft, 0);
AudioConnection          patchCord2(outputBufferLeft, 0, outputDeviceI2S, 0);

AudioConnection          patchCord3(inputDeviceI2S, 1, inputBufferRight, 0);
AudioConnection          patchCord4(outputBufferRight, 0, outputDeviceI2S, 1);

AudioControlSGTL5000     sgtl5000_1;     //xy=301.00000762939453,315.0000114440918
// GUItool: end automatically generated code

const int myInput = AUDIO_INPUT_LINEIN;

void setup() {
  // put your setup code here, to run once:
  AudioMemory(12);
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(myInput);
  sgtl5000_1.volume(1.0);
  inputBufferLeft.begin();
  inputBufferRight.begin();
}

void loop() {
  if (inputBufferLeft.available() >= 1){
    memcpy(outputBufferLeft.getBuffer(), inputBufferLeft.readBuffer(), 256);
    outputBufferLeft.playBuffer();
    inputBufferLeft.freeBuffer();
  }
    if (inputBufferRight.available() >= 1){
    memcpy(outputBufferRight.getBuffer(), inputBufferRight.readBuffer(), 256);
    outputBufferRight.playBuffer();
    inputBufferRight.freeBuffer();
  }
}
