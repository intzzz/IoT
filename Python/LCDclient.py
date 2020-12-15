import paho.mqtt.client as mqtt
import I2C_LCD_driver


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
        print("Connected with result code "+str(rc))

        # Subscribing in on_connect() means that if we lose the connection and
        # reconnect then subscriptions will be renewed.
        client.subscribe("embedded/#")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
        print(msg.topic+" "+str(msg.payload))
        if(msg.topic == "embedded/hall"):
            print("Hall sensor data received")
            mylcd.lcd_display_string(msg.payload, 1,11)

        if(msg.topic == "embedded/temp"):
            print("Temp sensor data received")
            mylcd.lcd_display_string(msg.payload, 2,11)

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("85.119.83.194", 1883, 60)

mylcd = I2C_LCD_driver.lcd()

mylcd.lcd_display_string("Hall sens:", 1)
mylcd.lcd_display_string("Temp sens:", 2)
# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()
