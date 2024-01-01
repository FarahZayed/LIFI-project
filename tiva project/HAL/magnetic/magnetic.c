#include "../LIB/std_types.h"
#include "../LIB/tm4c123gh6pm.h"
#include "../MCAL/DIO.h"
#include <stdio.h>
#include "stdint.h"
#include "magnetic.h"

uint8 is_door_opened = 0;

void magneticInit(void)
{
    DIO_Init(PORTB);
    DIO_DIR(PORTB, 7, INPUT);
    // SET_BIT(GPIO_PORTB_PUR_R,7);
}

void get_magnetic_status()
{
  uint8 status;
  status = DIO_ReadPin(PORTB, PIN7);
  if (status == 1)
  {
    if (is_door_opened == 0)
    {
      Bluetooth_Write_String("door opened \n");
      OpenLaser(150000);
      is_door_opened = 1;
    }
  }
  else
  {
    is_door_opened = 0;
  }
}
