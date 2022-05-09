
const int outputPin = 33;
const int readPin = 24; // pin 24

void setup() {
  pinMode(outputPin, OUTPUT);
  pinMode(readPin, INPUT);
  digitalWriteFast(outputPin, LOW);
  delay(1000);
}

void loop() {
  elapsedMicros latency;
  digitalWriteFast(outputPin, HIGH);

  while(!digitalReadFast(readPin));
  uint16_t measuredLatency = latency;

  Serial.print(String(measuredLatency) + ",");

  //Serial.println("\nDone");
  digitalWriteFast(outputPin, LOW);

  delay(1000);
}
