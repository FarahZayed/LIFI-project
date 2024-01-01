
#include "LIB/tm4c123gh6pm.h"
#include <stdio.h>
#include "LIB/std_types.h"
#include "./HAL/fume/fume.h"
#include "./HAL/magnetic/magnetic.h"
#include "./HAL/ultrasonic/ultrasonic.h"
#include "DIO.h"

int main(void)
{

  InitButtons();
  InitLaser();

  Timer0ACapture_init();
  fume_init();
  magneticInit();
  HC05_init();
  uint8 is_working = 1;

  while (1)
  {
    if (DIO_ReadPin(PORTF, PIN0) == 0)
    {
      is_working = 1;
    }
    if (DIO_ReadPin(PORTF, PIN4) == 0)
    {
      is_working = 0;
    }
    if (is_working)
    {
      fume();
      PrintUltraSonic();
      get_magnetic_status();
    }
    else
    {
      printf("Not working\n");
    }
  }
}
