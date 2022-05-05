#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            inputDeviceI2S; //xy=188,142
AudioRecordQueue         inputBuffer;    //xy=377,138
AudioPlayQueue           outputBuffer;   //xy=525,134
AudioOutputI2S           outputDeviceI2S; //xy=732,141
AudioConnection          patchCord1(inputDeviceI2S, 0, inputBuffer, 0);
AudioConnection          patchCord2(outputBuffer, 0, outputDeviceI2S, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=438,270
// GUItool: end automatically generated code

const int myInput = AUDIO_INPUT_LINEIN;

const int numberOfTests = 100;
const int testInterval = 1000;

bool outputVoltage = LOW;
int outputVoltagePin = 33;
IntervalTimer toggleVoltageTimer;

void toggleOutputVoltage(){
  setOutputVoltage(!outputVoltage);
}

void setOutputVoltage(bool newOutputVoltage){
  outputVoltage = newOutputVoltage;
  digitalWriteFast(outputVoltagePin, outputVoltage);
}

void setup() {
  // put your setup code here, to run once:
  AudioMemory(128);
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(myInput);
  sgtl5000_1.volume(1.0);
  pinMode(outputVoltagePin, OUTPUT);
  digitalWriteFast(outputVoltagePin, outputVoltage);
}

void testVoltage(){
  uint16_t spikeSampled = false;
  uint16_t latencyDuration = 0;
  digitalWriteFast(outputVoltagePin, HIGH);
  elapsedMicros latencyMicros;
  inputBuffer.begin();  
  while(!spikeSampled){
    while (inputBuffer.available() >= 1){
      latencyDuration = latencyMicros;
      int16_t leftChannelSamples[AUDIO_BLOCK_SAMPLES];
      memcpy(leftChannelSamples, inputBuffer.readBuffer(), AUDIO_BLOCK_SAMPLES*2);
      inputBuffer.freeBuffer();
      for (int i=0; i<AUDIO_BLOCK_SAMPLES; i++){
        if (leftChannelSamples[i] < -10000){
          spikeSampled = true;
          break;
        }
      }
    }
  }
  Serial.print(String(latencyDuration) + ",");
  digitalWriteFast(outputVoltagePin, LOW);
  inputBuffer.end();
  inputBuffer.clear();
}

void loop() {
  for (int i=0; i<numberOfTests; i++) {
      delay(testInterval);
      testVoltage();
  }
  Serial.print("\nDone!");
  while(1);
}
