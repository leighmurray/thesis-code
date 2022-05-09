#include <ADC.h>
#include <ADC_util.h>

const int outputPin = 33;
const int readPin = 24; // pin 24
int counter = 0;

ADC *adc = new ADC();; // adc object

IntervalTimer returnLowTimer;

void setup() {
  pinMode(outputPin, OUTPUT);
  pinMode(readPin, INPUT_PULLDOWN);
  digitalWriteFast(outputPin, LOW);
  adc->adc0->setAveraging(1); // set number of averages
  adc->adc0->setResolution(8); // set bits of resolution
  adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_HIGH_SPEED); // change the conversion speed
  adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED); // change the sampling speed
}

void loop() {
  delay(1000);
  uint16_t measuredLatency = 0;
  
  elapsedMicros latency;
  digitalWriteFast(outputPin, HIGH);
  
  while(adc->adc0->analogRead(readPin) < 11);
  measuredLatency = latency;

  digitalWriteFast(outputPin, LOW);
  Serial.print(String(measuredLatency) + ",");
  counter++;
  if ((counter % 100) == 0) {
    Serial.println();
  }
}
