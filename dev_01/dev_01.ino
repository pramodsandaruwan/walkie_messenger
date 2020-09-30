
/************************************************************************************************
 * Devoloped by pramod sandaruwan for third year electric product design and manufacture course.
 * Contact: pramodsandaruwan80@gmail.com
 * Elelctrinic and Electronic Engineer
 * Undergraduate, University of peradeniya
************************************************************************************************/

/* Authors: Pramod Sandaruwan */

#include <SoftwareSerial.h>
#include <string.h>
#include <EEPROM.h>

#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

RF24 radio(7, 8);                // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 00;   // Address of this node in Octal format ( 04,031, etc)
const uint16_t node01 = 01; 

SoftwareSerial BlueTooth(2, 3); // RX, TX

String a;
String CH_config;

void setup() {
  
  //Serial.begin(38400);
  BlueTooth.begin(9600);
  pinMode(13,OUTPUT);
  BlueTooth.println("bluetooth test ");
  
  SPI.begin();
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  network.begin(EEPROMReadInt(128), this_node);  //(channel, node address)
  
}

void loop() {
  
  network.update();
  char text[] = "Walkie messenger data transfer";
  
  /* Bluetooth to RF */
  
  if (BlueTooth.available() > 0) {
      // read the incoming byte:
      a = BlueTooth.readString();
      
      if (a.substring(0,5) == "setCH"){
        CH_config = a.substring(6,8);
        ch_configure(CH_config.toInt());
        }
      if (a == "getCH"){
        BlueTooth.print("RF_Channel = ");
        BlueTooth.println(EEPROMReadInt(128));
        return;
       }
  
      a.toCharArray(text, a.length()+1);
      
        RF24NetworkHeader header(node01);     // (Address where the data is going)
        bool ok = network.write(header, &text, sizeof(text)); // Send the data
        //Serial.println(ok);
      //Serial.println(text); //add rf sending fcn
      a = "";
    }
    
  /* RF to bluetooth */
  
    while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header;
    char rtext[64] = "";
    network.read(header, &rtext, sizeof(rtext)); // Read the incoming data
    BlueTooth.println(rtext);
    //Serial.println(rtext);
    delay(200);
  }
}
