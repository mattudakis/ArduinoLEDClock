// code includes birthday messages and also automatic daylight saving calculations.

#include <FastLED.h>
#include <DS3232RTC.h>      // https://github.com/JChristensen/DS3232RTC
#include <SoftwareSerial.h>
#include <EEPROM.h>

FASTLED_USING_NAMESPACE

#define DATA_PIN        6
#define LED_TYPE        WS2811
#define COLOR_ORDER     GRB
#define NUM_LEDS        121
#define PHOTO_RESISTOR  A0
#define BUTTON_PIN      2

CRGB leds[NUM_LEDS];
SoftwareSerial BT(8, 9); //  pin D8 to RXD p-in D9 purple to TXD

//Global Variables
uint8_t animate_speed = 100;
uint8_t Hour_DST;
int DST;
uint8_t colour_hue = 35;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns


const byte numChars = 35;
char receivedData[numChars];   // an array to store the received data
boolean changingTime = false;
boolean addingBday = false;
boolean removingBday = false;
boolean listingBday = false;
boolean newData = false;


// arrays for the LEDs
enum wordLEDs {
  ITS,
  FIVE,
  TEN,
  QUARTER,
  TWENTY,
  TWENTY_FIVE,
  HALF,
  PAST,
  TO,
  OCLOCK,
  HAPPY,
  BIRTHDAY
};

uint8_t wordArray[][11] = {
  {119,118,116,115},
  {83,84,85,86},
  {91,90,89},
  {102,103,104,105,106,107,108},
  {97,96,95,94,93,92},
  {83,84,85,86,92,93,94,95,96,97},
  {78,79,80,81},
  {70,69,68,67},
  {56,57},
  {5,4,3,2,1,0},
  {55,76,77,98,99},
  {43,44,65,66,87,88,109,110}
};

uint8_t wordLengths[] = {4,4,3,7,6,10,4,4,2,6,5,8}; // this is the size of each of the above arrays for indexing the hourarray later in script                       

uint8_t hourArray[][7] = { 
  {59,60,61,62,63,64}, // this outlnes which LEDs to light up, 0-24 arrays, 0 being twleve 1=one 2=two... 24=twelve
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
                        
uint8_t hourLengths[] = {6,3,3,5,4,4,3,5,5,4,3,6,6,3,3,5,4,4,3,5,5,4,3,6,6}; // this is the size of each of the above arrays for indexing the hourarray later in script                       

long previousMillis = 0;
long interval = 500; //how long in milli seconds to update clock

// settings to implement on startup

void setup() { 

//Pin Settings
  pinMode(PHOTO_RESISTOR, INPUT);// Set photo resistor
  pinMode(BUTTON_PIN, INPUT); //set a button - for changing colour
    
//LED settings
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    
//RTC Clock settings
  RTC.begin();
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  
  //****settings to edit the time on the real time clock ******
//  setTime(18,50, 00, 14, 05, 2023);   //this sets the system time set to GMT without the daylight saving added. 
//  RTC.set(now());   //sets time onto the RTC. Make sure to comment this out and then re-upload after setting the RTC. -- (setTime() and now() are Part of the Time Library)
  //**** Un-comment above 2 lines to set the time and load onto chip then comment out the lines and re-load onto the chip********


  // **** comment this out after this sketch is uploaded for the first time ****
  // **** it stores the number of birthdays stored in the EEPROM
//  EEPROM.write(0, 0);

// To add birthdates without bluetooth use this function, **** comment this out after loding for the first time ****
//  addManualBday(26, 3); // pass the variables (day, month) 


BT.begin(9600);
BT.println("Connected to WordClock");
    
}


void loop() {
  //bluetooth settings
  bluetoothGetInput();
  bluetoothCheckInput();
  bluetoothChangeTime();
  bluetoothAddBirthday();
  bluetoothRemoveBirthday();
  
  // run the Clock face LEDs every 500ms
  unsigned long currentMillis = millis();
    
  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;
       
    Clockset();
  }
  
  hourAnimation(); //hour animataion to run at full speed
  brightnessSet();  // changing the brightness
    
  if (digitalRead(BUTTON_PIN) == HIGH) {
     colour_hue++;
     delay(100);
  }  
}


void Clockset(){
  
  // ********************* Calculate offset for Daylight saving hours UK/Europe *********************
  // for UK/ Europe DST starts on last Sunday March at 2:00am until Last Sunday October at 2:00am
  int MonthStartDST = 3; //dst starts in march
  int MonthEndDST   = 10; //dst ends in october
  int TimeDSTSwitch = 2; // clocks change at 2:00am
  int y = year();
  
  int DayStartDST = calculateLastSundayInMonth(y, MonthStartDST); //find the last sunday in the month
  int DayEndDST = calculateLastSundayInMonth(y, MonthEndDST); //find the last sunday in the month

  SetDST(MonthStartDST, DayStartDST, MonthEndDST, DayEndDST, TimeDSTSwitch);



  // ********************* Calculate offset for Daylight saving hours USA *********************
//  // for USA DST starts on second Sunday March at 2:00am until First Sunday November at 2:00am
//  int MonthStartDST = 3; //dst strats in march
//  int MonthEndDST   = 11; //dst ends in october
//  int TimeDSTSwitch = 2; // clocks change at 2:00am
//  int y = year();
//  
//  int DayStartDST = calculateFirstSundayInMonth(y, MonthStartDST) + 7; // add 7 to get the second week of the month
//  int DayEndDST = calculateFirstSundayInMonth(y, MonthEndDST);
//
//  SetDST(MonthStartDST, DayStartDST, MonthEndDST, DayEndDST, TimeDSTSwitch);
  
  // *****************************************                                             

  
  
  // the first 8 seconds of the hour is a special animation if its past this time set time as normal
  
  if (minute() == 00  && second() > 8 || minute() > 00)  {     
    FastLED.clear (); // reset the LEDs prevents old times staying lit up 
    
    lightWordLEDs(ITS); // light up Its LEDs
    
    if (minute() >= 5 && minute() < 10 || minute() >= 55 && minute() < 60) {
      lightWordLEDs(FIVE);    // if functions to set the time minutes
    }
    
    if (minute() >= 10 && minute() < 15 || minute() >= 50 && minute() < 55) { 
      lightWordLEDs(TEN);
    }
    
    if (minute() >= 15 && minute() < 20 || minute() >= 45 && minute() < 50 ) { 
      lightWordLEDs(QUARTER);
    }
    
    if (minute() >= 20 && minute() < 25 || minute() >= 40 && minute() < 45) {
      lightWordLEDs(TWENTY);
    }
    
    if (minute() >= 25 && minute() < 30 || minute() >= 35 && minute() < 40) {
      lightWordLEDs(TWENTY_FIVE);
    }
    
    if (minute() >= 30 && minute() < 35){
      lightWordLEDs(HALF);
    }
    
    if (minute() >= 5 && minute() < 35) { // this sets the 'past' light so it only lights up for first 30 mins
      lightWordLEDs(PAST);
      lightHourLEDs(Hour_DST);
    }
    
    if (minute() >= 35 && minute() < 60 && hour() <= 23) {
      lightWordLEDs(TO);
      lightHourLEDs(Hour_DST+1);
    }
    
    if (minute() >= 0 && minute() < 5) { // sets the 'oclock' light if the time is on the hour
      lightHourLEDs(Hour_DST);
      lightWordLEDs(OCLOCK);
    }
    
    
    //*************** light up the Happy birthday on birthdays ******************
    int birthdayCount = EEPROM.read(0);
    int birthdayAddress = 1;

    for (int i = 0; i < birthdayCount; i++) {
      int bdayDay = EEPROM.read(birthdayAddress);
      int bdayMonth = EEPROM.read(birthdayAddress + 1);
      
      if(month() == bdayMonth && day() ==bdayDay) {
        lightBirthdayLEDs(HAPPY);
        lightBirthdayLEDs(BIRTHDAY);
        FastLED.show();
      }
      
      birthdayAddress += 2;
    }
    
    FastLED.show();
      
  }
  
  // **************animations for quarter to qurter past and half past the hour *******************
  if(minute() == 45 && second() ==00){
    FastLED.clear ();
    animateWordLEDs(ITS);
    delay(500);
    animateWordLEDs(QUARTER);
    animateWordLEDs(TO);
    delay(500);
    animateHourLEDs(Hour_DST+1);
    FastLED.show();
  }
  
  if(minute() == 15 && second() ==00){
    FastLED.clear ();
    animateWordLEDs(ITS);
    delay(500);
    animateWordLEDs(QUARTER);
    animateWordLEDs(PAST);
    delay(500);
    animateHourLEDs(Hour_DST);
    FastLED.show();
  }
  
  if(minute() == 30 && second() ==00){
    FastLED.clear ();
    animateWordLEDs(ITS);
    delay(500);
    animateWordLEDs(HALF);
    animateWordLEDs(PAST);
    delay(500);
    animateHourLEDs(Hour_DST);
    FastLED.show();
  }
}   //end of clock set



//******* functions to turn LEDs on ******************

void lightWordLEDs(enum wordLEDs word) {
  for (uint8_t i = 0; i < wordLengths[word]; i++) {
    leds[wordArray[word][i]] = CRGB::Black; 
    leds[wordArray[word][i]] += CHSV(colour_hue, 250, 250);
  }
}

void lightHourLEDs(uint8_t Hour) {
  for(uint8_t i = 0; i < hourLengths[Hour]; i++) {
    leds[hourArray[Hour][i]] = CRGB::Black; 
    leds[hourArray[Hour][i]] += CHSV( colour_hue, 250, 250);
  }
}


void lightBirthdayLEDs(enum wordLEDs word) {
  for (uint8_t i = 0; i < wordLengths[word]; i++) {
    leds[wordArray[word][i]] += CHSV( gHue+(i*10), 250, 250);
    EVERY_N_MILLISECONDS( 1000 ) { gHue++; } //change the colour of the animation
  }
}

void animateWordLEDs(enum wordLEDs word) {
  for (uint8_t i = 0; i < wordLengths[word]; i++) { 
    leds[wordArray[word][i]] += CHSV(colour_hue, 250, 250);
    FastLED.show();
    delay(animate_speed);
  }
}

void animateHourLEDs(uint8_t Hour) { 
  for(uint8_t i = 0; i < hourLengths[Hour]; i++) {
    leds[hourArray[Hour][i]] += CHSV( colour_hue, 250, 250);
    FastLED.show();
    delay(animate_speed);
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

void hourAnimation() {
//animation for on the hour
  if(minute() == 00 && second() >= 00 && second() <=8){
    Animation();
  }
  
  if(minute() == 00 && second() == 01){    // do a 10sec animation when it hits the hour
    lightWordLEDs(ITS); 
    FastLED.show();
  }
  
  if(minute() == 00 && second() >1 && second() <=2){   
    lightWordLEDs(ITS);
    lightHourLEDs(Hour_DST);
    FastLED.show();
  }
  
  if(minute() == 00 && second() >2 && second() <=8){    // do a 8sec animation when it hits the hour
    lightWordLEDs(ITS);
    lightHourLEDs(Hour_DST);
    lightWordLEDs(OCLOCK);
    FastLED.show();
  }
}



//******* Brightness settings to work with the light dependent resistor ******************

void brightnessSet () {
  int ambiant_Light = analogRead(PHOTO_RESISTOR);    // read the value of the photoresisor
  int Brightness_Set = map(ambiant_Light, 0, 500, 10, 200); // map the ambiant light value to a LED brightness value
  
  if (ambiant_Light >= 500){ 
    FastLED.setBrightness(200);
  }
  if (ambiant_Light < 500){
    FastLED.setBrightness(Brightness_Set);  // set the brightness of the LEDs
  }
}



// ******************* bluetooth implementation to change date and time ********************************

void bluetoothGetInput() { //take the message set by bluetooth and then add all the characters together in a char array
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

void bluetoothCheckInput() { //If the message sent is the same as the trigger word "settime" then ask for user to enter date and time
  if (newData == true && (strcasecmp("settime",receivedData) == 0)) {
    BT.println();
    BT.println("Set the Time (without daylight savings time added) & Date as: hh,mm,ss,dd,mm,yyyy");
    newData = false;
    changingTime = true; // set a switch to true that time is going to be changed
  }
  
  if (newData == true && (strcasecmp("addbday",receivedData) == 0)){
    BT.println();
    BT.println("Enter birthday to add as: dd/mm");
    newData = false;
    addingBday = true; // set a switch to true that time is going to be changed
  }
  
  if (newData == true && (strcasecmp("removebday",receivedData) == 0)){
    BT.println();
    BT.println("Enter birthday to remove as: dd/mm");
    newData = false;
    removingBday = true; // set a switch to true that time is going to be changed
  }

  if (newData && strcasecmp("gettime", receivedData) == 0) {
    char hoursString[3];
    char minutesString[3];
    char secondsString[3];
    
    BT.println();
    BT.print("Date: ");
    BT.print(day());
    BT.print("/");
    BT.print(month());
    BT.print("/");
    BT.println(year());

    sprintf(hoursString, "%02d", hour());
    sprintf(minutesString, "%02d", minute());
    sprintf(secondsString, "%02d", second());
    
    BT.print("Time: ");
    BT.print(hoursString);
    BT.print(":");
    BT.print(minutesString);
    BT.print(":");
    BT.println(secondsString);

    newData = false;
  }

  if (newData && strcasecmp("listbday", receivedData) == 0) {
      int birthdayCount = EEPROM.read(0);
      int birthdayAddress = 1;
  
      if (birthdayCount == 0) {
        BT.println();
        BT.print("Birthday count: ");
        BT.println(birthdayCount);
      } else {
        BT.println();
        for (int i = 0; i < birthdayCount; i++) {
          int bdayMonth = EEPROM.read(birthdayAddress);
          int bdayDay = EEPROM.read(birthdayAddress + 1);
    
          BT.print("Birthday ");
          BT.print(i + 1);
          BT.print(": ");
          BT.print(bdayMonth);
          BT.print("/");
          BT.println(bdayDay);
          
          birthdayAddress += 2;
        }
      }
      
      newData = false;
    }
  
  if (newData == true && 
      strcasecmp("settime",receivedData) != 0 && changingTime == false &&
      strcasecmp("addbday",receivedData) != 0 && addingBday == false &&
      strcasecmp("removebday",receivedData) != 0 && removingBday == false &&
      strcasecmp("listbday",receivedData) != 0 && listingBday == false) {
      newData = false;
      // if the user input isnt same as trigger word then inform user command not recognised
      BT.println();
      BT.println("Command not recognised: " + String(receivedData));
      BT.println("Avaliable commands: ");
      BT.println("settime");
      BT.println("gettime");
      BT.println("addbday");
      BT.println("removebday");
      BT.println("listbday");
      
  }
}

 void bluetoothChangeTime(){  
  if (newData == true && changingTime == true){ // if a new message has been recieved and the Change time switch is active change the time
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

    
    setTime(hr,mm,ss,dd,mth,yyyy);   //this sets the system time set to GMT without the daylight saving added. 
    RTC.set(now());
    
    BT.print("Date set as: ");
    BT.print(dd);
    BT.print("/");
    BT.print(mth);
    BT.print("/");
    BT.println(yyyy);

    //sprintf(hoursString, "%02d", hours);
    //sprintf(minutesString, "%02d", minutes);
    //sprintf(secondsString, "%02d", seconds);
    
    BT.print("Time set as: ");
    BT.print(hr);
    BT.print(":");
    BT.print(mm);
    BT.print(":");
    BT.print(ss);
    BT.println(" ");
        
    newData = false;
    changingTime = false;
  }
}

void bluetoothAddBirthday()  {
  if (newData == true && addingBday) {
    char *ptr = strtok(receivedData, "/");
    int month = -1;
    int day = -1;

    if (ptr) {
      day = atoi(ptr);
      ptr = strtok(NULL, "/");

      if (ptr) {
        month = atoi(ptr);
      }
    }

    if (month >= 1 && month <= 12 && day >= 1 && day <= 31) {
      int birthdayCount = EEPROM.read(0);

      int birthdayAddress = (birthdayCount * 2) + 1;

      EEPROM.write(birthdayAddress, day);
      EEPROM.write(birthdayAddress + 1, month);
      EEPROM.write(0, birthdayCount + 1);
    
      BT.print("Added birthday: ");
      BT.print(day);
      BT.print("/");
      BT.println(month);
    } else {
      BT.print("Invalid birthday: ");
      BT.print(String(receivedData));
    }
    
    newData = false;
    addingBday = false;
  }
}


void bluetoothRemoveBirthday()  {
  if (newData == true && removingBday) {
    char *ptr = strtok(receivedData, "/");
    int month = -1;
    int day = -1;

    if (ptr) {
      day = atoi(ptr);
      ptr = strtok(NULL, "/");

      if (ptr) {
        month = atoi(ptr);
      }
    }

    if (month >= 1 && month <= 12 && day >= 1 && day <= 31) {
      int birthdayCount = EEPROM.read(0);
      int birthdayAddress = 1;
      bool birthdayFound = false;

      for (int i = 0; i < birthdayCount; i++) {
        if (birthdayFound) {
          EEPROM.write(birthdayAddress - 2, EEPROM.read(birthdayAddress));
          EEPROM.write(birthdayAddress - 1, EEPROM.read(birthdayAddress + 1));
        } else if (EEPROM.read(birthdayAddress) == day && EEPROM.read(birthdayAddress + 1) == month) {
          birthdayFound = true;
        }

        birthdayAddress += 2;
      }

      if (birthdayFound) {
        EEPROM.write(0, birthdayCount - 1);
      
        BT.print("Removed birthday: ");
        BT.print(day);
        BT.print("/");
        BT.println(month);
      } else {
        BT.print("Birthday not found: ");
        BT.print(String(receivedData));
      }
    }
    
    newData = false;
    removingBday = false;
  }
}


void addManualBday(int day, int month) {
      int birthdayCount = EEPROM.read(0);
      int birthdayAddress = (birthdayCount * 2) + 1;

      EEPROM.write(birthdayAddress, day);
      EEPROM.write(birthdayAddress + 1, month);
      EEPROM.write(0, birthdayCount + 1);
}


void SetDST(int MonthStartDST, int DayStartDST, int MonthEndDST, int DayEndDST, int TimeDSTSwitch) {
  
  // *********** Test if time is between DST start and end dates and set DST *********
  if ((month() > MonthStartDST && month() < MonthEndDST) || (month() == MonthStartDST && day() >= DayStartDST) || (month() == MonthEndDST && day() <= DayEndDST)) {
     // Check if the time is after 2am
    DST = 1;
    if (month() == MonthStartDST && day() == DayStartDST){
        if (hour() < TimeDSTSwitch) {
           DST = 0;
        } else {
         DST = 1; }
    }
    if (month() == MonthEndDST && day() == DayEndDST){
      if (hour() < TimeDSTSwitch) {
           DST = 1;
        } else {
         DST = 0; }
    }
  } else {
  DST = 0;
  }
  
  Hour_DST = hour()+DST; //Add the DST to the hour to get correct DST
  
  if(hour() == 23 && DST == 1) {
    Hour_DST = 00;
  }
}


// these two function use the Zeller Congruence to calculate the date of the Last or first sunday of a given month for a given year. 
int calculateLastSundayInMonth(int year, int month) {
  int d; // Number of days in the month
  if (month == 2) {
    // February
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
      d = 29; // Leap year
    } else {
      d = 28; // Non-leap year
    }
  } else if (month == 4 || month == 6 || month == 9 || month == 11) {
    // April, June, September, November
    d = 30;
  } else {
    // January, March, May, July, August, October, December
    d = 31;
  }

  int K = year % 100;
  int J = year / 100;

  int w = (d + 13 * (month + 1) / 5 + K + K / 4 + J / 4 + 5 * J) % 7;
  int diff = w - 1; // Calculate the difference from 0 (Sunday)
  
  int lastSunday = d - diff;

  return lastSunday;
}


int calculateFirstSundayInMonth(int year, int month) {
  if (month == 1) {
    month = 13;
    year = year - 1;
  }
  if (month == 2) {
    month = 14;
    year = year - 1;
  }

  int d = 1;
  int K = year % 100;
  int J = year / 100;

  int w = (d + 13 * (month + 1) / 5 + K + K / 4 + J / 4 + 5 * J) % 7;
  d = ((w + 5) % 7) + 1; // Convert so Monday=1, Sunday=7

  int firstSunday;
  if (d == 7) {
    int firstSunday = 1;
    } else  {
     int firstSunday = 8 - d;
      } 

  return firstSunday;
}

    
