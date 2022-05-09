#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            inputDeviceI2S; //xy=253,223
AudioMixer4              mixer1;         //xy=430,236
AudioRecordQueue         inputBufferLeft;    //xy=592,213
AudioOutputI2S           outputDeviceI2S; //xy=947,216
AudioSynthWaveform       waveform1;     
AudioConnection          patchCord1(inputDeviceI2S, 0, mixer1, 0);
AudioConnection          patchCord2(waveform1, 0, outputDeviceI2S, 0);
AudioConnection          patchCord3(mixer1, inputBufferLeft);
AudioControlSGTL5000     sgtl5000_1;     //xy=653,345
// GUItool: end automatically generated code


const int myInput = AUDIO_INPUT_LINEIN;

const int testInterval = 1000;

volatile bool outputVoltage = LOW;
int outputVoltagePin = 33;
IntervalTimer toggleVoltageTimer;
bool spikeFound = false;
elapsedMicros latencyMicros;
uint16_t storedLatency = 0;

void toggleOutputVoltage(){
  setOutputVoltage(!outputVoltage);
}

void setOutputVoltage(bool newOutputVoltage){
  outputVoltage = newOutputVoltage;
  if (outputVoltage == HIGH){
    latencyMicros = 0;
    spikeFound = false;
    AudioNoInterrupts();
    waveform1.amplitude(1);
    waveform1.phase(179);
    AudioInterrupts();
  } else {
    waveform1.amplitude(0);
  }
}

void setup() {
  // put your setup code here, to run once:
  AudioMemory(128);
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(myInput);
  sgtl5000_1.volume(1.0);
  mixer1.gain(0, -2);
  waveform1.pulseWidth(0.5);
  waveform1.frequency(220);
  waveform1.amplitude(0);
  waveform1.begin(WAVEFORM_PULSE);
  pinMode(outputVoltagePin, OUTPUT);
  digitalWriteFast(outputVoltagePin, outputVoltage);
  inputBufferLeft.begin();
  toggleVoltageTimer.begin(toggleOutputVoltage, 1000000);
}

void printSamples(){
  while (inputBufferLeft.available() >= 1){
    storedLatency = latencyMicros;
    int16_t leftChannelSamples[AUDIO_BLOCK_SAMPLES];
    memcpy(leftChannelSamples, inputBufferLeft.readBuffer(), AUDIO_BLOCK_SAMPLES*2);
    inputBufferLeft.freeBuffer();
    
    for (int i=0; i<AUDIO_BLOCK_SAMPLES; i++){
      Serial.println(String(leftChannelSamples[i]));
    }
//    if(!spikeFound){
//      for (int i=0; i<AUDIO_BLOCK_SAMPLES; i++){
//        if (leftChannelSamples[i] > 30000){
//           spikeFound = true;
//           Serial.println(storedLatency);
//           break;
//        }
//      } 
//    }
  }
}

void loop() {
  printSamples();
}
