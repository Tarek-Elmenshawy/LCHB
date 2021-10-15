/*********************************************************************
*************     IMPLEMENTATION OF CHESS LOGIC     ******************
*********************************************************************/

/*
 * Function: initBoard
 * ---------------------------------------------------------
 * Creates an initial placement of the pieces
 * in the global board array, regarding chess rules.
 * ---------------------------------------------------------
 */ 
 
void initBoard(void){
  for(int i = 0; i < 128; i++) board[i] = NO_PIECE;
  //Set up white pieces
  board[0] = B_ROOK;
  board[1] = B_KNIGHT;
  board[2] = B_BISHOP; 
  board[3] = B_QUEEN;
  board[4] = B_KING;
  board[5] = B_BISHOP;
  board[6] = B_KNIGHT;
  board[7] = B_ROOK;
  for(int i = 0; i < 8; i++) board[16+i] = B_PAWN;   //16 start of second row in 0x88
  //Set up black pieces
  board[112] = W_ROOK;
  board[113] = W_KNIGHT;
  board[114] = W_BISHOP; 
  board[115] = W_QUEEN;
  board[116] = W_KING;
  board[117] = W_BISHOP;
  board[118] = W_KNIGHT;
  board[119] = W_ROOK;
  for(int i = 0; i < 8; i++) board[96+i] = W_PAWN;   //96 start of seventh row in 0x88
  for(int i = 0; i < 16; i++) detect_0[i] = 1;
  for(int i = 48; i < 64; i++) detect_0[i] = 1;
}

/*
 * Function: makeMove
 * ---------------------------------------------------------
 * Moves the piece from one field to another. Take 
 * to be sure that the pieces exists, the function 
 * does not check that. Stores essential information
 * (captured, move itself, etc...) in global buffers.
 *
 * from:  0x88 coordinates in the global board array 
 *        defining the figure that has to be moved.
 * to:    0x88 coordinates  in the global board array 
 *        defining where the figure in from is moved to.
 * ---------------------------------------------------------
 */ 
 
void makeMove(int from, int to){
  from_buffer = from;
  to_buffer = to;
  byte piece_from = board[from];
  byte piece_to   = board[to];
  board[from] = NO_PIECE;
  board[to]   = piece_from;
  captured_buffer = piece_to;
  if(captured_buffer >> 2 == ROOK){
    if(captured_buffer & B00010){
      if(to == 0) B_Q_ROOK_FLAG = 1;
      else if(to == 7) B_K_ROOK_FLAG = 1;
    }else{
      if(to == 112) W_Q_ROOK_FLAG = 1;
      else if(to == 119) W_K_ROOK_FLAG = 1;
    }
  }
}

/*
 * Function: unmakeMove
 * ---------------------------------------------------------
 * Makes the former move undone in the global board
 * array.
 * The function does not ensure if there is a move in
 * the buffers, undefined behavior if not!
 * ---------------------------------------------------------
 */
 
void unmakeMove(){
  byte piece = board[to_buffer];
  board[to_buffer] = captured_buffer;
  board[from_buffer] = piece;
  captured_buffer = NO_PIECE;
  from_buffer = -1;
  to_buffer   = -1;
}

/*
 * Function: getKingIndex
 * ---------------------------------------------------------
 * Finds the king of the given color and returns its
 * index in the global board.
 *
 * color:   Color of the searched king 
 *          0 = white, 1 = black
 * 
 * returns: 0x88 coordinate of the king on the board.
 *          -1 if no king present.
 * ---------------------------------------------------------
 */

int getKingIndex(int color){
  byte king = color ? B_KING : W_KING;
  int index;
  for(int i = 0; i < 64; i++){
    index = i + (i & ~7); //coordinate transformation to 0x88
    king_index = i;
    if(board[index] == king) return index;  
  }
  return -1;
}

/*
 * Function: isLegalMoveRelativeToKing
 * ---------------------------------------------------------
 * Returns if a move is legit. Therefore, it makes a 
 * move on the board, checks if legit (king in check?)
 * and unmakes the move.
 *
 * from:     0x88 coordinates in the global board array 
 *           defining the figure that has to be moved.
 * to:       0x88 coordinates  in the global board array 
 *           defining where the figure in from is moved to.
 * 
 * returns:  Return 1 if the move is legit, 0 if not
 * ---------------------------------------------------------
 */

int isLegalMoveRelativeToKing(int from, int to){
  int color = board[from] >> 1 & 1;
  int otherColor = color ? 0 : 1;
  makeMove(from, to);
  int index = getKingIndex(color);
  int result = !isAttacked(index, otherColor);
  unmakeMove();
  return result;
}

/*
 * Function: isAllowedToKing
 * ---------------------------------------------------------
 * Returns if a move is legit. Therefore, it makes a 
 * move on the board, checks if legit (king in check?)
 * and unmakes the move.
 *
 * from:     0x88 coordinates in the global board array 
 *           defining the figure that has to be moved.
 * to:       0x88 coordinates  in the global board array 
 *           defining where the figure in from is moved to.
 * 
 * returns:  Return 1 if the move is legit, 0 if not
 * ---------------------------------------------------------
 */

int isAllowedToKing(int from, int to){
  int color = board[from] >> 1 & 1;
  int otherColor = color ? 0 : 1;
  makeMove(from, to);
  int result = !isAttacked(to, otherColor);
  unmakeMove();
  return result;
}

/*
 * Function: isAttacked
 * ---------------------------------------------------------
 * Checks if a given square is attacked by a 
 * specified color. This method uses a precalculated
 * attack array, defining which pieces can 
 * attack the unique 0x88 square to square difference.
 * The lookup avoids generation of unnecessary pseudo-
 * legal moves. Operates on the global board array.
 *
 * index:     0x88 coordinates of the square
 * byColor:   Color of the attacker.
 *            0 = white, 1 = black
 *
 * returns:   int defining if it is attackable or not.
 *            1 = attackable, 0 = not attackable.
 * ---------------------------------------------------------
 */

int isAttacked(int index, int byColor){
  int _index, diff, res;
  for(int i = 0; i < 64; i++){
    _index = i + (i & ~7);   //coordinate transformation to 0x88
    byte piece = board[_index];
    byte piece_type = piece >> 2;
    if((piece >> 1 & 1) == byColor){
      int diff = index - _index + 119;   
      switch( attack_array[diff] ){
        case ATTACK_KQR:
          if(piece_type == KING || piece_type == QUEEN || piece_type == ROOK)
            res = canReach(_index, index);
          break;
        case ATTACK_QR:
          if(piece_type == QUEEN || piece_type == ROOK)
            res = canReach(_index, index);
          break;
        case ATTACK_KQBwP:
          if(piece_type == KING || piece_type == QUEEN || piece_type == BISHOP || piece == W_PAWN)
            res = canReach(_index, index);
          break;
        case ATTACK_KQBbP:
          if(piece_type == KING || piece_type == QUEEN || piece_type == BISHOP || piece == B_PAWN)
            res = canReach(_index, index);
          break;
        case ATTACK_QB:
          if(piece_type == BISHOP || piece_type == QUEEN) 
            res = canReach(_index, index);
          break;
        case ATTACK_N:
          if(piece_type == KNIGHT)
            res = canReach(_index, index);
          break;
        default:
          res = 0;
      }
      if(res == 1) return 1;
    }
  }
  return 0;
}

/*
 * Function: canReach
 * ---------------------------------------------------------
 * Determine if a piece on a specific square can 
 * reach a target square in a pseudo-legal manner.
 * The function calls the generation of moves with
 * a temporal buffer.
 *
 * index:     0x88 coordinates of the piece
 * target:    0x88 coordinates of the target square
 *
 * returns:   int defining if the given piece can 
 *            reach the target or not.
 *            1 = reachable, 0 = not reachable.
 * ---------------------------------------------------------
 */

int canReach(int index, int target){
  pseudoLegalMoves(index, temp_move_buffer);
  return temp_move_buffer[target]; 
}

/*
 * Function: pseudoLegalMoves
 * ---------------------------------------------------------
 * Generates the pseudo-legal moves of the piece at
 * index. Operates on the global board array and writes
 * the results in a buffer. Calls specific subroutines 
 * for PAWNS, KINGS and the rest.
 *
 * index:     0x88 coordinates of the piece
 * buffer:    Buffer defining the movements. Each 
 *            field in the 128 byte array is either
 *            marked 1 for a possible move target or
 *            0 for not reachable.
 * ---------------------------------------------------------
 */

void pseudoLegalMoves(int index, byte * buffer){
  //memset( array, value you want to set, number of bytes);
  memset(buffer,0, 128 * sizeof(byte));
  REACTION:
  byte piece = board[index];
  byte color = board[index] >> 1 & 1;
  byte piece_type = piece >> 2;
  int end_row = color ? 7 : 0;
  if(piece_type == PAWN){
    if(color == 1 && index/16 == end_row){ board[index] = B_QUEEN; getAction(0, color); goto REACTION; }
    if(color == 0 && index/16 == end_row){ board[index] = W_QUEEN; getAction(0, color); goto REACTION; }
    generatePseudoLegalPawnMoves(piece, index, buffer);
  }else if(piece_type == KING) {
    generateLegalKingMoves(piece, index, buffer);
  }else{ //everything but a pawn and king
    generatePseudoLegalMoves(piece, index, buffer);
  }
}

/*
 * Function: generatePseudoLegalMoves
 * ---------------------------------------------------------
 * Generates the pseudo-legal moves of the piece at
 * index. Specialized routine for QUEEN, ROOK, KNIGHT,
 * BISHOP. Operates on global board array.
 *
 * piece:     byte describing the piece. Important 
 *            for color and sliding attribute. Only
 *            Q,R,K,B are allowed, undefined behavior 
 *            else.
 * index:     0x88 coordinates of the piece
 * buffer:    Buffer defining the movements. Each 
 *            field in the 128 byte array is either
 *            marked 1 for a possible move target or
 *            0 for not reachable.
 * ---------------------------------------------------------
 */

void generatePseudoLegalMoves(byte piece, int index, byte * buffer){
  const int * md = move_deltas[piece >> 2];
  byte sliding = piece & B1;
  byte color   = piece >> 1 & 1;
  int tmp_index = index;

  for(int i = 0; i < 8; i++){
    int delta = md[i];
    if(delta == 0) break;
    tmp_index = index + delta;
    while((tmp_index & 0x88) == 0){   //check! Does the index in real board ? 
      byte target = board[tmp_index];
      if(target != NO_PIECE){
        if((target >> 1 & 1) != color){   buffer[tmp_index] = 1; }
        break;
      }
      buffer[tmp_index] = 1;
      if(!sliding) break;
      tmp_index += delta;
    }
  }    
}

/*
 * Function: generateLegalKingMoves
 * ---------------------------------------------------------
 * Generates the pseudo-legal moves of the piece at
 * index. Specialized routine for QUEEN, ROOK, KNIGHT,
 * BISHOP. Operates on global board array.
 *
 * piece:     byte describing the piece. Important 
 *            for color and sliding attribute. Only
 *            Q,R,K,B are allowed, undefined behavior 
 *            else.
 * index:     0x88 coordinates of the piece
 * buffer:    Buffer defining the movements. Each 
 *            field in the 128 byte array is either
 *            marked 1 for a possible move target or
 *            0 for not reachable.
 * ---------------------------------------------------------
 */

void generateLegalKingMoves(byte piece, int index, byte * buffer){
  const int * md = move_deltas[piece >> 2];
  byte color   = piece >> 1 & 1;
  int castling_king_index = color ? 4 : 116;
  int tmp_index = index;

  for(int i = 0; i < 8; i++){
    int delta = md[i];
    if(delta == 0) break;
    tmp_index = index + delta;
    while((tmp_index & 0x88) == 0){   //check! Does the index in real board ? 
      byte target = board[tmp_index];
      if(target != NO_PIECE){
        if((target >> 1 & 1) != color && isAllowedToKing(index, tmp_index)){   buffer[tmp_index] = 1; }
        break;
      } else {
        if(isAllowedToKing(index, tmp_index)){ buffer[tmp_index] = 1; }
        break;
      }
    }
  }
  if(index == castling_king_index && !check){
    if(! color && ! W_KING_FLAG){
      if(! W_K_ROOK_FLAG){
        shortCastlingMoves(index, buffer);
      }
      if(! W_Q_ROOK_FLAG){
        longCastlingMoves(index, buffer);
      }
    } else if (color && ! B_KING_FLAG){
      if(! B_K_ROOK_FLAG){
        shortCastlingMoves(index, buffer);
      }
      if(! B_Q_ROOK_FLAG){
        longCastlingMoves(index, buffer);
      }
    }
  }
}

void shortCastlingMoves(int index, byte * buffer){
  for(int i= 1; i < 3; i++){
    if(board[index+i] != NO_PIECE){ break; }
    if(! isAllowedToKing(index, index+i)){ break; }
    buffer[index+i] = 1;
  }
  CASTLING_FLAG = buffer[index+2];
}

void longCastlingMoves(int index, byte * buffer){
  for(int i= 1; i < 3; i++){
    if(board[index-i] != NO_PIECE){ break; }
    if(! isAllowedToKing(index, index-i)){ break; }
    buffer[index-i] = 1;
  }
  CASTLING_FLAG = buffer[index-2];
}

/*
 * Function: generatePseudoLegalPawnMoves
 *---------------------------------------------------------
 * Generates the pseudo-legal moves for pawns. 
 * Calls straight and capture subroutines.
 * Operates on global board array.
 *
 * piece:     byte describing the piece. Important 
 *            for color. Every passed piece is handled
 *            as a pawn.
 * index:     0x88 coordinates of the piece
 * buffer:    Buffer defining the movements. Each 
 *            field in the 128 byte array is either
 *            marked 1 for a possible move target or
 *            0 for not reachable.
 * ---------------------------------------------------------
 */

void generatePseudoLegalPawnMoves(byte piece, int index, byte * buffer){
  byte color = piece >> 1 & 1;
  //Get the direction of the pieces!
  int dir = color ? -1 : 1;
  int starting_row = color ? 1 : 6;
  straightPawnMove(dir, index, starting_row, buffer);
  capturePawnMove(dir, index, color, buffer);
  enPassantPawnMove(dir, index, color, buffer);
}


/*
 * Function: straightPawnMove
 * ---------------------------------------------------------
 * Generates pseudo-legal straight pawn moves. 
 * Handles two field move from initial position.
 * Every incomming piece is handled as a pawn.
 * Operates on global board array.
 *
 * dir:       Defining direction, 1 = white (to top)
 *            -1 = black (to bottom).
 * starting_row: defines the row of initial position
 * index:     0x88 coordinate of the pawn
 * buffer:    Buffer defining the movements. Each 
 *            field in the 128 byte array is either
 *            marked 1 for a possible move target or
 *            0 for not reachable.
 * ---------------------------------------------------------
 */

void straightPawnMove(int dir, int index, int starting_row, byte * buffer){
  if(board[index + (dir * NORTH)] == NO_PIECE){
    buffer[index + (dir * NORTH)] = 1;
    if(index / 16 == starting_row  && board[index + (dir * NN)] == NO_PIECE){
          buffer[index + (dir * NN)] = 1;
    }
  }
}

/*
 * Function: capturePawnMove
 * ---------------------------------------------------------
 * Generates pseudo-legal pawn captures. 
 * Every incomming piece is handled as a pawn.
 * Operates on global board array.
 *
 * dir:       Defining direction, 1 = white (to top)
 *            -1 = black (to bottom).
 * index:     0x88 coordinate of the pawn
 * color:     defines the color of the pawn (w=0,b=1)
 * buffer:    Buffer defining the movements. Each 
 *            field in the 128 byte array is either
 *            marked 1 for a possible move target or
 *            0 for not reachable.
 * ---------------------------------------------------------
 */

void capturePawnMove(int dir, int index, int color, byte * buffer){
  int  attack_index = index + (dir * NW);
  byte attack_piece = board[attack_index];
  if(attack_piece != NO_PIECE && (attack_piece >> 1 & 1) != color){
    buffer[attack_index] = 1; 
  }
  attack_index = index + (dir * NE);
  attack_piece = board[attack_index];
  if(attack_piece != NO_PIECE && (attack_piece >> 1 & 1) != color){
    buffer[attack_index] = 1;
  }
}

/*
 * Function: enPassantPawnMove
 * ---------------------------------------------------------
 * Generates pseudo-legal pawn captures. 
 * Every incomming piece is handled as a pawn.
 * Operates on global board array.
 *
 * dir:       Defining direction, 1 = white (to top)
 *            -1 = black (to bottom).
 * index:     0x88 coordinate of the pawn
 * color:     defines the color of the pawn (w=0,b=1)
 * buffer:    Buffer defining the movements. Each 
 *            field in the 128 byte array is either
 *            marked 1 for a possible move target or
 *            0 for not reachable.
 * ---------------------------------------------------------
 */

void enPassantPawnMove(int dir, int index, int color, byte * buffer){
  int  attack_index = index + (dir * NW);
  byte attack_piece = board[attack_index];
  int passant_row = color ? 4 : 3;
  if(EN_PASSANT_FLAG == 0){
    if(attack_piece == NO_PIECE && attack_index == passer_pawn + (dir * NORTH) && index / 16 == passant_row){
      buffer[attack_index] = 1;
      en_passant_index = attack_index;
    }
    attack_index = index + (dir * NE);
    attack_piece = board[attack_index];
    if(attack_piece == NO_PIECE && attack_index == passer_pawn + (dir * NORTH) && index / 16 == passant_row){
      buffer[attack_index] = 1;
      en_passant_index = attack_index;
    }
  }
}

/*
 * Function: filterLegalMoves
 * ---------------------------------------------------------
 * Takes a buffer of psuedo-legal moves and an index 
 * of a piece and checks the moves to be legal. 
 * Illegal moves get deleted from the moves array.
 *
 * index:    0x88 coordinates of the piece
 * buffer:   Buffer of pseudo-legal moves, may
 *           containing check violations etc. Moves
 *           are directly delete from this pointer.
 * ---------------------------------------------------------
 */

void filterLegalMoves(int index, byte * buffer){
  int _index;
  for(int i = 0; i < 64; i++){
    _index = i + (i & ~7);       //coordinate transformation to 0x88
    if(buffer[_index])
        if(!isLegalMoveRelativeToKing(index, _index)) buffer[_index] = 0;
  }
}

/*
 * Function: legalMoves
 * ---------------------------------------------------------
 * Generates the legal moves of the piece at
 * index. Calls pseudolegal and filter subroutines
 * and writes the results in a buffer.
 *
 * index:     0x88 coordinates of the piece
 * buffer:    Buffer defining the movements. Each 
 *            field in the 128 byte array is either
 *            marked 1 for a possible move target or
 *            0 for not reachable.
 * returns:   boolean defining if a legit move is 
 *            available or not.
 *            1 = available, 0 = not legit.
 * ---------------------------------------------------------           
 */

bool legalMoves(int index, byte * buffer){
  byte piece_type = board[index] >> 2;
  pseudoLegalMoves(index, buffer);
  if(piece_type != KING){ filterLegalMoves(index, buffer); }
  for(int i = 0; i < 64; i++){
    int _index = i +(i & ~7);
    if(buffer[_index]){ return 1;}
  }
  return 0;
}

/*
 * Function: isLegitMove
 * ---------------------------------------------------------
 * Check Is the move legal? by return the value of 
 * the index in move_buffer array.
 *
 * from_index: 0x88 coordinates of the piece
 * to_index:   Buffer defining the movements. Each 
 *             field in the 128 byte array is either
 *             marked 1 for a possible move target or
 *             0 for not reachable.
 * returns:    boolean defining if the given move is 
 *             legit or not.
 *             1 = legit, 0 = not legit.
 * ---------------------------------------------------------
 */

bool isLegitMove(int from_index, int to_index){
  if(from_index == to_index){ return 1;}
  return move_buffer[to_index];
}

/*
 * Function: isKingDead
 * ---------------------------------------------------------
 * Check if there is a legit move can save the king by 
 * Generating the legal moves of all pieces at
 * their position. Calls legalmoves subroutine. 
 * and writes the results in a buffer.
 *
 * color:     King color, 0 = white, 1 = black.
 * returns:   boolean defining if the king is dead
 *            or not.
 *            1 = dead, 0 = alive.
 * ---------------------------------------------------------
 */

bool isKingDead(bool color){
  bool res = legalMoves(getKingIndex(color), move_buffer);
  if(res){ return 0;}
  for(int i = 0; i < 64; i++){
    int index = i + (i & ~7);
    if(board[index] != NO_PIECE &&  bool(board[index] >> 1 & 1) == color){
      res = legalMoves(index, move_buffer);
      if(res){ return 0;}
    }
  }
  return 1;
}

/*
 * Function: checkPiecesFlag
 * ---------------------------------------------------------
 * Change the state of flags which needs to change
 *
 * index:     0x88 coordinates of the piece
 * ---------------------------------------------------------
 */

void checkPiecesFlag(int index0, int index1){
  EN_PASSANT_FLAG ++; 
  byte piece = board[index1] >> 2;
  if(piece == KING || piece == ROOK){
    switch( index0 ){
      case 0:
        B_Q_ROOK_FLAG = 1;
        break;
      case 4:
        B_KING_FLAG = 1;
        break;
      case 7:
        B_K_ROOK_FLAG = 1;
        break;
      case 112:
        W_Q_ROOK_FLAG = 1;
        break;
      case 116:
        W_KING_FLAG = 1;
        break;
      case 119:
        W_K_ROOK_FLAG = 1;
        break;
    }
  } 
}

/*
 * Function: checkCastling
 * ---------------------------------------------------------
 * Check If there is castling and make procedures to
 * castling.
 *
 * _index0_64: 64 Coordinates of place0 
 * _index1_64: 64 Coordinates of place1
 * ---------------------------------------------------------
 */

void checkCastling(int _index0_64, int _index1_64){
  
  if(board[place1] >> 2 == KING && place1 == place0 - 2){
    leds[_index0_64-4] = CRGB::Blue;
    FastLED.show();
    lcdwhite.clear();
    lcdblack.clear();
    lcdwhite.write("   CASTLING !   ");
    lcdblack.write("   CASTLING !   ");
    while(! scanBoard() || detect_1[_index0_64-4] == 1){
      leds[_index0_64-4] = CRGB::Blue;
      FastLED.show();
    }
    lightLedsOff(turn);
    leds[_index0_64-1] = CRGB::Blue;
    FastLED.show();
    while(! scanBoard() || detect_1[_index0_64-1] != 1){
      leds[_index0_64-1] = CRGB::Blue;
      FastLED.show();
    }
    makeMove(place0-4, place0-1);
  }else if(board[place1] >> 2 == KING && place1 == place0 + 2){
    leds[_index0_64+3] = CRGB::Blue;
    FastLED.show();
    lcdwhite.clear();
    lcdblack.clear();
    lcdwhite.write("   CASTLING !   ");
    lcdblack.write("   CASTLING !   ");
    while(! scanBoard() || detect_1[_index0_64+3] == 1){
      leds[_index0_64+3] = CRGB::Blue;
      FastLED.show();
    }
    lightLedsOff(turn);
    leds[_index0_64+1] = CRGB::Blue;
    FastLED.show();
    while(! scanBoard() || detect_1[_index0_64+1] != 1){
      leds[_index0_64+1] = CRGB::Blue;
      FastLED.show();
    }
    makeMove(place0+3, place0+1);
  }
}

/*
 * Function: checkEnPassant
 * ---------------------------------------------------------
 * Check If there is en passant and make procedures to
 * en passant.
 *
 * _index0_64: 64 Coordinates of place0 
 * _index1_64: 64 Coordinates of place1
 * ---------------------------------------------------------
 */

void checkEnPassant(int _index0_64, int _index1_64){
  if(EN_PASSANT_FLAG == 0){
    if(board[place1] >> 2 == PAWN && place1 == en_passant_index){
      int dir = turn ? -1 : 1;
      leds[_index1_64+(dir*8)] = CRGB::Blue;
      FastLED.show();
      while(! scanBoard() || detect_1[_index1_64+(dir*8)] == 1){
        leds[_index1_64+(dir*8)] = CRGB::Blue;
        FastLED.show();
        lcdwhite.clear();
        lcdblack.clear();
        lcdwhite.setCursor(0,1);
        lcdblack.setCursor(0,1);
        lcdwhite.write("  EN PASSANT !  ");
        lcdblack.write("  EN PASSANT !  ");
      }
      board[passer_pawn] = NO_PIECE;
      killed_pieces ++;
    }
  }
}
