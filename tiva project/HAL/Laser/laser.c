#include "laser.h"
#include "DIO.h"

void InitLaser()
{
  DIO_Init(PORTD);
  DIO_DIR(PORTD, PIN7, OUTPUT);
}

void OpenLaser(int time)
{

  DIO_WritePin(PORTD, PIN7, 1);
  Delay_MicroSecond(time);
  DIO_WritePin(PORTD, PIN7, 0);
}

