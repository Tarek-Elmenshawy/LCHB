/*********************************************************************
****************          LED Control         ************************
*********************************************************************/

/*
 * Function: configLEDs
 * ---------------------------------------------------------
 * Configuring number of LEDs and set thier brightness.
 * ---------------------------------------------------------
 */ 

void configLEDs(void){
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
}

/*
 * Function: moveBufferToLed
 * ---------------------------------------------------------
 * Moving indexes wanted to light on from 0x88 coordinates to 
 * LEDs coordinates.
 * ---------------------------------------------------------
 */ 

void moveBufferToLed(void){
   memset(led_matrix,0, 64 * sizeof(bool));
  int index;
  for (int i = 0; i < 64; i++){
    index = i + (i & ~7);
    if (move_buffer[index]) led_matrix[i] = 1; 
  }  
}

/*
 * Function: lightLedsOff
 * ---------------------------------------------------------
 * This function is responsible to light off the allowable places
 * and light on the chessboard according to the player's turn.
 * ---------------------------------------------------------
 */ 

void lightLedsOff(bool turn){
  FastLED.clear();

  if(turn){
    int r,c,x;
    for(r = 1; r < 8; r+=2){
      for(c = 0, x = r; c <= r; x--, c++){
        leds[ x*8 + c] = CRGB :: Green;
      }
    }

    for(r = 2; r < 7; r+=2){
      for(c = 7, x = r; c >= r; x++, c--){
        leds[ x*8 + c] = CRGB :: Green;
      }
    }
    FastLED.show();
  }else{
    int r,c,x;
    for(r = 1; r < 8; r+=2){
      for( c = 0, x = r; c <= r; x--, c++){
        leds[ x*8 + c] = CRGB :: HotPink;
      }
    }
    for(r = 2; r < 7; r+=2){
      for(c = 7, x = r; c >= r; x++, c--){
        leds[ x*8 + c] = CRGB :: HotPink;
      }
    }
    FastLED.show();
  }
}

/*
 * Function: lightLedsOn
 * ---------------------------------------------------------
 * This function is responsible to light on the allowable places.
 * ---------------------------------------------------------
 */ 

void lightLedsOn(void){
  int index;
  for(int i = 0; i < 64; i++){
    index = i + ( i& ~7);
    if(led_matrix[i]){
      if (bool(board[index] >> 1 & 1) == !turn  && board[index] != NO_PIECE){
        leds[i] = CRGB::Red;  
      }else{
        leds[i] = CRGB::Blue;
      }
    }   
  }
  FastLED.show();
}

/*
 * Function: scanLights
 * ---------------------------------------------------------
 * Making show during starting game and when one of the two
 * players win.
 * ---------------------------------------------------------
 */ 

void scanLights(void){
  for(int i = 0; i < 1; i++){
    for (int i = 0; i <= 63; i++){
      if(led_piece[i]) continue;
      leds[i] = CRGB ( 0, 0, 255);
      FastLED.show();
      delay(3);
    }
    for(int i = 63; i >= 0; i--){
      if(led_piece[i]) continue;
      leds[i] = CRGB ( 0, 255, 0);
      FastLED.show();
      delay(3);
    }
    for(int i = 0; i<= 63; i++){
      if(led_piece[i]) continue;
      leds[i] = CRGB ( 255, 0, 0);
      FastLED.show();
      delay(3);
    }
  }
}
