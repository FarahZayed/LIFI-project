#include "../LIB/std_types.h"
#include "../LIB/tm4c123gh6pm.h"
#include "../MCAL/DIO.h"
#include <stdio.h>
#include "stdint.h"
#include "ultrasonic.h"

uint32_t Measure_distance(void)
{
   int lastEdge, thisEdge;

   /* Given 10us trigger pulse */
   GPIO_PORTB_DATA_R &= ~(1 << 2); /* make trigger  pin high */
   Delay_MicroSecond(10);          /*10 seconds delay */
   GPIO_PORTB_DATA_R |= (1 << 2);  /* make trigger  pin high */
   Delay_MicroSecond(10);          /*10 seconds delay */
   GPIO_PORTB_DATA_R &= ~(1 << 2); /* make trigger  pin low */

   while (1)
   {
      TIMER0_ICR_R = 0x4; /* clear timer0A capture flag */
      while ((TIMER0_RIS_R & 0x4) == 0)
         ;                              /* wait till captured */
      if (GPIO_PORTB_DATA_R & (1 << 6)) /*check if rising edge occurs */
      {
         lastEdge = TIMER0_TAR_R; /* save the timestamp */
                                  /* detect falling edge */
         TIMER0_ICR_R = 4;        /* clear timer0A capture flag */
         while ((TIMER0_RIS_R & 4) == 0)
            ;                          /* wait till captured */
         thisEdge = TIMER0_TAR_R;      /* save the timestamp */
         return (thisEdge - lastEdge); /* return the time difference */
      }
   }
}

void Timer0ACapture_init(void)
{
   SYSCTL_RCGCTIMER_R |= 1;          /* enable clock to Timer Block 0 */
   SYSCTL_RCGCGPIO_R |= 2;           /* enable clock to PORTB */
   GPIO_PORTB_DIR_R &= ~0x40;        /* make PB6 an input pin */
   GPIO_PORTB_DEN_R |= 0x40;         /* make PB6 as digital pin */
   GPIO_PORTB_AFSEL_R |= 0x40;       /* use PB6 alternate function */
   GPIO_PORTB_PCTL_R &= ~0x0F000000; /* configure PB6 for T0CCP0 */
   GPIO_PORTB_PCTL_R |= 0x07000000;  /* PB2 as a digital output signal to provide trigger signal */
   // SYSCTL_RCGCGPIO_R |= 1;          /* enable clock to PORTA */
   GPIO_PORTB_DIR_R |= (1 << 2); /* set PB2 as a digial output pin */
   GPIO_PORTB_DEN_R |= (1 << 2); /* make PB2 as digital pin */

   TIMER0_CTL_R &= ~1;       /* disable timer0A during setup */
   TIMER0_CFG_R = 4;         /* 16-bit timer mode */
   TIMER0_TAMR_R = 0x17;     /* up-count, edge-time, capture mode */
   TIMER0_CTL_R |= 0x0C;     /* capture the rising edge */
   TIMER0_CTL_R |= (1 << 0); /* enable timer0A */
}

void Delay_MicroSecond(int time)
{
   int i;
   SYSCTL_RCGCTIMER_R |= 2; /* enable clock to Timer Block 1 */
   TIMER1_CTL_R = 0;        /* disable Timer before initialization */
   TIMER1_CFG_R = 0x04;     /* 16-bit option */
   TIMER1_TAMR_R = 0x02;    /* periodic mode and down-counter */
   TIMER1_TAILR_R = 16 - 1; /* TimerA interval load value reg */
   TIMER1_ICR_R = 0x1;      /* clear the TimerA timeout flag */
   TIMER1_CTL_R |= 0x01;    /* enable Timer A after initialization */

   for (i = 0; i < time; i++)
   {
      while ((TIMER1_RIS_R & 0x1) == 0)
         ;                /* wait for TimerA timeout flag */
      TIMER1_ICR_R = 0x1; /* clear the TimerA timeout flag */
   }
}
void PrintUltraSonic()
{
   uint32_t time;
   uint32_t distance;
   time = Measure_distance();            /* take pulse duration measurement */
   distance = (time * 10625) / 10000000; /* convert pulse duration into distance */
   if (distance >= 20 && distance <= 50)
   {
      printf("Distance : %d\n", distance);
    Bluetooth_Write_String("object near \n");
    OpenLaser(250000);
   }
}

void Delay(unsigned long counter)
{
   unsigned long i = 0;

   for (i = 0; i < counter * 1000; i++)
      ;
}