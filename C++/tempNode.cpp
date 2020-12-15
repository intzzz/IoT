#include <Arduino.h>
#include <ittiot.h>

#include <DHT.h>
#define DHTPIN D3     // what pin we're connected to. Change this to D3 if your shield has one leg removed.
#define DHTTYPE DHT22   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);

void iot_connected()
{
  Serial.println("MQTT connected callback");
  iot.log("IoT DHT example!");
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Booting");
  iot.printConfig(); // print json config to serial //Peale Serial.begin ja enne iot.setup
  iot.setup();
  pinMode(16, OUTPUT);
  dht.begin();

}

void loop()
{
  iot.handle();

	float t = dht.readTemperature();
  if(std::isnormal(t)){
      char buf[10]; // Put whatever length you need here
	    String(t).toCharArray(buf,10);
      digitalWrite(16,HIGH);
      iot.publishMsgTo("embedded/temp",buf, false);
  }
	delay(2000);
}
