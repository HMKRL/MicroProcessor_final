/* 
 * File:   app2led.h
 * Author: hty
 *
 * Created on December 30, 2017, 4:34 PM
 */

#ifndef APP2LED_H
#define	APP2LED_H

void app2led_init(int module_row, int module_col);
void app2led_setLed(char *red, char *blue);
void app2led_setIntensity(char red, char blue);

#endif	/* APP2LED_H */
