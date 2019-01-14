
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
RF24 radio(9,10);
RF24Network network(radio);
const uint16_t this_node = 00;
const uint16_t other_node = 01;

unsigned long Speed2;
void setup(void)
{
  Serial.begin(57600);
  Serial.println("RF24Network/examples/helloworld_rx/");

  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
}
void loop(void){

  network.update();

  while ( network.available() ) {

    RF24NetworkHeader header;

    network.read(header,&Speed2,sizeof(Speed2));
    Serial.print("Received packet ");
    Serial.print(Speed2);
    Serial.print(" KM/H ");

  }
}
