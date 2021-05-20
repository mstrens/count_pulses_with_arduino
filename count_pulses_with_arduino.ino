/*
  Run on a arduino Uno, nano or pro mini (with avr328p)
  Count pulses on pin 5 via Timer-1; other timers are disabled.  
*/
unsigned long countInactive = 0;
unsigned long countOverflow ;
uint16_t prevCounter1 ;
void setup() {
  Serial.begin(115200);
  Serial.println("Start counting automatically") ; 
  Serial.println("Display a '.' once every 65536 counts to show it is still working") ;
  Serial.println("Stop counting when no pulse for about 5 sec") ;
  Serial.println("Then automatically restart counting");
  //pinMode(2, INPUT); digitalWrite(2, HIGH); // trigger on pin-2 = PD2 = interrupt-0
  pinMode(5, INPUT); digitalWrite(5, HIGH); // pulses on pin-5 = PD5 = T1

  TCCR0A=0; // setup timer-0
  TCCR0B=0;
  TIMSK0=0;
  
  TCCR2A=0; // setup timer-2
  TCCR2B=0;
  TIMSK2=0;
  
  TCCR1A=0; // setup timer-1
  TCCR1C=0;
  TIMSK1=0;
  GTCCR=0;
  TCCR1B=0; // stop
  TCNT1=0; // reset timer 1
  countOverflow = 0 ;
  TIFR1 = (1<<TOV1);   //reset overflow
  TCCR1B=7; // start counting pulse on pin 5 with timer1
  
}

void loop() {
  uint16_t counts;
  while (1) {
    if (TIFR1 & (1<<TOV1) ) { //when counter 1 overflow
       TIFR1 = (1<<TOV1);   //reset overflow
       countOverflow++;
       Serial.print("."); // print a . each overflow = 65536 counting
    }
    counts = TCNT1 ;
    if ( prevCounter1 == counts){
      countInactive++;  
    } else {
      prevCounter1 = counts ;
      countInactive = 0;
    }
    if (countInactive > 2000000) {
      countInactive = 0;
      TCCR1B=0; // stop
      counts = TCNT1;
          //attachInterrupt(0, counting_start, RISING); // ready to start
      Serial.println(" ");
      Serial.print("Count = ") ;Serial.println(countOverflow * 256 * 256  + counts) ;
      countOverflow = 0 ;  // reset counter
      TCNT1=0;
      TCCR1B=7; // start counter
    }
  }
}


