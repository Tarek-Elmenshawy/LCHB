/*********************************************************************
****************       Detection  control       **********************
*********************************************************************/

/*
 * Function: scanBoard
 * ---------------------------------------------------------
 * Scannig each spot on chessboard by making one row HIGH, 
 * the others LOW and storing readings from each column.      
 * Then, by comparing the new readings and the last readings,
 * we can detect any change happened. This fuction ignores
 * changes more than one and return 1 if a change happened or
 * 0 if no change happened
 * ---------------------------------------------------------
 */ 
 
bool scanBoard(void){
  Re:
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
  countBoardPieces();
  byte changes = 0;
  for (int i = 0 ; i < 64; i++){
    if (detect_1[i] != detect_0[i]){
      changes++;
      index64 = i;
      index88 = i + (i & ~7);
    }
  }
  if(changes > 1){
  #ifdef DEBUG
    Serial.println(" Error! changes are more than 1");
  #endif
    goto Re;
  }else if(changes == 0 ){
    return 0;
  }else{
    swap0And1();
    return 1;
  }
}

bool checkBoardPieces(void){
  
  byte count_out = 0;                                                             //to control where to write in the detect_1 
  for(int i = ROWS_START_PIN; i <= ROWS_END_PIN; i++){                                                   //for all the rows
    for (int j = ROWS_START_PIN; j <= ROWS_END_PIN; j++){                                              //for making only one row HIGH and the rest are LOW
      if(i == j) digitalWrite(j,HIGH);
      else digitalWrite(j,LOW);
    }
    //delayMicroseconds(500);
    //This reads all the columns and the reading belongs to the only row which is HIGH
    //signal interference is prevented by putting a diode after every single switch in the matrix    
    for(int j = COLUMNS_START_PIN; i <= COLUMNS_END_PIN; j++){
      detect_1[count_out++] = digitalRead(j);                            
    }
   
  }
  memset(led_piece, 0, 64 * sizeof(bool));
  int changes = 0;
  for (int i = 0 ; i < 64; i++){
    if (detect_1[i] != detect_0[i]){
      led_piece[i] = 1;
      leds[i] = CRGB::White;
      FastLED.show();
      changes++;
    }
    
  }
  if(changes >= 1){
  #ifdef DEBUG
    Serial.println("Please put all pieces in its right position to start the game");
  #endif
    return 1;
  }else return 0;
}

void countBoardPieces(void){
  ones = 0;
  for(int i = 0; i <64 ; i++){
    if( detect_1[i] == 1 ) { ones++; }
  }
}

void swap0And1(void){
  for(int i = 0 ; i < 64 ;i++){
    detect_0[i] = detect_1[i];
  }
}
