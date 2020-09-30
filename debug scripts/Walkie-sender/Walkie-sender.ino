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

RF24 radio(7, 8);                // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 00;   // Address of this node in Octal format ( 04,031, etc)
const uint16_t node01 = 01;      

void setup() {
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  network.begin(90, this_node);  //(channel, node address)
}

void loop() {
  network.update();
  
  const char text[] = "Walkie messenger data transfer test";
  
  RF24NetworkHeader header(node01);     // (Address where the data is going)
  bool ok = network.write(header, &text, sizeof(text)); // Send the data
  Serial.println(ok);
  delay(200);
}
