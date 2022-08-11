#include "simpletools.h"                      // Include simple tools
#include "servo.h"
#include "ws2812.h"
#include "adcDCpropab.h"

#define LED_PIN 0
#define HAPPY_LED_PIN 1
#define ANGRY_LED_PIN 2
#define SAD_LED_PIN 3
#define LOVE_LED_PIN 4

#define TOUCH_PIN 5
#define SOUND_PIN 6
#define PIEZO_PIN 7

#define IR_LED_PIN 8
#define IR_SENSOR_PIN 9

int irSense;
ws2812 * ledDriver;

int currentEmotionalStateIndex = 0;

void defaultHappyState();
void elevatedHappyState();
void defaultAngryState();
void elevatedAngryState();
void defaultSadState();
void elevatedSadState();
void defaultLoveState();
void elevatedLoveState();

void playHappySong();
void playSadSong();
void playLoveSong();

float happySong[] = {261.63, 349.23, 349.23, 349.23, 440, 523.25, 440, 349.23,
                     392, 329.63, 261.63, 523.25, 440, 349.23,
                     261.63, 349.23, 349.23, 349.23, 440, 523.25, 440, 349.23,
                     392, 261.63, 349.23};

int happySongNoteDuration[] = {500, 500, 500, 500, 500, 500, 500, 1000, 500, 500, 1000, 500, 500, 1000,
                              500, 500, 500, 500, 500, 500, 500, 1000, 1000, 1000, 1000};
                              
           
float loveSong[] = {698.46, 1318.5, 1174.7, 783.99, 698.46, 1318.5, 1174.5, 783.99, 698.46,
                    1046.5, 932.33, 698.46, 587.33,
                    1046.5, 932.33, 698.46, 932.33, 783.99, 698.46, 587.33, 466.16};



int loveSongNoteDuration [] = {500, 350, 300, 450, 350, 500, 300, 350, 350,
                               500, 300, 350, 350,
                               650, 300, 550, 350, 300, 300, 350, 1000};

float sadSong[] = {311.13, 311.13, 311.13, 349.23, 293.67, 293.67, 311.13, 311.13};

int sadSongNoteDuration [] = {1000, 1000, 1000, 500, 500, 500, 500, 1000};

int main()                                    // Main function
{  
  // Add startup code here.
  ledDriver = ws2812_open();
  
  adc_init(21, 20, 19, 18);
  float lrV, udV;
  
  while(1)
  {    
    // Add main loop code here.
    switch(currentEmotionalStateIndex)
    {
      case 0:
        defaultHappyState();
        if(input(SOUND_PIN)){
          currentEmotionalStateIndex += 4;
        }          
        break;
      case 1:
        defaultAngryState();
        if(input(TOUCH_PIN)){
          currentEmotionalStateIndex += 4;
        }          
        break;
      case 2:
        playSadSong();
        defaultSadState();
        break;
      case 3:
        playLoveSong();
        defaultLoveState();
        break;
      case 4:
        playHappySong();
        elevatedHappyState();
        break;
      case 5:
        elevatedAngryState();
      default:
        break; 
    }      
    
    udV = adc_volts(3);                             // Check A/D 2                
    lrV = adc_volts(2);                             // Check A/D 3
   
    putChar(HOME);                                  // Cursor -> top-left "home"
    print("Up/Down = %.2f V %c\n", udV, CLREOL);    // Display voltage
    print("Left/Right = %.2f V %c\n", lrV, CLREOL); // Display voltage
    print("Emotion state = %d\n", currentEmotionalStateIndex);
    
    if(lrV > 4.5 && currentEmotionalStateIndex <= 3) {
      currentEmotionalStateIndex++;
      pause(250);
    } else if(lrV < 0.5 && currentEmotionalStateIndex >= 0) {
      currentEmotionalStateIndex--;
      pause(250);
    }      
    if(udV > 4.5 && currentEmotionalStateIndex <= 3) {
      currentEmotionalStateIndex += 4;
    } else if (udV < 0.5 && currentEmotionalStateIndex >= 4) {
      currentEmotionalStateIndex -= 4;
    }        
    pause(1);
    /*
    freqout(6, 1, 38000);
    irSense = input(7);
    
    if(irSense == 1)
    {
      print("%c N", HOME);
      low(2);
      servo_speed(12,200);
      servo_speed(13,-200);
    } else {
      print("%c Y", HOME);
      high(2);
      servo_speed(12,200);
      servo_speed(13,0);
    }
    print("%c irSense = %d", HOME, irSense);
    pause(100);*/
  }  
}


void defaultHappyState()
{
  static int ledCounter = 0;
  static int counter = 0;
  int lightA[] = {COLOR_YELLOW};
  int lightB[] = {COLOR_BLACK};
  high(HAPPY_LED_PIN);
  low(ANGRY_LED_PIN);
  low(SAD_LED_PIN);
  low(LOVE_LED_PIN);
  if(ledCounter <= 5) {
    ws2812_set(ledDriver, LED_PIN, lightA, 1);
    ledCounter++;
  } else if (ledCounter <= 10){
    ws2812_set(ledDriver, LED_PIN, lightB, 1);
    ledCounter++;
  } else {
    ledCounter = 0;
  }    
  if(counter <= 20){
    servo_speed(12,50);
    servo_speed(13,-50);
    counter++;
  } else if (counter <= 40) {
    servo_speed(12,-50);
    servo_speed(13,50);
    counter++;
  } else {
    counter = 0;
  }
}

void playHappySong()
{
  static int noteIndex = 0;
  static int noteCounter = 0;
  if(noteCounter == 0){
    freqout(PIEZO_PIN, happySongNoteDuration[noteIndex], happySong[noteIndex]);
    noteCounter = 1;
    noteIndex++;
  } else {
    noteCounter--;
  }   
  if(noteIndex == 25){
    currentEmotionalStateIndex -= 4;
    noteIndex = 0;
  }     
}  

void elevatedHappyState()
{
  static int ledCounter = 0;
  static int counter = 0;
  int lightA[] = {COLOR_YELLOW};
  int lightB[] = {COLOR_BLACK};
  high(HAPPY_LED_PIN);
  low(ANGRY_LED_PIN);
  low(SAD_LED_PIN);
  low(LOVE_LED_PIN);
  if(ledCounter <= 1) {
    ws2812_set(ledDriver, LED_PIN, lightA, 1);
    ledCounter++;
  } else if (ledCounter <= 2){
    ws2812_set(ledDriver, LED_PIN, lightB, 1);
    ledCounter++;
  } else {
    ledCounter = 0;
  }    
  if(counter <= 5){
    servo_speed(12,200);
    servo_speed(13,-200);
    counter++;
  } else if (counter <= 10) {
    servo_speed(12,-200);
    servo_speed(13,200);
    counter++;
  } else {
    counter = 0;
  }   
}


void defaultAngryState()
{
  static int counter = 0;
  int light[] = {COLOR_RED};
  high(ANGRY_LED_PIN);
  low(HAPPY_LED_PIN);
  low(SAD_LED_PIN);
  low(LOVE_LED_PIN);
  ws2812_set(ledDriver, LED_PIN, light, 1);
  if(counter <= 5) {
    servo_speed(12, 200);
    servo_speed(13,-200);
    counter++;
  } else if(counter <= 7){
    servo_speed(12,200);
    servo_speed(13,200);
    counter++;
  } else if(counter <= 13) {
    servo_speed(12,200);
    servo_speed(13,-200);
    counter++;
  } else if(counter <= 15) {
    servo_speed(12, -200);
    servo_speed(13, -200);
    counter++;
  } else if(counter <= 20) {
    servo_speed(12,200);
    servo_speed(13,-200);
    counter++;
  } else if(counter <= 22) {
    servo_speed(12, 200);
    servo_speed(13,200);
    counter++;
  } else {
    counter = 0;
  }    
}  

void elevatedAngryState()
{
  static int counter = 0;
  int lightA [] = {COLOR_RED};
  int lightB [] = {COLOR_BLACK};
  if(counter%2){
    ws2812_set(ledDriver, LED_PIN, lightA, 1);  
  } else {
    ws2812_set(ledDriver, LED_PIN, lightB, 1);
  }    
  if(counter == 70){
    currentEmotionalStateIndex -= 4;
    counter = 0;
    return;
  }    
  freqout(PIEZO_PIN, 100, 2000);
  counter++;
}  

void playSadSong()
{
  static int noteIndex = 0;
  static int noteCounter = 0;
  if(noteCounter == 0){
    freqout(PIEZO_PIN, sadSongNoteDuration[noteIndex], sadSong[noteIndex]);
    noteCounter = 1;
    noteIndex++;
  } else {
    noteCounter--;
  }   
  if(noteIndex == 7){
    noteIndex = 0;
  }   
}  

void defaultSadState()
{
  static int counter = 0;
  high(SAD_LED_PIN);
  low(ANGRY_LED_PIN);
  low(HAPPY_LED_PIN);
  low(LOVE_LED_PIN);
  if(counter <= 100) {
    ws2812_set(ledDriver, LED_PIN, COLOR_GREEN, 1);
    counter++;
  } else if (counter <= 200){
    ws2812_set(ledDriver, LED_PIN, COLOR_BLACK, 1);
    counter++;
  } else {
    counter = 0;
  }    
}

void playLoveSong()
{
  static int noteIndex = 0;
  static int noteCounter = 0;
  if(noteCounter == 0){
    freqout(PIEZO_PIN, loveSongNoteDuration[noteIndex], loveSong[noteIndex]);
    noteCounter = 1;
    noteIndex++;
  } else {
    noteCounter--;
  }   
  if(noteIndex == 21){
    noteIndex = 0;
  }   
}  

void defaultLoveState()
{
  static int counter = 0;
  high(LOVE_LED_PIN);
  low(ANGRY_LED_PIN);
  low(SAD_LED_PIN);
  low(HAPPY_LED_PIN);
  int lightA[] = {COLOR_MAGENTA};
  int lightB[] = {COLOR_BLACK};
  if(counter <= 1) {
    ws2812_set(ledDriver, LED_PIN, lightA, 1);
    counter++;
  } else if (counter <= 2){
    ws2812_set(ledDriver, LED_PIN, lightB, 1);
    counter++;
  } else {
    counter = 0;
  }    
}