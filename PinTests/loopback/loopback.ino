#define OUTPUT_PIN 33
#define LOOPBACK_PIN 14
#define CIRCUIT_PIN 26
int firstCircutPinValue;
int loopbackDelay;

void runTest() {
  // disable interrupts while we do the tests

  digitalWriteFast(OUTPUT_PIN, LOW);
  delay(100);
  noInterrupts();
  // write the pin high
  digitalWriteFast(OUTPUT_PIN, HIGH);
  // set the counter to zero
  ARM_DWT_CYCCNT = 0;
  while(digitalReadFast(LOOPBACK_PIN) ==0);
  loopbackDelay = ARM_DWT_CYCCNT;
  
  while(digitalReadFast(CIRCUIT_PIN) == 0);
  int circuitDelay = ARM_DWT_CYCCNT;
  
  interrupts();
  
  Serial.print("Loopback delay: ");
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
  // put your setup code here, to run once:
  pinMode(OUTPUT_PIN, OUTPUT);
  pinMode(LOOPBACK_PIN, INPUT);
  pinMode(CIRCUIT_PIN, INPUT);
}

void loop() {
  runTest();
  delay(1000);
}
