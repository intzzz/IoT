#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <Wire.h>

// Replace the next variables with your SSID/Password combination
const char* ssid = "";
const char* password = "";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "mqtt.thingspeak.com";
const char* mqtt_server = "85.119.83.194";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

String formattedDate;
String dayStamp;
String timeStamp;

void setup() {
  Serial.begin(115200); 
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(LED_BUILTIN, OUTPUT);

  timeClient.begin();
  timeClient.setTimeOffset(10800);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {

  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  int hallReading;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  hallReading = messageTemp.toInt();
  Serial.println();

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  //if (String(topic) == "channels/695112/subscribe/fields/field2") {
  if (String(topic) == "embedded/hall") {
    if(hallReading <= 0){
      Serial.println("negative");
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else if(hallReading > 0){
      Serial.println("positive");
      digitalWrite(LED_BUILTIN, LOW);
    } 
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      //client.subscribe("channels/695112/subscribe/fields/field2");
      client.subscribe("embedded/hall");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      while (WiFi.status() != WL_CONNECTED) {
        setup_wifi();
      }
      delay(5000);
    }
  }
}
void get_time(){
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  formattedDate = timeClient.getFormattedDate();

  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
  Serial.print("DATE: ");
  Serial.println(dayStamp);
    // Extract time
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  Serial.print("HOUR: ");
  Serial.println(timeStamp);
}


void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  long now = millis();
  if (now - lastMsg > 1000) {
    
    lastMsg = now;

    int val = hallRead();
    get_time();
    
    char hallString[8];
    char string_to_send[25];
    char time_to_send[10];
    dtostrf(val, 1, 2, hallString);
    //timeStamp.toCharArray(time_to_send, 10);

    //strcpy(string_to_send, hallString);
    //strcat(string_to_send, ";");
    //strcat(string_to_send, time_to_send);
    
    //client.publish("channels/695112/publish/fields/field2/V8DD4LPOL4XZGM8V", hallString);
    client.publish("embedded/hall", hallString);
    
  }
}
