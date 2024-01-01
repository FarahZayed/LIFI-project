
#include "../LIB/std_types.h"
#include "../LIB/tm4c123gh6pm.h"
#include "../MCAL/DIO.h"
#include "fume.h"
#include <stdio.h>
#include "stdint.h"

uint8 is_smoking = 0;
void fume_init(){
  // to be removed
  DIO_Init(PORTB);
  DIO_Init(PORTF); // turn on bus clock for GPIOF
  DIO_DIR(PORTF, PIN3, OUTPUT); //set GREEN pin as a digital output pin
  
  SET_BIT(SYSCTL_RCGCADC_R, 0);           // Enable ADC0 clock
  SET_BIT(GPIO_PORTB_AFSEL_R, 5);         // Enables the alternate function of PB5
  CLEAR_BIT(GPIO_PORTB_DEN_R, 5);         // Disable digital 
  SET_BIT(GPIO_PORTB_AMSEL_R, 5);         // Enable analog
  
  CLEAR_BIT(ADC0_ACTSS_R, 3);
  ADC0_EMUX_R &= ~0xF000;
  ADC0_SSMUX3_R = 0xB; 
  ADC0_SSCTL3_R |= 0x6;                   // take one sample at a time, set flag at 1st sample
  SET_BIT(ADC0_ACTSS_R, 3);               // enable ADC0 sequencer 3
}

void fume()
{

  SET_BIT(ADC0_PSSI_R, 3); /* Enable SS3 conversion or start sampling data from AN11 */
  if (READ_BIT(ADC0_RIS_R, 3) == 1)
  {                                      /* Wait untill sample conversion completed*/
    uint32_t adc_value = ADC0_SSFIFO3_R; /* read adc coversion result from SS3 FIFO*/
    SET_BIT(ADC0_ISC_R, 3);              /* clear coversion clear flag bit*/

    if (adc_value >= 1000)
    {
      if (is_smoking == 0)
      {

        DIO_WritePin(PORTF, PIN3, STD_HIGH); /* turn on green LED*/
        Bluetooth_Write_String("smoke \n");
        OpenLaser(50000);
        is_smoking = 1;
      }
    }
    else if (adc_value < 1000)
    {
      DIO_WritePin(PORTF, PIN3, STD_LOW); /* turn off green LED*/
      is_smoking = 0;
    }
  }
}