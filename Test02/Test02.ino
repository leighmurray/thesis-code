#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            inputDeviceI2S;           //xy=185,238.00000667572021
AudioOutputI2S           outputDeviceI2S; //xy=418.00001525878906,238.00002670288086
AudioConnection          patchCord1(inputDeviceI2S, 0, outputDeviceI2S, 0);
AudioConnection          patchCord2(inputDeviceI2S, 1, outputDeviceI2S, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=301.00000762939453,315.0000114440918
// GUItool: end automatically generated code

const int myInput = AUDIO_INPUT_LINEIN;

void setup() {
  // put your setup code here, to run once:
  AudioMemory(12);
  sgtl5000_1.enable();
  sgtl5000_1.lineInLevel(0); // 3.12 p-p
  sgtl5000_1.lineOutLevel(13); // 3.16 p-p
  sgtl5000_1.inputSelect(myInput);
  sgtl5000_1.volume(1.0);
}

void loop() {
  // put your main code here, to run repeatedly:

}
