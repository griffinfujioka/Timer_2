/* timer parameters. */
#define LATCH_COUNT     0x00	/* cc00xxxx, c = channel, x = any */
#define SQUARE_WAVE     0x36	/* ccaammmb, a = access, m = mode, b = BCD */

/************************ NOTICE THE DIVISOR VALUE ***********************/
#define TIMER_FREQ   1193182L	/* timer frequency for timer in PC and AT */
#define TIMER_COUNT ((unsigned) (TIMER_FREQ/60)) /* initial value for counter*/

#define TIMER0       0x40
#define TIMER_MODE   0x43
#define TIMER_IRQ       0

#define true 1
#define false 0


void printTime(); 

int enable_irq(irq_nr) unsigned irq_nr;
{
  lock();
    out_byte(0x21, in_byte(0x21) & ~(1 << irq_nr));

}

/*===========================================================================*
 *				timer_init				     *
 *===========================================================================*/

ushort tick;
int seconds, minutes, hours; 

int timer_init()
{
  /* Initialize channel 0 of the 8253A timer to e.g. 60 Hz. */

  printf("timer init\n");
  tick = 0;
  seconds = minutes = hours = 0; 

  out_byte(TIMER_MODE, SQUARE_WAVE);	/* set timer to run continuously */
  out_byte(TIMER0, TIMER_COUNT);	/* load timer low byte */
  out_byte(TIMER0, TIMER_COUNT >> 8);	/* load timer high byte */
  enable_irq(TIMER_IRQ); 
}

void printTime()
{
  int oneDigitSeconds = false; 
  int oneDigitMinutes = false; 
  int oneDigitHours = false; 

  if(seconds < 10)
    oneDigitSeconds = true; 
  if(minutes < 10)
    oneDigitMinutes = true; 
  if(hours < 10)
    oneDigitHours = true; 

  if(oneDigitSeconds && oneDigitMinutes && oneDigitHours)
  {
    printf("0%d:0%d:0%d", hours, minutes, seconds); 
  }
  else if(!oneDigitSeconds && oneDigitMinutes && oneDigitHours)
    printf("0%d:0%d:%d", hours, minutes, seconds); 
  else if(oneDigitSeconds && !oneDigitMinutes && oneDigitHours)
    printf("0%d:%d:0%d", hours, minutes, seconds); 
  else if(!oneDigitSeconds && !oneDigitMinutes && oneDigitHours)
    printf("0%d:%d:%d", hours, minutes, seconds); 
}

/*===========================================================================*
 *				timer_handler				     *
 *===========================================================================*/

int thandler()
{
  int pos; 
  char output; 
  tick++;     /* Increment tick on each system tick  */ 
  tick %= 60; /* Convert from clock tick to seconds */ 



  if (tick % 60 == 0)
  {
    //printf("1 second timer interrupt\n");


    seconds++; 
    if(seconds % 60 == 0)
    {
      minutes++; 
      seconds = 0; 
    }
    if(minutes != 0 && minutes % 60 == 0)
    {
      hours++; 
    }
    if(hours % 12 > 1)
    {
      hours = hours % 12; 
    }

    // TODO: Display the current time HH:MM:SS in the lower right corner
      // Set cursor position to coordinates in bottom right corner 
          // Use org and offset? 
          // Just insert a newline and offset into the bottom right corner. 
      // Get current system time, format into ASCII and print 
    // Set cursor to write in bottom right corner 
    row = 24; 
    column = 80 - 10;  // Subtract 9 so 8 bytes (= 8 chars) can be printed 


    //printf("00:00:0%d\n", seconds);   // Print time 
    printTime(); 

    // Return cursor to the way it was. 
    row--; 
    pos = 2*(row*80 + column);        
    offset = (org + pos) & vid_mask;  
    set_6845(CURSOR, offset >> 1);
    

  }

  // TODO: Toggle FD every 5 seconds 
  //if(tick  % 120 == 0)    /* 60 ticks/second * 5 seconds = 300 ticks */ 
  //{
    //printf("2 second interrupt - Toggle FD!\n"); 
    // If FD is on, turn it off 
    // Else if FD is off, turn it on

    // ... what is FD? Floppy Disk? 
  //}


    out_byte(0x20, 0x20);  

}





