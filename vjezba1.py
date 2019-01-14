
import paho.mqtt.client as mqtt #import the client1
import time
import serial
############
def on_message(client, userdata, message):
    print("message received " ,str(message.payload.decode("utf-8")))
    print("message topic=",message.topic)
    print("message qos=",message.qos)
    print("message retain flag=",message.retain)
########################################

broker_address="broker.mqttdashboard.com"
arduino = serial.Serial('/dev/ttyACM0', 57600, timeout=.1)
while True:
    data = arduino.readline()

    if(data):
        print("creating new instance")
        client = mqtt.Client("P1") #create new instance
        client.on_message=on_message #attach function to callback
        print("connecting to broker")
        client.connect(broker_address) #connect to broker

        client.loop_start() #start the loop
        client.subscribe("projekt")
        client.publish("projekt",data)
        print(data)
        time.sleep(4) # wait
        client.loop_stop() #stop the loop
