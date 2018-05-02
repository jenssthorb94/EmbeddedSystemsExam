// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_Sensor.h>
// This #include statement was automatically added by the Particle IDE.
#include <DHT.h>
// This #include statement was automatically added by the Particle IDE.
#include <blynk.h>

#define waterPin A0
#define redLight D0
#define greenLight D1
#define yellowLight D2
#define DHTPIN D5
#define DHTTYPE DHT11

// Use your own authentication token, because it is not recommended to share your personal one. 
char blynkAuth[] = "";

int maxWater = 800;
int midWater = 250;
int lowWater = 50;


WidgetLED redLedBlynk(V0);
WidgetLED yellowLedBlynk(V1);
WidgetLED greenLedBlynk(V2);

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Blynk.begin(blynkAuth);

  dht.begin();
  pinMode(greenLight, OUTPUT);
  pinMode(redLight, OUTPUT);
  pinMode(yellowLight, OUTPUT);



}
void loop() {
  Blynk.run();
  water_readings();
  dht_readings();
}

void water_readings() {
    int waterlevel = analogRead(waterPin);
    setLightsLow();

  if (waterlevel >= maxWater) {
    Particle.publish("water-level-changed", "high");
    digitalWrite(redLight, HIGH);
    Blynk.virtualWrite(V5, "Water level is high!");
    blynkOff();
    redLedBlynk.on();
    redLedBlynk.setValue(255);
  } 
  else if (waterlevel >= midWater) {
    Particle.publish("water-level-changed", "mid-water");
    digitalWrite(yellowLight, HIGH);
    Blynk.virtualWrite(V5, "Water level is starting to get high!");
    blynkOff();
    yellowLedBlynk.on();
    yellowLedBlynk.setValue(255);
  } 
  else if (waterlevel <= lowWater) {
    Particle.publish("water-level-changed", "low");
    digitalWrite(greenLight, HIGH);
    Blynk.virtualWrite(V5, "Water level is low, but watch out!");
    blynkOff();
    greenLedBlynk.on();
    greenLedBlynk.setValue(255);

  }

}

void dht_readings() {
  delay(5000); // So it waits between readings.
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Particle.publish("temperature", String(temperature) + "°C");
  Blynk.virtualWrite(V3, String(temperature));
  Particle.publish("humidity", String(humidity) + "%");
  Blynk.virtualWrite(V4, String(humidity));
}
void blynkOff() {
  redLedBlynk.setValue(0);
  yellowLedBlynk.setValue(0);
  greenLedBlynk.setValue(0);
}

void setLightsLow(){
  digitalWrite(greenLight, LOW);
  digitalWrite(redLight, LOW);
  digitalWrite(yellowLight, LOW);
}





