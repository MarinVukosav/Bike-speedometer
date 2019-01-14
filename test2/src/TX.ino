/*
 Copyright (C) 2012 James Coliz, Jr. <maniacbug@ymail.com>
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.

 Update 2014 - TMRh20
 */
/**
 * Simplest possible example of using RF24Network
 *
 * TRANSMITTER NODE
 * Every 2 seconds, send a payload to the receiver node.
 */
#include <Arduino.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
RF24 radio(9,10);                    // nRF24L01(+) radio attached using Getting Started board
RF24Network network(radio);          // Network uses that radio
const uint16_t this_node = 01;        // Address of our node in Octal format
const uint16_t other_node = 00;
const byte LED_pin = 9;

const unsigned long wheel_circumference = 36000UL * 2UL * 31416UL;

unsigned long Speed=0;
unsigned long PrevSpeed = 0;

volatile byte hall_rising = 0; // interrupt flag
volatile unsigned long irqMicros;

unsigned long startMicros;
unsigned long elapsedMicros;

unsigned long displayStartMillis;
const unsigned long displayWaitMillis = 100;
unsigned long last_time_sent;
unsigned long vr=0;
const unsigned long interval = 2000; //ms  // How often to send 'hello world to the other unit
unsigned long last_sent,last_read;             // When did we last send?
unsigned long packets_sent;          // How many have we sent already
                 // Structure of our payload
unsigned long Speed2;
int flag;


unsigned long readSpeed(){
  if ( millis() - displayStartMillis >= displayWaitMillis )
  {
    Speed = wheel_circumference / elapsedMicros;

    displayStartMillis += displayWaitMillis;
    if ( Speed != PrevSpeed )
    {
      Serial.print(Speed*0.0036);
      Serial.print("\n");// this shows mm/sec with no remainder

    }


    PrevSpeed = Speed;
  }
 return PrevSpeed*0.0036;
}

void wheel_IRQ()
{
  irqMicros = micros();
  hall_rising = 1;
  flag=1;

}
void setup(void)
{
  Serial.begin(57600);
  Serial.println("RF24Network/examples/helloworld_tx/");
  pinMode( 2, INPUT ); // pin # is tied to the interrupt
  pinMode( LED_pin, OUTPUT );


  delay( 1000 );

  attachInterrupt( digitalPinToInterrupt(2), wheel_IRQ, RISING );
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
}
void loop() {
  unsigned long now = millis();
  network.update();                          // Check the network regularly
  if ( hall_rising == 1 )
        {
          digitalWrite(LED_pin, HIGH);
          elapsedMicros = irqMicros - startMicros;
          startMicros = irqMicros;
          hall_rising = 2;

        }
  else if ( hall_rising == 2 )
        {
          digitalWrite(LED_pin, LOW);
          hall_rising = 0;

        }

  if(micros() - startMicros > 9000000){
    Speed2=0;

}
  else Speed2=readSpeed();



        // If it's time to send a message, send it!
  if ( now - last_sent >= interval  )
  {
    last_sent = now;
    Serial.print("Sending...");

    RF24NetworkHeader header(/*to node*/ other_node);
    bool ok = network.write(header,&Speed2,sizeof(Speed2));
    if (ok)
      Serial.println("ok.");
    else
      Serial.println("failed.");
  }
}
