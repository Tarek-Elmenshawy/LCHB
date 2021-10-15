//Define directions

#define NORTH -16
#define NN     (NORTH + NORTH)
#define SOUTH  16
#define SS     (SOUTH + SOUTH)
#define EAST   1
#define WEST  -1
#define NE    -15
#define SW     15
#define NW    -17
#define SE     17
#define WHITE  0
#define BLACK  1

  
//Constants for pieces, while last bit stands for its color
//ENCODING:    
//             PPCS
//
//P: Piece type, 3 bits
//C: Color, 0 = white, 1 = black
//S: Sliding, 0 none-sliding, 1 sliding

const byte W_PAWN    = B00000;
const byte W_ROOK    = B00101;
const byte W_KNIGHT  = B01000;
const byte W_BISHOP  = B01101;
const byte W_KING    = B10000;
const byte W_QUEEN   = B10101;
const byte B_PAWN    = B00010;
const byte B_ROOK    = B00111;
const byte B_KNIGHT  = B01010;
const byte B_BISHOP  = B01111;
const byte B_KING    = B10010;
const byte B_QUEEN   = B10111;
const byte NO_PIECE  = B11111;

//General piece types:

const byte PAWN   = B000;
const byte ROOK   = B001;
const byte KNIGHT = B010;
const byte BISHOP = B011;
const byte KING   = B100;
const byte QUEEN  = B101;


//Move deltas. Define the index-deltas for different figures
//Index of move vector defined by the peace bit representation

const int move_deltas[6][8] = {
  { 0 },                                          //pawn: Handled differently
  { WEST, SOUTH, EAST,  NORTH, 0, 0, 0, 0},       //rook
  {-18, -33, -31, -14,  18,  33,  31,  14},       //knight very special movements
  {SE, SW, NW, NE, 0, 0, 0, 0},                   //bishop
  {SE, SW, NW, NE, WEST, SOUTH, EAST, NORTH},     //king
  {SE, SW, NW, NE, WEST, SOUTH, EAST, NORTH}      //queen
};


const byte ATTACK_NONE   = 0;                     //none
const byte ATTACK_KQR    = 1;                     //king - queen - rook
const byte ATTACK_QR     = 2;                     //queen - rook
const byte ATTACK_KQBbP  = 3;                     //king - queen - bishop - black pawn
const byte ATTACK_KQBwP  = 4;                     //king - queen - bishop - white pawn
const byte ATTACK_QB     = 5;                     //queen - bishop
const byte ATTACK_N      = 6;                     //kinight

const byte attack_array[239] =
  {5, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0,      //0 - 13
   5, 0, 0, 5, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0,      //14 - 27
   0, 5, 0, 0, 0, 0, 5, 0, 0, 0, 0, 2, 0, 0,      //28 - 41
   0, 0, 5, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 2,      //42 - 55
   0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0,      //56 - 69
   0, 2, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0,      //70 - 83
   0, 5, 6, 2, 6, 5, 0, 0, 0, 0, 0, 0, 0, 0,      //84 - 97
   0, 0, 0, 6, 4, 1, 4, 6, 0, 0, 0, 0, 0, 0,      //98 - 111
   2, 2, 2, 2, 2, 2, 1, 0, 1, 2, 2, 2, 2, 2,      //112 - 125
   2, 0, 0, 0, 0, 0, 0, 6, 3, 1, 3, 6, 0, 0,      //126 - 139
   0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 2, 6, 5,      //140 - 153
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 2,      //154 - 167
   0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0,      //168 - 181
   0, 2, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 5, 0,      //182 - 195
   0, 0, 0, 2, 0, 0, 0, 0, 5, 0, 0, 0, 0, 5,      //196 - 209
   0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 5, 0, 0,      //210 - 223
   5, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 5};  //224 - 238             

   

   
