//Send the following characters via the serial monitor to change the mode
//
//d = detection mode.
//n = print the number of pieces on board.
//l = check all LEDs work well by making some lights.



#include <FastLED.h>

#define LED_PIN     10
#define NUM_LEDS    64
#define ROWS_START_PIN     14
#define ROWS_END_PIN       21
#define COLUMNS_START_PIN  62
#define COLUMNS_END_PIN    69

CRGB leds[NUM_LEDS];
int detect_1[64];
int i, j, ones;
byte data;


void setup() {
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(255);
    Serial.begin(115200);
    for(int i = ROWS_START_PIN; i <= ROWS_END_PIN; i++){
      pinMode(i,OUTPUT);
    }
    while(Serial.available()) data = Serial.read();

}

void loop(){
  if(Serial.available()){
    data =Serial.read();
    delay(100);
    while(Serial.available()) Serial.read();
  }

  if(data == 'd'){
    Reading_data();
    FastLED.clear();
    for(i = 0; i < 64; i++){
      if(detect_1[i] == 1){
        leds[i] = CRGB::Green;
      }
    }
    FastLED.show();
  }else if(data ='n'){
    Reading_data();
    for( i = 0; i < 64; i++){
      if( i % 8 == 0 ) Serial.println();
      Serial.print(detect_1[i]);
      Serial.print(" ");
    }
    Serial.println(" No. Of pieces :");
    Serial.print(ones);
    data = 0;
  }else if(data = 'l'){
     for (int i = 0; i <= 63; i++) {
       leds[i] = CRGB ( 0, 0, 255);
       FastLED.show();
       delay(40);
     }
     for (int i = 63; i >= 0; i--) {
       leds[i] = CRGB ( 0, 255, 0);
       FastLED.show();
       delay(40);
     }
     for(int i = 0; i<= 63; i++) {
       leds[i] = CRGB ( 255, 0, 0);
       FastLED.show();
       delay(40);
     }
  
     FastLED.clear();
     //FastLED.show();
     byte r,c,x;
     for ( r = 1; r < 8; r+=2) {
       for ( c = 0, x = r; c <= r; x--, c++) {
         leds[ x*8 + c] = CRGB ::Green ;
       }
     }
    
     for ( r = 2; r < 7; r+=2) {
       for ( c = 7, x = r; c >= r; x++, c--) {
         leds[ x*8 + c] = CRGB :: Green;
       }
     }
     FastLED.show();
     delay(5500);
     FastLED.clear();
     FastLED.show();
     data = 0;
  }
  

}

void Reading_data(){
  byte count_out = 0;                                                             //To control where to write in the detect_1 
  for(int i = ROWS_START_PIN; i <= ROWS_START_PIN; i++){                          //For all the rows
    for (int j = ROWS_START_PIN; i <= ROWS_START_PIN; j++){                       //For making only one row HIGH and the rest are LOW
     if(i == j) digitalWrite(j,HIGH);
      else digitalWrite(j,LOW);
    }
    delayMicroseconds(500);
    //This reads all the columns and the reading belongs to the only row which is HIGH
    //Signal interference is prevented by putting a diode after every single switch in the matrix    
    for(int j = COLUMNS_START_PIN; i <= COLUMNS_START_PIN; j++){
      detect_1[count_out++] = digitalRead(j);                            
    }
  }
  Count_my_ones();
}


void Count_my_ones(void){
  
  ones = 0;
  for(int i = 0; i <64 ; i++)
    if( detect_1[i] == 1 ) ones++;
    
}
