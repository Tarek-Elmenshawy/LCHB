
/*
 * Function: timerSetup
 * ---------------------------------------------------------
 * Set timer1 on output compare match mode
 * ---------------------------------------------------------
 */
 
void timerSetup(void){
  cli();                              //Clear all interrupts
  TCCR1A = TCCR1B = TCNT1 = 0;
  OCR1A = 62500;                      //Compare match register (16Mhz/256prescaler/1Hz)
  TCCR1B |= (1 << WGM12);             //CTC mode
  TCCR1B |= (1 << CS12);              //256 prescaler
  TIMSK1 |= (1 << OCIE1A);            //Enable compare timer interrupt
  sei();                              //Set all interrupts
}

/*
 * Function: ISR(TIMER1_COMPA_vect)
 * ---------------------------------------------------------
 * this ISR is calling every second to update time 
 * for each player.
 * ---------------------------------------------------------
 */

ISR(TIMER1_COMPA_vect){
  if(turn){
    secs_b--;
    while(mins_b == 0 && secs_b < 0){
      getAction(5, turn);
      scanLights();
    }
    start_time = millis();
    if(secs_b < 0){
      secs_b = 59;
      mins_b--;
    }
  }else{
    secs_w--;
    while(mins_w == 0 && secs_w < 0){
      getAction(5, turn);
      scanLights();
    }
    start_time = millis();
    if(secs_w < 0){
      secs_w = 59;
      mins_w--;
    }
  }
}

/*
 * Function: addTime
 * ---------------------------------------------------------
 * adding some time to the player who played his turn for
 * adding some enthusiam to the game.
 * 
 * _turn:    Turn of player (0 = white, 1 = black)
 * ---------------------------------------------------------
 */

void addTime(bool _turn){
  if(_turn){
    secs_b += added_secs;
    if(secs_b >= 60){
      secs_b -= 60;
      mins_b++;
    }
  }else{
    secs_w += added_secs;
    if(secs_w >= 60){
      secs_w -= 60;
      mins_w++;
    }
  }
}

/*
 * Function: showTimer
 * ---------------------------------------------------------
 * Show the rest time for each player
 * ---------------------------------------------------------
 */

void showTimer(void){
  lcdwhite.setCursor(0,0);
  lcdwhite.print("      Time      ");
  lcdwhite.setCursor(0,1);
  lcdwhite.print("      ");
  if(mins_w < 10) lcdwhite.print("0"); 
  lcdwhite.print(mins_w);
  lcdwhite.print(":");
  if(secs_w < 10) lcdwhite.print("0"); 
  lcdwhite.print(secs_w);
  lcdwhite.print("      ");
          
  lcdblack.setCursor(0,0);
  lcdblack.print("      Time      ");
  lcdblack.setCursor(0,1);
  lcdblack.setCursor(0,1);
  lcdblack.print("      ");
  if(mins_b < 10) lcdblack.print("0");
  lcdblack.print(mins_b);
  lcdblack.print(":");
  if(secs_b < 10) lcdblack.print("0");
  lcdblack.print(secs_b);
  lcdblack.print("      ");
}
