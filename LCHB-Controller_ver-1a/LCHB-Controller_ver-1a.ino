/*
 * Terms of use
 * ---------------------------------------------------------------------------------------------------
 * The SOFTWARE is provided "as it", without warranty of any kind, express or implied, including but 
 * not limited to the warranties of merchantability, fitness for a particular purpose and noninfringement.
 * 
 * You may use this work for noncommercial purposes. We hope if you use any parts of the Project, 
 * mentioned the source(LCHB Project By Momment Team) in order not to waste this effort.
 * For more details about the uesd algorithim, check "Algorithim_guide.pdf". 
 * 
 * This is the SOFTWARE of Learning Chessboard(LCHB) Project.
 * Written By: MOMENTUM SOFWARE BRANCH
 * Facebook Page Link: https://www.facebook.com/MomentumTeamMU/
 * ---------------------------------------------------------------------------------------------------
 */

#include "chess_constants.h"
#include "define_pins.h"
#include <FastLED.h>
#include <LiquidCrystal.h>

CRGB leds[NUM_LEDS];
LiquidCrystal lcdwhite (23, 25, 27, 29, 31, 33);
LiquidCrystal lcdblack (8, 9, 10, 11, 12, 13);


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//Declaring global variables
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool turn, check;
bool detect_1[64], detect_0[64];
bool led_matrix[64] = {0}, led_piece[64] = {0};
int killed_pieces = 0, index64 = -1, index88 = -1, king_index = -1;
int place0 = -1, place1 = -1, passer_pawn = -1, en_passant_index = -1;
int ones, mins_w = 5, secs_w = 0, mins_b = 5, secs_b = 0, added_secs = 5;
unsigned long start_time;

//Board representation in 0x88 
byte board[128] = {0};
byte move_buffer[128] = {0};
byte temp_move_buffer[128] = {0};

//Buffers for undoing moves 
byte captured_buffer = NO_PIECE;
int  from_buffer    = -1;
int  to_buffer      = -1;

//Flags for castling 
bool B_Q_ROOK_FLAG = 0, B_K_ROOK_FLAG = 0, W_Q_ROOK_FLAG = 0, W_K_ROOK_FLAG = 0;
bool B_KING_FLAG = 0, W_KING_FLAG = 0;
bool CASTLING_FLAG = 0;
int EN_PASSANT_FLAG = 1;

//void(*resetFunc)(void) = 0;                        //Declare reset function at address 0
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//Setup routine
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup(){
#ifdef DEBUG
  Serial.begin(115200);                              //For debugging
#endif
  initBoard();
  configPins();
  configLCDs();
  
  do
  {
    scanLights();
  }
  while(checkBoardPieces());
  
  lightLedsOff(turn);
  timerSetup();
  start_time = millis();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//Main loop
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop(){
  
  MAIN:
  showTimer();
  int _index0_64 = -1, _index1_64 = -1 ;
  check = isAttacked(getKingIndex(turn), turn^1);
  if (check){
    leds[king_index] = CRGB::Red;
    FastLED.show();
    getAction(1, turn);
    Buzzer();
    while(isKingDead(turn)){
      //GAME OVER
      getAction(4, turn);
      scanLights();
      //resetFunc(); 
    }
  }
  while(! scanBoard() || detect_1[index64] != 0) showTimer();
  //Check if the turn is right
  byte turn_index = index64;
  if(turn == ! bool(board[index88] >> 1 &1)){
    getAction(2, board[index88] >> 1 &1);
    while(! scanBoard() || detect_1[turn_index] == 0){
      leds[turn_index] = CRGB :: Red;
      FastLED.show();
    }
    lightLedsOff(turn);
    showTimer();
    goto MAIN;
  }
  if(check){
    leds[king_index] = CRGB::Red;
    getAction(1, turn);
    FastLED.show();
    Buzzer();
  }
  if(detect_1[index64] == 0 && ones == (32 - killed_pieces -1) && turn == bool(board[index88] >> 1 &1)){
        
    place0 = index88;
    _index0_64 = index64;
    //A pice is picked up
    //we should light leds on 
    #ifdef DEBUG
      Serial.print("A pice is picked up from place0 : ");
      Serial.println(place0);
    #endif
    
    Rescan:
    showTimer();
    legalMoves(place0, move_buffer);
    moveBufferToLed();
    lightLedsOn();

    
    while(! scanBoard()) showTimer(); 
    if(detect_1[index64] == 1 && ones == (32 - killed_pieces)){
      //A piece moved from place0 to place1
      //light leds off
      showTimer();
      lightLedsOff(turn);
      place1 = index88;
      _index1_64 = index64;
      if(!isLegitMove(place0, place1)){
        leds[_index1_64] = CRGB::Red;
        FastLED.show();
      #ifdef DEBUG
        Serial.print("unlegal move");
      #endif
        getAction(3, turn);
        while(! scanBoard() || detect_1[_index1_64] != 0){
          getAction(3, turn);
        }
        showTimer();
        lightLedsOff(turn);
        goto Rescan;
      }
      showTimer();
      makeMove(place0, place1);
      checkCastling(_index0_64, _index1_64);
      checkEnPassant(_index0_64, _index1_64);
   #ifdef DEBUG
      Serial.println(" A pice moved from place0 to place1");
      Serial.print("place1 : ");
      Serial.println(place1);
   #endif
      
      if(place1 != place0){
        addTime(turn);
        showTimer();
        turn ^= 1;
        checkPiecesFlag(place0, place1);
        if(board[place1] >> 2 == PAWN && abs(place1 - place0) == 32){
          passer_pawn = place1;
          EN_PASSANT_FLAG = 0;
        }
      }
      place0 = place1 = -1;
      lightLedsOff(turn);
   
            
    }
    else if(detect_1[index64] == 0 && ones ==(32 - killed_pieces - 2) && turn == ! bool(board[index88] >> 1 & 1)){
      //another piece is picked up  
      place1 = index88;
      int place1_64 = index64;
      //Serial.print("another enemy piece is picked up from place1 : ");
      //Serial.println(place1);
      int _index = index64;
      while(! scanBoard() || detect_1[_index] == 0) showTimer(); 
      if(detect_1[_index] == 1 && ones ==(32 - killed_pieces - 1) && turn == ! bool(board[index88] >> 1 & 1)){
        //a piece that was in place1 was eaten
      #ifdef DEBUG
        Serial.println(" A pice moved from place0 to eat piece in place1");
        Serial.print("place1 : ");
        Serial.println(place1);
      #endif
        showTimer();
        lightLedsOff(turn);
        if(!isLegitMove(place0, place1)){
          leds[_index] = CRGB::Red;
          FastLED.show();
          getAction(3, turn);
          while(! scanBoard() || detect_1[_index] != 0){
            getAction(3, turn);
          }
          lightLedsOff(turn);
          while(! scanBoard() || detect_1[place1] != 1){
            leds[place1_64] = CRGB::Red;
            FastLED.show();
          }
          lightLedsOff(turn);
        }
        makeMove(place0, place1);
        place0 = place1 = -1;
        killed_pieces++;
        addTime(turn);
        showTimer();
        turn ^= 1;
        lightLedsOff(turn);
      }
    } 
  }
  else{
    //We will disregard the last move 
    for(int i = 0 ; i< 64;i++){
       detect_1[i] = detect_0[i];
    }
  #ifdef DEBUG
    Serial.println("Error! A piece has been added");
    #endif
  }
  
}
