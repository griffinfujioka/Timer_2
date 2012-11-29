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
  int prevRow; 
  int prevColumn; 
  int i; 

  printf(""); 
  prevRow = row; 
  prevColumn = column; 
  tick++;     /* Increment tick on each system tick  */ 
  tick %= 60; /* Convert from clock tick to seconds */ 

  out_byte(0x20, 0x20); /* Re-enable 8259 so PROC switching may occur */ 

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

    row = 23; 
    column = 80 - 10;  // Subtract 10 so 8 bytes (= 8 chars) can be printed + 2 buffer spots


    // Print time 
    printTime(); 

    // Return cursor to the way it was. 
    //row = prevRow; 
    column = prevColumn; 
    
    
    pos = 2*(row*80 + column);        
    offset = (org + pos) & vid_mask;  
    set_6845(CURSOR, offset >> 1);
    
    // If in Umode, decrement running->time 
    //printf("inkmode = %d\n", inkmode); 
    if(1 == inkmode)
    {
      
      running->time--; 

      // If running proc is out of time, switch PROCs
      if(running->time <= 0)
      {
        running->time = 5; 
        tswitch(); 
      }
    }


    // TODO: Finish this component. 
    // Decrement time for all sleeping PROCs, wake them up if time == 0
    for(i=0; i<NPROC; i++)
    {
      //printf("Looking at P%d\n", i); 
      if(proc[i].status == SLEEP)
      {
        printf("P%d: is asleep!\n", i);   // Print PROC number 
        printf("Time remaining: %d\n", proc[i].time); 
        proc[i].time--; 
        if(proc[i].time <= 0)
        {
          proc[i].time = 5; 
          wakeup(&proc[i].time); 
        }
      }
    }


    

  }

  // TODO: Toggle FD every 5 seconds 
  //if(tick  % 120 == 0)    /* 60 ticks/second * 5 seconds = 300 ticks */ 
  //{
    // ... what is FD? Floppy Disk? 
  //}


  

}





