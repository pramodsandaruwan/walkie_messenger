/*
  Arduino Wireless Network - Multiple NRF24L01+ - Walkie messenger
  Electronic product design - pramod sandaruwan
  Libraries:
  nRF24/RF24, https://github.com/nRF24/RF24
  nRF24/RF24Network, https://github.com/nRF24/RF24Network
*/


#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>


RF24 radio(7, 8);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 01;   // Address of our node in Octal format ( 04,031, etc)

void setup() {
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  network.begin(90, this_node); //(channel, node address)
}

void loop() {
  network.update();
  while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header;
    char text[64] = "";
    network.read(header, &text, sizeof(text)); // Read the incoming data
    Serial.println(text);
    delay(200);
  }
}
