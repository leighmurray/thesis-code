#define OUTPUT_PIN 33
#define LOOPBACK_PIN 26
#define CIRCUIT_PIN 27

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
  //noInterrupts();
  // write the pin high
  digitalWriteFast(OUTPUT_PIN, HIGH);
  // set the counter to zero
  ARM_DWT_CYCCNT = 0;
  while((loopbackAnalogVoltage = analogRead(LOOPBACK_PIN)) < 10){
    loopbackReadCount++;
  }
  loopbackDelay = ARM_DWT_CYCCNT;
  
  while((circuitAnalogVoltage = analogRead(CIRCUIT_PIN)) < 10){
    circuitReadCount++;
  }
  int circuitDelay = ARM_DWT_CYCCNT;
  
  //interrupts();
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
  Serial.begin(115200);
  analogReadAveraging(0);
  analogReadRes(8);
  pinMode(OUTPUT_PIN, OUTPUT);
  pinMode(LOOPBACK_PIN, INPUT);
  pinMode(CIRCUIT_PIN, INPUT);
}

void loop() {
  runTest();
  delay(1000);
}
