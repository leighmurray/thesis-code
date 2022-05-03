#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

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

void setup() {
  // put your setup code here, to run once:
  AudioMemory(12);
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(myInput);
  sgtl5000_1.volume(1.0);
  inputBuffer.begin();
}

void loop() {
  if (inputBuffer.available() >= 1){
    memcpy(outputBuffer.getBuffer(), inputBuffer.readBuffer(), 256);
    outputBuffer.playBuffer();
    inputBuffer.freeBuffer();
  }
}
