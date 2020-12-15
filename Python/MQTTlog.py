#!/usr/bin/env python3

# This program is for logging MQTT messages into different log files.
# Log 
#

import paho.mqtt.client as mqtt
import logging

#topics and corresponding files
topic1="embedded/hall"
file1="hall.log"

topic2="embedded/temp"
file2="temp.log"

#sets the format for the log file entries
formatter = logging.Formatter('%(asctime)s %(message)s',datefmt='%d/%m/%Y %H:%M:%S')

#function for setting up new loggers
def setup_logger(name, log_file, level=logging.INFO):
    handler=logging.FileHandler(log_file)
    handler.setFormatter(formatter)
    logger=logging.getLogger(name)
    logger.setLevel(level)
    logger.addHandler(handler)
    return logger

#callback function that will run when the client connects with the MQTT broker
def on_connect(client, userdata, flags, rc):
  print("Connected with result code "+str(rc))
  client.subscribe(topic1)
  client.subscribe(topic2)

#callback function that will run when a message is received
def on_message(client, userdata, msg):
    print(msg.payload.decode())
    print(msg.topic)
    if(msg.topic==topic1):
        logger1.info(msg.payload.decode())
    if(msg.topic==topic2):
        logger2.info(msg.payload.decode())

logger1=setup_logger("logger1", file1)
logger2=setup_logger("logger2", file2)

client = mqtt.Client()
client.connect("85.119.83.194",1883,60) #instead of localHost, an IP address can be used
client.on_connect = on_connect
client.on_message = on_message

client.loop_forever()
