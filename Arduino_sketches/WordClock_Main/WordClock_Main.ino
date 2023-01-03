
// code includes birthday messages and also automatic daylight saving calculations.

#include <FastLED.h>
#include <DS3232RTC.h>      // https://github.com/JChristensen/DS3232RTC
#include <SoftwareSerial.h>


FASTLED_USING_NAMESPACE

#define DATA_PIN    6
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    121
CRGB leds[NUM_LEDS];

SoftwareSerial BT(8, 9); //  pin D8 to RXD p-in D9 purple to TXD

//Global Variables

const byte pResistor = A0; // Photoresistor at Arduino analog pin A1
const byte buttonPin = 2;

uint8_t animate_speed = 100;
uint8_t Hour_DST;
uint8_t colour_hue = 35;
uint8_t i;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns


const byte numChars = 35;
char receivedData[numChars];   // an array to store the received data
char SetTime[] = "SetTime";
char SetBday[] = "SetBday";
boolean ChangeTime = false;
boolean Changebday = false;
boolean newData = false;
uint8_t bday;
char *strings[10];


// arrays for the LEDs

uint8_t it_s[ ] = {119,118,116,115}; // the arrays of LEDs to light up for its, five past, ten past etc...
uint8_t Five [ ] = {83,84,85,86};
uint8_t Ten [ ] = {91,90,89};
uint8_t Quarter[ ] = {102,103,104,105,106,107,108};
uint8_t Twenty [ ] = {97,96,95,94,93,92};
uint8_t TwentyFive [ ] = {83,84,85,86,92,93,94,95,96,97};
uint8_t Half [ ] = {78,79,80,81}; 

uint8_t Past [ ] = {70,69,68,67};
uint8_t To [ ] = {56,57};

uint8_t Oclock [] = {5,4,3,2,1,0};

uint8_t Happy[] ={55,76,77,98,99};
uint8_t Birthday[] ={43,44,65,66,87,88,109,110};

uint8_t hourarray[][7] = { {59,60,61,62,63,64}, // this outlnes which LEDs to light up, 0-24 arrays, 0 being twleve 1=one 2=two... 24=twelve
                       {36,37,38},
                       {34,35,36},
                       {31,30,29,28,27},
                       {13,14,15,16},
                       {54,53,52,51},
                       {9,8,7},
                       {17,18,19,20,21},
                       {51,50,49,48,47},
                       {39,40,41,42},
                       {47,46,45},
                       {27,26,25,24,23,22},
                       {59,60,61,62,63,64},
                       {36,37,38},
                       {34,35,36},
                       {31,30,29,28,27},
                       {13,14,15,16},
                       {54,53,52,51},
                       {9,8,7},
                       {17,18,19,20,21},
                       {51,50,49,48,47},
                       {39,40,41,42},
                       {47,46,45},
                       {27,26,25,24,23,22},
                       {59,60,61,62,63,64}
                        };
                        
uint8_t Char_array[] = {6,3,3,5,4,4,3,5,5,4,3,6,6,3,3,5,4,4,3,5,5,4,3,6,6}; // this is the size of each of the above arrays for indexing the hourarray later in script                       

// settings to implement on startup

void setup() { 

//Pin Settings
  pinMode(pResistor, INPUT);// Set photo resistor
  pinMode(buttonPin, INPUT); //set a button - for changing colour
    
//LED settings
    FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    
//RTC Clock settings
    setSyncProvider(RTC.get);   // the function to get the time from the RTC

//****settings to edit the time on the real time clock ******


//setTime(12,59, 55, 05, 01, 2020);   //this sets the system time set to GMT without the daylight saving added. 
//RTC.set(now());   //sets time onto the RTC. Make sure to comment this out and then re-upload after setting the RTC. -- (setTime() and now() are Part of the Time Library)


//**** Un-comment above 2 lines to set the time and load onto chip then comment out the lines and re-load onto the chip********

    BT.begin(9600);
    BT.println("Connected to WordClock");
    
}


long previousMillis = 0;
long previousMillis2 = 0;
long interval = 500; //how long in milli seconds to update clock


void loop()
{

//bluetooth settings
    GetInput();
    showNewData();
    BTChangedatetime();

// run the Clock face LEDs every 500ms

unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   
   
    Clockset();
  }

    houranimataion(); //hour animataion to run at full speed
    BrightnessSet();  // changing the brightness

  uint8_t buttonState;
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH) {
    colour_hue++;
   delay(100);
  }  
}


void Clockset(){
// ********************* Calculate offset for Daylight saving hours (UK) *********************
    int y = year();                          // year in 4 digit format
    uint8_t Mar_x = 31 - (4 + 5*y/4) % 7;      // will find the day of the last sunday in march
    uint8_t Oct_x = 31 - (1 + 5*y/4) % 7;       // will find the day of the last sunday in Oct
    uint8_t DST;
                                               
    // *********** Test DST: BEGINS on last Sunday of March @ 2:00 AM *********
    if(month() == 3 && day() == Mar_x && hour() >= 2)
       {                                   
        DST = 1;                           // Daylight Savings Time is TRUE (add one hour)
       }
    if((month() == 3 && day() > Mar_x) || month() > 3)
       {
        DST = 1;
       }
    // ************* Test DST: ENDS on Last Sunday of Oct @ 2:00 AM ************  
    
    if(month() == 10 && day() == Oct_x && hour() >= 2)
       {
        DST = 0;                            // daylight savings time is FALSE (Standard time)
       }
    if((month() == 10 && day() > Oct_x) || month() > 10 || month() < 3 || month() == 3 && day() < Mar_x || month() == 3 && day() == Mar_x && hour() < 2)
       {
        DST = 0;
       }
    
    
    Hour_DST = hour()+DST; //Add the DST to the hour to get correct DST
    if(hour() == 23 && DST == 1)
    {
    Hour_DST = 00;
    }
    
  // ********************************************************************************
    // the first 8 seconds of the hour is a special animation if its past this time set time as normal
    
    if (minute() ==00  && second() > 8 || minute() >00)  {     
    FastLED.clear (); // reset the LEDs prevents old times staying lit up 
    
    ITs(); // light up Its LEDs
    
    if (minute() >= 5 && minute() < 10 || minute() >= 55 && minute() < 60) Five_t();    // if functions to set the time minutes
    if (minute() >= 10 && minute() < 15 || minute() >= 50 && minute() < 55) Ten_t();
    if (minute() >= 15 && minute() < 20 || minute() >= 45 && minute() < 50 ) Quarter_t();
    if (minute() >= 20 && minute() < 25 || minute() >= 40 && minute() < 45) Twenty_t();
    if (minute() >= 25 && minute() < 30 || minute() >= 35 && minute() < 40) TwentyFive_t();
    if (minute() >= 30 && minute() < 35) Half_t();
    
    if (minute() >= 5 && minute() < 35) { // this sets the 'past' light so it only lights up for first 30 mins
      Past_t();
      Hour_t();
    }
    
    if (minute() >= 35 && minute() < 60 && hour() <= 23) {
      To_t();
      Hour_t1();
    }
    
    if (minute() >= 0 && minute() < 5) { // sets the 'oclock' light if the time is on the hour
      Hour_t();
      Oclock_t();
    }
    
    
    //*************** light up the Happybday on birthdays ******************
    
    if(month() == 1 && day() ==6 || month() == 2 && day() ==14){
      
      H_appy();
      B_day();
      FastLED.show();
      }
    
    //******************************************************
    FastLED.show();
    
    }
    
    // **************animations for quarter to qurter past and half past the hour *******************
    
    if(minute() == 45 && second() ==00){
      FastLED.clear ();
      ITs_animate();
      delay(500);
      Quarter_animate();
      To_animate();
      delay(500);
      Hour_1_animate();
      FastLED.show();
      }
    
    if(minute() == 15 && second() ==00){
      FastLED.clear ();
      ITs_animate();
      delay(500);
      Quarter_animate();
      Past_animate();
      delay(500);
      Hour_animate();
      FastLED.show();
      }
    
    if(minute() == 30 && second() ==00){
      FastLED.clear ();
      ITs_animate();
      delay(500);
      Half_animate();
      Past_animate();
      delay(500);
      Hour_animate();
      FastLED.show();
      }

}   //end of clock set




// voids for lighting the LEDs 

void H_appy() 
{
for (i = 0; i < 5; i++) { 
  leds[Happy[i]] += CHSV( gHue+(i*10), 250, 200);
  EVERY_N_MILLISECONDS( 10 ) { gHue++; } //change the colour of the animation
  
  }
}

void B_day() 
{
for (i = 0; i < 8; i++) { 
  leds[Birthday[i]] += CHSV( gHue+100+(i*10), 250, 200);; 
  EVERY_N_MILLISECONDS( 10 ) { gHue++; } //change the colour of the animation
  }
}


void ITs() 
{
for (i = 0; i < 4; i++) { // loop index to set the correct numbers of LEDs to light up in the defined LED array 

  leds[it_s[i]] = CRGB::Black; 
  leds[it_s[i]] += CHSV( colour_hue, 250, 250); //sets the colour of the LEDs
  
  }
}


void Five_t() 
{
for (i = 0; i < 4; i++) { 
  leds[Five[i]] += CHSV( colour_hue, 250, 250);;
  }
}

void Ten_t() 
{
for (i = 0; i < 3; i++) { 
  leds[Ten[i]] += CHSV( colour_hue, 250, 250);;
  }
}
void Quarter_t() {
for (i = 0; i < 7; i++) { 
  leds[Quarter[i]] += CHSV( colour_hue, 250, 250);; 
  }
}



void ITs_animate() 
{
for (i = 0; i < 4; i++) { // loop index to set the correct numbers of LEDs to light up in the defined LED array 
  leds[it_s[i]] += CHSV( colour_hue, 250, 250);; //sets the colour of the LEDs
  FastLED.show();
  delay(animate_speed);
  }
}
void Quarter_animate() {
for (i = 0; i < 7; i++) { 
  leds[Quarter[i]] += CHSV( colour_hue, 250, 250);; 
  FastLED.show();
  delay(animate_speed);
  }
}

void Half_animate() {
for (i = 0; i < 4; i++) { 
  leds[Half[i]] += CHSV( colour_hue, 250, 250);;
 FastLED.show();
  delay(animate_speed);
}
}

void Past_animate() {
for (i = 0; i < 4; i++) { 
  leds[Past[i]] += CHSV( colour_hue, 250, 250);; 
  FastLED.show();
  delay(animate_speed);
  }
}

void To_animate() {
for (i = 0; i < 2; i++) { 
  leds[To[i]] += CHSV( colour_hue, 250, 250);;
  FastLED.show();
  delay(animate_speed);
  }
}


void Hour_animate() {
for(i = 0; i < Char_array[Hour_DST]; i++) { 
  leds[hourarray[Hour_DST][i]] += CHSV( colour_hue, 250, 250);;
  FastLED.show();
  delay(animate_speed);
  }
}

void Hour_1_animate() {

for(i = 0; i < Char_array[(Hour_DST)+1]; i++) { 
  leds[hourarray[(Hour_DST)+1][i]] += CHSV( colour_hue, 250, 250);;
  FastLED.show();
  delay(animate_speed);
  }
}


void Oclock_animate() {
for (i = 0; i < 6; i++) { 
  leds[Oclock [i]] += CHSV( colour_hue, 250, 250);;
    FastLED.show();
  delay(animate_speed);
  }
}

void Twenty_t() {
for (i = 0; i < 6; i++) { 
  leds[Twenty[i]] += CHSV( colour_hue, 250, 250);;
  }
}

void TwentyFive_t() {
for (i = 0; i < 10; i++) { 
  leds[TwentyFive[i]] += CHSV( colour_hue, 250, 250);;}
}

void Half_t() {
for (i = 0; i < 4; i++) { 
  leds[Half[i]] += CHSV( colour_hue, 250, 250);;}
}

void Hour_t() {
for(i = 0; i < Char_array[Hour_DST]; i++) {
  leds[hourarray[Hour_DST][i]] = CRGB::Black; 
  leds[hourarray[Hour_DST][i]] += CHSV( colour_hue, 250, 250);;}
}



void Hour_t1() {
for(i = 0; i < Char_array[Hour_DST+1] ; i++) { 
  leds[hourarray[Hour_DST+1][i]] += CHSV( colour_hue, 250, 250);;}
}

void Past_t() {
for (i = 0; i < 4; i++) { 
  leds[Past[i]] += CHSV( colour_hue, 250, 250);; }
}

void To_t() {
for (i = 0; i < 2; i++) { 
  leds[To[i]] += CHSV( colour_hue, 250, 250);;
  }
}


void Oclock_t() {
for (i = 0; i < 6; i++) { 
  leds[Oclock [i]] = CRGB::Black;
  leds[Oclock [i]] += CHSV( colour_hue, 250, 250);;
  
  }
}



void Animation(){ /// animation to run on the hour
  
    // random colored speckles that blink in and fade smoothly
  
 fadeToBlackBy( leds, NUM_LEDS, 15);
 uint8_t pos = random8(NUM_LEDS);
 leds[pos] += CHSV( gHue + random8(64), 200, 250);
 EVERY_N_MILLISECONDS( 20) { gHue++; } //change the colour of the animation
 delay(10);


}

void houranimataion() {
//************* animation for on the hour ************************

  if(minute() == 00 && second() >= 00 && second() <=8){
    Animation();
    }
  if(minute() == 00 && second() == 01){    // do a 10sec animation when it hits the hour
    ITs(); 
    FastLED.show();
    }
  if(minute() == 00 && second() >1 && second() <=2){   
    ITs();
    Hour_t();
    FastLED.show();
    }
  if(minute() == 00 && second() >2 && second() <=8){    // do a 8sec animation when it hits the hour
    ITs();
    Hour_t();
    Oclock_t();
    FastLED.show();
    }
}



//******* Brightness settings to work with the light dependent resistor ******************

void BrightnessSet () {
   int ambiant_Light = analogRead(pResistor);    // read the value of the photoresisor
   int Brightness_Set = map(ambiant_Light, 0, 500, 10, 200); // map the ambiant light value to a LED brightness value
    
   if (ambiant_Light >= 500){ 
      FastLED.setBrightness(200);
    }
   if (ambiant_Light < 500){
      FastLED.setBrightness(Brightness_Set);  // set the brightness of the LEDs
   }
}



// ******************* bluetooth implementation to change date and time ********************************


void GetInput() { //take the message set by bluetooth and then add all the characters together in a char array
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    
    while (BT.available() > 0 && newData == false) {    //create a char array untill you get a /n signal from bluetooth
        rc = BT.read();

        if (rc != endMarker) {
            receivedData[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
                }
            }
        else {
            receivedData[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
            }
    }
}



void showNewData() { //If the message sent is the same as the trigger word "settime" then ask for user to enter date and time
    if (newData == true && (strcmp(SetTime,receivedData) == 0 || strcmp("settime",receivedData) == 0 || strcmp("SETTIME",receivedData) == 0 || strcmp("SetTime/r",receivedData) == 0 || strcmp("set time",receivedData) == 0) ){
        
        BT.println("Set the Time & Date as: hh,mm,ss,dd,mm,yyyy");
        newData = false;
        ChangeTime = true; // set a switch to true that time is going to be changed
    }


    if (newData == true && (strcmp(SetBday,receivedData) == 0 || strcmp("setbday",receivedData) == 0 || strcmp("SETBDAY",receivedData) == 0 || strcmp("SetBday/r",receivedData) == 0 || strcmp("set bday",receivedData) == 0) ){

        newData = false;
        Changebday = true; // set a switch to true that time is going to be changed
    }

    
    if (newData == true && strcmp(SetTime,receivedData) != 0 && ChangeTime == false) {
          newData = false;
          int a = strcmp(SetTime,receivedData);
          String Cmd = (String)"Cmd not recognised ("+ a + ")"; // if the user input isnt same as trigger word then inform user command not recognised
           BT.println(Cmd);
        }
        }


 void BTChangedatetime()  {
    
    if (newData == true && ChangeTime == true){ // if a new message has been recieved and the Change time switch is active change the time
   
    char *strings[10];    //following code parses out the date based on being delimited by commas fullstops etc. this gives 
    char *ptr = NULL;
    byte index = 0;
    
    ptr = strtok(receivedData, " :/,.");  // takes a list of delimiters 
    
    while(ptr != NULL){
        strings[index] = ptr;
        index++;
        ptr = strtok(NULL, " :/,.");  // takes a list of delimiters
        }

    long hr = atol(strings[0]); // take the parsed date from array which corresponds to hour minute seconds ect. 
    long mm = atol(strings[1]);
    long ss = atol(strings[2]);
    long dd = atol(strings[3]);
    long mth = atol(strings[4]);
    long yyyy = atol(strings[5]);
    String Dateset =  (String)dd+"/"+mth+"/"+yyyy;  //create a string to update user interface to bluetooth 
    String Timeset = (String)hr+":"+mm+":"+ss;
    
    BT.println("Time set as: " + Timeset);
    BT.println("Date set as: " + Dateset);
    
    setTime(hr,mm,ss,dd,mth,yyyy);   //this sets the system time set to GMT without the daylight saving added. 
    RTC.set(now());   
    
            
   newData = false;
   ChangeTime = false;
  }
}

// TO ADD setting the bday and saving it to the eprom memory
    
