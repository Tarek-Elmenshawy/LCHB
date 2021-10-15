/*********************************************************************
*******************       LCDs  control       ************************
*********************************************************************/

/*
 * Function: configLCD
 * ---------------------------------------------------------
 * Configuring two LCDs and printing a welcome message.
 * ---------------------------------------------------------
 */ 

void configLCDs(void){
  //Lcd configuration
  lcdwhite.begin(16,2);
  lcdblack.begin(16,2);
  lcdwhite.print(" MOMENTUM TEAM  ");
  lcdblack.print(" MOMENTUM TEAM  ");
  delay(2000);
  lcdwhite.setCursor(0,1);
  lcdwhite.print(" Hello, White!  ");
  lcdblack.setCursor(0,1);
  lcdblack.print(" Hello, Black!  ");
}


/*
 * Function: getAction
 *---------------------------------------------------------
 * Get the action which belongs to the current state.
 * 
 * act:       integer defining the state
 * color:     turn color, 0 = white, 1 = black.
 * ---------------------------------------------------------
 */

void getAction(int act, int color){
  
  if (act == 0){
    //Promotion
    if (color == WHITE){
          
      lcdwhite.setCursor(0,0);
      lcdwhite.print("  Promote pawn  ");
      lcdwhite.setCursor(0,1);
      lcdwhite.print("    to Queen!   ");
          
      lcdblack.setCursor(0,0);
      lcdblack.print("  Your ennemey  ");
      lcdblack.setCursor(0,1);
      lcdblack.print(" has promotion! ");
      Buzzer();
          
  }else{
    lcdblack.setCursor(0,0);
    lcdblack.print("  Promote pawn  ");
    lcdblack.setCursor(0,1);
    lcdblack.print("    to Queen!   ");
          
    lcdwhite.setCursor(0,0);
    lcdwhite.print("  Your ennemey  ");
    lcdwhite.setCursor(0,1);
    lcdwhite.print(" has promotion! ");
    Buzzer();
          
   }
  }else if (act == 1){                                                 //check
    if (color == WHITE){
      lcdwhite.setCursor(0,0);
      lcdwhite.print("                ");
      lcdwhite.setCursor(0,1);
      lcdwhite.print("     Check!     ");
      Buzzer();
    }else{
      lcdblack.setCursor(0,0);
      lcdblack.print("                ");
      lcdblack.setCursor(0,1);
      lcdblack.print("     Check!     ");
      Buzzer();
     }
   }else if (act == 2){
     if (color == WHITE){
      lcdwhite.setCursor(0,0);
      lcdwhite.print("                ");
      lcdwhite.setCursor(0,1);
      lcdwhite.print("  WRONG TURN !  ");
      
      lcdblack.setCursor(0,0);
      lcdblack.print("                ");
      lcdblack.setCursor(0,1);
      lcdblack.print("ALERT! YOUR TURN");
      Buzzer();
     }else{
       lcdwhite.setCursor(0,0);
       lcdwhite.print("                ");
       lcdwhite.setCursor(0,1);
       lcdwhite.print("ALERT! YOUR TURN");
       
       lcdblack.setCursor(0,0);
       lcdblack.print("                ");
       lcdblack.setCursor(0,1);
       lcdblack.print(" WRONG   TURN ! ");
       Buzzer();
      }
    }else if(act == 3){
      if (color == WHITE){
        lcdwhite.setCursor(0,0);
        lcdwhite.print("                ");
        lcdwhite.setCursor(0,1);
        lcdwhite.print(" ALERT! ILLEGAL ");
        
        lcdblack.setCursor(0,0);
        lcdblack.print("                ");
        lcdblack.setCursor(0,1);
        lcdblack.print(" ALERT! ILLEGAL ");
        Buzzer();
      }else{
        lcdwhite.setCursor(0,0);
        lcdwhite.print("                ");
        lcdwhite.setCursor(0,1);
        lcdwhite.print(" ALERT! ILLEGAL ");

        lcdblack.setCursor(0,0);
        lcdblack.print("                ");
        lcdblack.setCursor(0,1);
        lcdblack.print(" ALERT! ILLEGAL ");
        Buzzer();
       }
    }else if(act == 4){
      if (color == WHITE){
        lcdwhite.setCursor(0,0);
        lcdwhite.print("  GAME   OVER!  ");
        lcdwhite.setCursor(0,1);
        lcdwhite.print("   THE LOSER!   ");
        
        lcdblack.setCursor(0,0);
        lcdblack.print("  GAME   OVER!  ");
        lcdblack.setCursor(0,1);
        lcdblack.print("   THE WINNER!  ");
        lcdblack.setCursor(0,1);
        Buzzer();
      }else{
        lcdblack.setCursor(0,0);
        lcdblack.print("  GAME   OVER!  ");
        lcdblack.setCursor(0,1);
        lcdblack.print("   THE LOSER!   ");
        
        lcdwhite.setCursor(0,0);
        lcdwhite.print("  GAME   OVER!  ");
        lcdwhite.setCursor(0,1);
        lcdwhite.print("   THE WINNER!  ");
        lcdwhite.setCursor(0,1);
        Buzzer();
       }
    }else if(act == 5){
      if (color == WHITE){
        lcdwhite.setCursor(0,0);
        lcdwhite.print("   TIME  OUT!   ");
        lcdwhite.setCursor(0,1);
        lcdwhite.print("   THE LOSER!   ");
        
        lcdblack.setCursor(0,0);
        lcdblack.print("   TIME   OUT!  ");
        lcdblack.setCursor(0,1);
        lcdblack.print("   THE WINNER!  ");
        lcdblack.setCursor(0,1);
        Buzzer();
      }else{
        lcdblack.setCursor(0,0);
        lcdblack.print("   TIME  OUT!   ");
        lcdblack.setCursor(0,1);
        lcdblack.print("   THE LOSER!   ");
        
        lcdwhite.setCursor(0,0);
        lcdwhite.print("   TIME   OUT!  ");
        lcdwhite.setCursor(0,1);
        lcdwhite.print("   THE WINNER!  ");
        lcdwhite.setCursor(0,1);
        Buzzer();
       }
    }
}

/*
 * Function: Buzzer
 * ---------------------------------------------------------
 * Make some tone
 * ---------------------------------------------------------
 */

void Buzzer(void){
  for(int i = 0; i < 2; i++){
    digitalWrite(BYZZER_R,HIGH);
    digitalWrite(BYZZER_L,HIGH);
    delay(50);
    digitalWrite(BYZZER_R,LOW);
    digitalWrite(BYZZER_L,LOW);
    delay(50);
  }   
}
