//Pins definition

#define LED_PIN            54
#define NUM_LEDS           64
#define BYZZER_R           2
#define BYZZER_L           55
#define ROWS_START_PIN     14
#define ROWS_END_PIN       21
#define COLUMNS_START_PIN  62
#define COLUMNS_END_PIN    69

/*
 * Function: configPins
 * ---------------------------------------------------------
 * Configure the uesd pin as outputs.
 * ---------------------------------------------------------
 */ 
 
void configPins(void){
  for(int i = ROWS_START_PIN; i <= ROWS_END_PIN; i++){
    pinMode(i,OUTPUT); 
  }
  pinMode(BYZZER_R,OUTPUT);
  pinMode(BYZZER_L,OUTPUT);
}
