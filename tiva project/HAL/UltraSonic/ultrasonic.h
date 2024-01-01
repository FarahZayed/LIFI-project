#ifndef _HAL_ULTRASONIC_H_
#define _HAL_ULTRASONIC_H_
#include "stdint.h"

/**
 *
 *
 *
 *
 */
uint32_t Measure_distance(void);
/**
 *
 *
 *
 *
 */
void Timer0ACapture_init(void);
/**
 *
 *
 *
 *
 *
 */
void Delay_MicroSecond(int time);
/**
 *
 *
 *
 *
 */
void Delay(unsigned long counter);
/**
 *
 *
 *
 *
 *
 *
 */
/**
 *
 *  stores measured distance value
 *
 *
 *
 * */
void PrintUltraSonic();

#endif