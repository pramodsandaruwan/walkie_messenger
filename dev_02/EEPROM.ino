
/* functions */
void(* resetFunc) (void) = 0;

void ch_configure(int channelNo){

      EEPROMWriteInt(128,channelNo);
      BlueTooth.println("Channel set to "+CH_config);
      BlueTooth.println("Restarting device...");
      delay(1000);
      resetFunc(); // call reset function
      while(1);    // never gets here. NOT NEEDED OF COURSE
   }
 
void EEPROMWriteInt(int address, int value) {

      byte two = (value & 0xFF);
      byte one = ((value >> 8) & 0xFF);
      
      EEPROM.update(address, two);
      EEPROM.update(address + 1, one);
  }
   
int EEPROMReadInt(int address) {

      long two = EEPROM.read(address);
      long one = EEPROM.read(address + 1);
   
      return ((two << 0) & 0xFFFFFF) + ((one << 8) & 0xFFFFFFFF);
  }
