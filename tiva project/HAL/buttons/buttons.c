#include "buttons.h"
#include "DIO.h"

void InitButtons()
{
  DIO_Init(PORTF);
  DIO_DIR(PORTF, PIN0, INPUT);
  DIO_DIR(PORTF, PIN4, INPUT);
}
