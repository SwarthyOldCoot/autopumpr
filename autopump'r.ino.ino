// set vars for pins
const int soilMoistureSensor = A0;
const int pumpControl = 3;
const int redLed = 10;
const int greenLed = 12;
const int airValue = 515;
const int waterValue = 260;

// set initial vars for moisture values
int intervals = (airValue - waterValue) / 3;
int soilMoistureValue = 0;

// set vars for delay
unsigned long previousMillis = 0;
const long millisInterval = 60000;

// setup serial output and inital pin modes
void setup() {
  Serial.begin(9600);
  Serial.println("Starting!");
  pinMode(pumpControl, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(soilMoistureSensor, INPUT);
}

void loop() {
  // read the sensor and stick it in a variable
  int soilMoistureValue = analogRead(soilMoistureSensor);
  // for debugging
  // Serial.println(soilMoistureValue);

  // too wet!  both lights on.
  if (soilMoistureValue > waterValue && soilMoistureValue < (waterValue + intervals)) {
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, HIGH);
    digitalWrite(pumpControl, LOW);
  // just right, green light.
  } else if (soilMoistureValue > (waterValue + intervals) && soilMoistureValue < (airValue - intervals)) {
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
    digitalWrite(pumpControl, LOW);
  // too dry!  red light on, pump runs once per millisInterval.
  } else if (soilMoistureValue < airValue && soilMoistureValue > (airValue - intervals)) {
    // set currentMillis from how long the arduino's been on
    unsigned long currentMillis = millis();

    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);

    // run the pump for 1s if it hasn't been run in 60s
    if (currentMillis - previousMillis >= millisInterval) {
      previousMillis = currentMillis;
      Serial.println("Pump turning on!");
      digitalWrite(pumpControl, HIGH);
      delay(2000);
      Serial.println("Pump turning off!");
      digitalWrite(pumpControl, LOW);
    }
  }
  delay(1000);
}
