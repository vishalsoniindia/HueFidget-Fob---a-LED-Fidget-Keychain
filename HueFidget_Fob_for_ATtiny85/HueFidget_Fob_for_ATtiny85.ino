/*
 * By Vishal Soni (https://www.youtube.com/vishalsoniindia)
 * 
 * Connect Data Pin Of LED Ring TO 3
 * Connect Button Pin to 2 and GND
 * Connect Buzzer Positive to 0 and GND
 * 
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <FastLED.h>

#define LED_PIN     3
#define NUM_LEDS    12

// Constants
const int interruptPin = 2; 
int speakerPin = 0;

CRGB leds[NUM_LEDS];
int ledIndex = 0;
int ledcolor = 0;
int Selected_Color = 0;
int delayBetweenLedChange = 1000;

int tone_f = 0;

bool isNextGameStarted = 1;

// Define an array of rainbow colors
CRGB rainbowColors[] = {
    CRGB::Red, CRGB::Chocolate, CRGB::Yellow, CRGB::Green,
    CRGB::Blue, CRGB::Pink, CRGB::Purple
  };

int MaxNumColors = sizeof(rainbowColors) / sizeof(rainbowColors[0]);

bool isInterrupt = 0;

// Interrupt Service Routine (ISR) function
void Button_Pressed()
{
  isInterrupt = 1;
}

void setup() {
//  Serial.begin(9600);
  // Set the interrupt pin as input
  pinMode(interruptPin, INPUT_PULLUP);

  if(digitalRead(interruptPin) == LOW){
    speakerPin = 1;
  }
  
  pinMode(speakerPin, OUTPUT);
  
  // Attach the interrupt function to the interrupt pin
  attachInterrupt(0, Button_Pressed, FALLING);
  
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(20); // Adjust the brightness (0-255) as per your preference
  intro();
  delay(100);
  TurnOffAll();
  delay(1000);
  randomSeed(analogRead(A2) + millis()); // Seed the random number generator with a combination of analog reading and milliseconds
  Selected_Color  = random(MaxNumColors); //Skiping first random
}

void loop() {
  if(isNextGameStarted == 1){
    ledIndex = 0;
    Next_color();
    isNextGameStarted = 0;
  }

  // Call the function to randomly turn on and change color for one LED
  turnOnAndChangeColorRandomly();

  if(isInterrupt){
    delay(1000);
    noInterrupts();
    if(ledcolor == Selected_Color){
//      Serial.println("correct !!");
      delayBetweenLedChange = delayBetweenLedChange - 100;
      if(delayBetweenLedChange < 0){
//        Serial.println("WON !!");
        delayBetweenLedChange = 1000;
        for(int i = 0; i < 7; i++){
          tone(speakerPin, 500); // Frequency for C note
          ColorAll( i ); 
          delay(100);
          noTone(speakerPin);
          delay(100);
        }
        delay(200);
        TurnOffAll();
        delay(1000);
      }
    }
    else{
      delayBetweenLedChange = 1000;
      intro();
      delay(100);
      TurnOffAll();
      delay(1000);
    }
    isNextGameStarted = 1; //Change color Selection
    interrupts();
    isInterrupt = 0;
  }
  
}

void turnOnAndChangeColorRandomly() {

  // Choose a random color from the array
  ledcolor = random(MaxNumColors);

  // Turn on the selected LED with the random color
  leds[ledIndex] = rainbowColors[ledcolor];

  tone(speakerPin, 600);
  delay(1);
  noTone(speakerPin);
  // Show the updated LED colors
  FastLED.show();

  // Wait for a short time before turning off the LED
  delay(delayBetweenLedChange);
  
  
  // Clear the LED to turn it off
  leds[ledIndex] = CRGB::Black;

//  // Show the updated LED colors with the LED turned off
//  FastLED.show();

  // Increase LED index in a circular loop
  ledIndex = (ledIndex + 1) % NUM_LEDS;
}

void intro() {
  ledIndex = 0;
  tone_f = 100;
  while(true){
    // Turn on the selected LED with the random color
    leds[ledIndex] = rainbowColors[ledcolor];

    tone_f = tone_f + 50;
    tone(speakerPin, tone_f);

    // Show the updated LED colors
    FastLED.show();

    // Wait for a short time before turning off the LED
    delay(100);
    noTone(speakerPin);

    if(ledIndex == NUM_LEDS - 1){
      break;
    }
    // Increase LED index in a circular loop
    ledIndex = (ledIndex + 1) % NUM_LEDS;
    ledcolor = (ledcolor+1)% MaxNumColors;
  }
}

void TurnOffAll(){
  ledIndex = 0;
  while(1){
    // Turn on the selected LED with the random color
    leds[ledIndex] = CRGB::Black;

    // Show the updated LED colors
    FastLED.show();

    if(ledIndex == NUM_LEDS - 1){
      break;
    }

    // Increase LED index in a circular loop
    ledIndex = (ledIndex + 1) % NUM_LEDS;
  }
}

void ColorAll( int color_num){
  ledIndex = 0;
  while(1){
    // Turn on the selected LED with the random color
    leds[ledIndex] = rainbowColors[color_num];

    // Show the updated LED colors
    FastLED.show();

    if(ledIndex == NUM_LEDS - 1){
      break;
    }

    // Increase LED index in a circular loop
    ledIndex = (ledIndex + 1) % NUM_LEDS;
  }
}

void Next_color(){
    Selected_Color  = random(MaxNumColors);
    ColorAll(Selected_Color);
    tone(speakerPin, 500); // Frequency for C note
    delay(100);
    noTone(speakerPin);
    delay(400);
    TurnOffAll();
    delay(100);
    ColorAll(Selected_Color);
    tone(speakerPin, 500); // Frequency for C note
    delay(100);
    noTone(speakerPin);
    delay(400);
    TurnOffAll();
}
