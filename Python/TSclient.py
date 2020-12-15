import paho.mqtt.client as mqtt
from time import sleep

hallValue = None
tempValue = None

#Thingspeak topics
TS_Topic = "channels/774222/publish/5FB6LNOHESRBEIDG"

#Eclipse broker topics
hall_topic= "embedded/hall"
temp_topic= "embedded/temp"

# The callback for when the client receives a CONNACK response from the server.
def on_connect_E(client, userdata, flags, rc):
        print("Connected to iot.Eclipse.org with result code "+str(rc))

        # Subscribing in on_connect() means that if we lose the connection and
        # reconnect then subscriptions will be renewed.
        client.subscribe("embedded/#")

# The callback for when the client receives a CONNACK response from the server.
def on_connect_TS(client, userdata, flags, rc):
        print("Connected to Thingspeak with result code "+str(rc))

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
        print(msg.topic+" "+str(msg.payload))
        if(msg.topic == hall_topic):
            global hallValue
            print("Hall sensor data received")
            hallValue = msg.payload

        if(msg.topic == temp_topic):
            global tempValue
            print("Temp sensor data received")
            tempValue = msg.payload

E_client = mqtt.Client()
E_client.on_connect = on_connect_E
E_client.on_message = on_message

E_client.connect("85.119.83.194", 1883, 60)

TS_client = mqtt.Client()
TS_client.on_connect = on_connect_TS
TS_client.connect("mqtt.thingspeak.com",1883,60)

# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
E_client.loop_start()
TS_client.loop_start()
while (True):
    sleep(20)
    print "Sleep over"
    payload = ''
    if(hallValue):
        payload = payload + 'field1=' + str(hallValue) + '&'
    if(tempValue):
        payload = payload + 'field2=' + str(tempValue) + '&'
    if(payload):
        payload = payload + 'status=MQTTPUBLISH'
        TS_client.publish(TS_Topic, payload)
        print "Publishing"
    hallValue = None
    tempValue = None
