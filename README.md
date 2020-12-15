**EMBEDDED SOFTWARE WORKSHOP PROJECT**
  
AUTHORS:  
RIM PUKS 182558 IASM   
INDREK GUITOR 182555 IASM    

LINK TO VIDEO  
https://drive.google.com/file/d/1WoQsEd66PgRu2aDheJ49QU8MXd5tRifi/view?usp=sharing  
  
SCHEMATIC ILLUSTRATING THE PROJECT IS ADDED TO GIT  

**SUMMARY**   
The main goal of this project was to test different ways to send data using MQTT. This project uses the public broker iot.eclipse.org.
The system logs, displays and uploads Hall sensor and temperature data.
  
**Hall sensor node**  
This is Sparkfun ESP32 Thing with internal Hall sensor. Since the previous lab task (Mario ESP32 task) was written with Eclipse in C, it was decided
that this task would be in different language, Arduino. 
ESP32 first connects to wifi and gets IP address, after which it connects to MQTT broker to publish and subscribe to the hall sensor topic.
Then the ESP32 reads the HALL sensor (reads magnitude of a magnetic field) every second and sends the data to this server. 
It also reads the data published to this server and reacts to it. For example, if the hall sensor value is low (under 0), it turns on the blue LED, if the hall sensor value is high (over 0) it turns the LED off.   
  
**Temperature sensor node**  
This node composes of Wemos D1 Mini with attached AM2302 temperature sensor shield. It reads the sensor value using Adafruit Sensor library and  
publishes the temperature sensor value to "embedded/temp" topic.
It is programmed using ITT IoT software framework.  
  
**Raspberry Pi**  
The Raspberry Pi has 3 Python scripts running concurrently. Each of them is its own MQTT node.
- Display node
Display node subscribes to "embedded/hall" and "embedded/temp" topics and displays their newest values on LCD screen. Communication with the screen uses I2C protocol.
- Logger node
Logger node also subscribes to "embedded/hall" and "embedded/temp" topics. It then writes them to a corresponding file (temp.log and hall.log).
It also adds a timestamp.
- Thingspeak node
This node also subscribes to the same topics as the previous nodes. Every 20 seconds, it uploads the temperature and Hall sensor values to the Thingspeak
database.
