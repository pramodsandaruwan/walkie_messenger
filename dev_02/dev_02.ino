
/************************************************************************************************
 * Devoloped by pramod sandaruwan for third year electric product design and manufacture course.
 * Contact: pramodsandaruwan80@gmail.com
 * Elelctrinic and Electronic Engineer
 * Undergraduate, University of peradeniya
************************************************************************************************/

/* Authors: Pramod Sandaruwan */

#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>
#include <EEPROM.h>

#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

RF24 radio(7, 8);                // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 01;   // Address of this node in Octal format ( 04,031, etc)
const uint16_t node01 = 00; 

SoftwareSerial BlueTooth(2, 3); // RX, TX
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

bool bt_stat = 0;
const int red = 10;
const int grn = 9;
const int pwr = 6;
const int bt_pin = 4;
String a;
String CH_config;

void setup() {
  
  //Serial.begin(38400);
  lcd.init(); 
  pinMode(red, OUTPUT);
  pinMode(grn, OUTPUT);
  pinMode(pwr, OUTPUT);
  pinMode(bt_pin,INPUT);
  boot();                     
  BlueTooth.begin(38400);
  BlueTooth.println("bluetooth test ");
  
  SPI.begin();
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  network.begin(EEPROMReadInt(128), this_node);  //(channel, node address)

  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hello,welcome to");
  lcd.setCursor(0,1);
  lcd.print("Walkie messenger");
  delay(2000);
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);
  lcd.print("RF.CH>");
  lcd.setCursor(6,0);
  lcd.print(EEPROMReadInt(128));
  lcd.setCursor(8,0);
  lcd.print("|WM-V1.0");
}

void loop() {
  
  bt_connected();
  network.update();
  char text[] = "Walkie messenger data transfer";
  
  /* Bluetooth to RF */
  if(bt_connected()){
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
          lcd_write("Msg sent");
          //Serial.println(ok);
        //Serial.println(text); //add rf sending fcn
        a = "";
      }
  } 
  /* RF to bluetooth */
  
    while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header;
    char rtext[64] = "";
    network.read(header, &rtext, sizeof(rtext)); // Read the incoming data
    BlueTooth.println(rtext);
    lcd_write(rtext);
    //Serial.println(rtext);
    delay(200);
  }
}
void boot(){
  
  digitalWrite(red, HIGH);
  digitalWrite(grn, HIGH);
  digitalWrite(pwr, HIGH);
  delay(700);             
  digitalWrite(red, LOW);
  digitalWrite(grn, LOW);
  digitalWrite(pwr, LOW); 
  delay(300);
  digitalWrite(grn, HIGH);
  }
  
int bt_connected(){
  
  if(!digitalRead(bt_pin)){
    digitalWrite(pwr, HIGH-digitalRead(pwr)); 
    delay(200);
    bt_stat = 0;
    lcd.setCursor(0,1);
    lcd.print("BT-Not Connected");
    return 0;
    }
  digitalWrite(pwr, HIGH);
  if(bt_stat == 0){  
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(2,1);
    lcd.print("BT Connected");
    bt_stat = 1;
    }
  return 1;
  }
  
void lcd_write(char msg[64]){
  //lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print(msg);
  }
