#include <ADC.h>
#include <ADC_util.h>

#define OUTPUT_PIN 33
#define LOOPBACK_PIN A0
#define CIRCUIT_PIN 26

ADC *adc = new ADC();; // adc object

int firstCircutPinValue;
int loopbackDelay;

void runTest() {
  int loopbackReadCount = 0;
  int circuitReadCount = 0;
  int loopbackAnalogVoltage;
  int circuitAnalogVoltage;
  // disable interrupts while we do the tests

  digitalWriteFast(OUTPUT_PIN, LOW);
  delay(100);
  noInterrupts();
  // write the pin high
  digitalWriteFast(OUTPUT_PIN, HIGH);
  // set the counter to zero
  ARM_DWT_CYCCNT = 0;
  while((loopbackAnalogVoltage = adc->adc0->analogRead(LOOPBACK_PIN)) < 10){
    loopbackReadCount++;
  }
  loopbackDelay = ARM_DWT_CYCCNT;
  
  while((circuitAnalogVoltage = adc->adc1->analogRead(CIRCUIT_PIN)) < 10){
    circuitReadCount++;
  }
  int circuitDelay = ARM_DWT_CYCCNT;
  
  interrupts();
  Serial.print("LAV: ");
  Serial.print(loopbackAnalogVoltage);
  Serial.print(" - CAV: ");
  Serial.print(circuitAnalogVoltage);
  Serial.print(" - LoopbackReadLoops: ");
  Serial.print(loopbackReadCount);
  Serial.print(" - CircuitReadLoops: ");
  Serial.print(circuitReadCount);
  Serial.print(" - Loopback delay: ");
  Serial.print(loopbackDelay);
  Serial.print(" - Circuit Delay: ");
  Serial.print(circuitDelay);
  int delta = circuitDelay-loopbackDelay;
  Serial.print(" - Delta: ");
  Serial.print(delta);
  float latency = delta * 1.67;
  Serial.print(" - LATENCY: ");
  Serial.print(latency);
  Serial.println(" ns");
}

void setup() {
    ///// ADC0 ////
  adc->adc0->setAveraging(1); // set number of averages
  adc->adc0->setResolution(8); // set bits of resolution
  adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_HIGH_SPEED); // change the conversion speed
  adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED); // change the sampling speed

  adc->adc1->setAveraging(1); // set number of averages
  adc->adc1->setResolution(8); // set bits of resolution
  adc->adc1->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_HIGH_SPEED); // change the conversion speed
  adc->adc1->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED); // change the sampling speed
  
  Serial.begin(115200);

  pinMode(OUTPUT_PIN, OUTPUT);
  pinMode(LOOPBACK_PIN, INPUT);
  pinMode(CIRCUIT_PIN, INPUT);
}

void loop() {
  runTest();
  delay(1000);
}
