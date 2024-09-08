/*--------------------------------------------------------------------------------------
//SURABAYA MOJOKERTO SIDOARJO PORONG
 dmd_latin_chars
   Demo and example Latin-1 encoding project for the Freetronics DMD, a 512 LED matrix display
   panel arranged in a 32 x 16 layout.

See http://www.freetronics.com/dmd for resources and a getting started guide.

This example code is in the public domain. It demonstrates using the ISO-8859-1
(Latin-1) extended character set. Thanks to Xavier Seignard for contributing
Latin-1 support.

*******************************
HOW TO ENTER Latin-1 CHARACTERS
*******************************

Unfortunately entering Latin-1 characters like Ã or è is not as simple as just
typing them. Arduino Sketches are saved in Unicode UTF-8 format, but the DMD library
does not understand Unicode (it's too complex.)

To enter the characters as Latin-1, look at the codepage layout here and look for
the hexadecimal digit that corresponds to the character you want.
https://en.wikipedia.org/wiki/ISO/IEC_8859-1#Codepage_layout

For example, á (lowercase a, rising diacritical mark) has hex value 00E0 in Latin-1.
To translate this into a constant string, replace the leading 00 with \x - so the
string could be "The Portugese for rapid is r\xE0pido".

To be safe, the string may also need to be separated in its own quote marks - ie
"The Portugese for rapid is r""\xE0""pido"

When you compile the sketch, the compiler will join all these strings up into one
long string - however without the quotes around the \x it may try to include additional
characters as part of the hexadecimal sequence.

/*--------------------------------------------------------------------------------------
  Includes
--------------------------------------------------------------------------------------*/

//UNTUK INPUT KEYPAD/////////////////////////////////////////////////////
#include <Keypad_I2C.h>
#include <Keypad.h>
#include <Wire.h>
#include <EEPROM.h>
const byte ROWS = 4; // empat rows
const byte COLS = 4; // empat columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {0, 1, 2, 3}; 
byte colPins[COLS] = {4, 5, 6, 7};
int i2caddress = 0x20; // alamat PCF8574, semua pin A0 - A2 ke Ground.
Keypad_I2C kpd = Keypad_I2C( makeKeymap(keys), colPins, rowPins, ROWS, COLS, i2caddress );
//UNTUK INPUT KEYPAD//////////////////////////////////////////////////////


#include <SPI.h>        //SPI.h must be included as DMD is written by SPI (the IDE complains otherwise)
#include <DMD.h>        //
#include <TimerOne.h>   //
#include "SystemFont5x7.h"
#include "Arial_Black_16_ISO_8859_1.h"
#include <fonts/Arial14.h>
#include <fonts/Arial_Black_16.h>
#include <fonts/Droid_Sans_24.h>



//Fire up the DMD library as dmd
#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);
const char *MSG;
const uint8_t *FONT = Arial_Black_16;
/*--------------------------------------------------------------------------------------
  Interrupt handler for Timer1 (TimerOne) driven DMD refresh scanning, this gets
  called at the period set in Timer1.initialize();
--------------------------------------------------------------------------------------*/

int addr = 0;
byte value;
byte value1;
void ScanDMD()
{ 
  dmd.scanDisplayBySPI();
  ////////////////////
}

/*--------------------------------------------------------------------------------------
  setup
  Called by the Arduino architecture before the main loop begins
--------------------------------------------------------------------------------------*/
String oput,oput1;
void setup(void)
{

   //initialize TimerOne's interrupt/CPU usage used to scan and refresh the display
   Timer1.initialize(1000);           //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
   Timer1.attachInterrupt( ScanDMD );   //attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()

   //clear/init the DMD pixels held in RAM
   dmd.clearScreen( true );   //true is normal (all pixels off), false is negative (all pixels on)
  Serial.begin(115200);
  kpd.begin();
  //value = EEPROM.read(addr);

      
value = EEPROM.read(40);
if(value<10){
  oput = "0"+(String)value;
  }
  else{
    oput = (String)value;
    }

//oput.toCharArray(MSG, 3);
delay(200);

value1 = EEPROM.read(41);
if(value1<10){
  oput1 = "0"+(String)value1;
  }
  else{
    oput1 = (String)value1;
    }

//oput1.toCharArray(MSG, 3);
delay(200);

String gabung = oput+oput1;
gabung.toCharArray(MSG,4); 
  MSG = gabung.c_str();
 //MSG = "8865";      
     
    runtext();
}

/*--------------------------------------------------------------------------------------
  loop
  Arduino architecture main loop
--------------------------------------------------------------------------------------*/
char key;
void loop(void)
{
  
   

    
  
}

void runtext(){
  
   dmd.clearScreen( true );
  dmd.selectFont(FONT);
//   // Français, Österreich, Magyarország

 dmd.drawMarquee(MSG,strlen(MSG),(32*DISPLAYS_ACROSS)-1,+1);
long start=millis();
long timer=start;

while(1){
 if ((timer+75) < millis()) {
 dmd.stepMarquee(-1,0);
  timer=millis();
      Serial.println(MSG);
      inputbtn();

     
 
        }  
     
   }
}





void inputbtn(){
  static char userinput[11];  
  static int count = 0;
  
  char key = kpd.getKey();
  if (key != NO_KEY)
  {    
    Serial.print(key);
  }


// key = kpd.getKey();//////////////////////////////
 if (key){
    if(key=='*'){
      while(1){        
          switch (key)
  {
    case NO_KEY:
      // nothing to do
      break;
    case '*':      
      Serial.print(F("Press a number:"));     
      memset(userinput, 0, sizeof(userinput));
      count = 0;
      break;
    case 'D':      
      Serial.print(F("Number:"));
      Serial.println(userinput);
      break;
    default:
      // if not 10 characters yet
      if (count < 10)
      {
        // add key to userinput array and increment counter
        userinput[count++] = key;
      }
      break;
  }

        
        key = kpd.getKey();//////////////////////////////if (key){
          if(key=='#'){
            
            break;
                       
            }
            
          }
        }  
        
        MSG=userinput; 
        int inr = atoi(userinput);
        int belakang;
          int depan;
        if(inr>99&&inr<10000){
          belakang = inr%100;
          depan = inr/100;
          }
        
       EEPROM.write(40, depan);
       delay(200);   
        EEPROM.write(41, belakang);
        delay(200);   
        //    
        dmd.drawMarquee(MSG,strlen(MSG),(32*DISPLAYS_ACROSS)-1,+1);     
      }
  }
  


  
