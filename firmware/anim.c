#include <avr/io.h>      // this contains all the IO port definitions
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
 
#include "util.h"
#include "ratt.h"
#include "ks0108.h"
#include "glcd.h"
#include "font5x7.h"
#include "fonttable.h"
#include "bitmaps.h"


extern volatile uint8_t time_s, time_m, time_h;
extern volatile uint8_t old_m, old_h;
extern volatile uint8_t date_m, date_d, date_y;
extern volatile uint8_t alarming, alarm_h, alarm_m;
extern volatile uint8_t time_format;
extern volatile uint8_t region;
extern volatile uint8_t score_mode;

extern volatile uint8_t second_changed, minute_changed, hour_changed;

uint8_t redraw_time = 0;
uint8_t last_score_mode = 0;



void initanim(void) {
  DEBUG(putstring("screen width: "));
  DEBUG(uart_putw_dec(GLCD_XPIXELS));
  DEBUG(putstring("\n\rscreen height: "));
  DEBUG(uart_putw_dec(GLCD_YPIXELS));
  DEBUG(putstring_nl(""));

}

void initdisplay(uint8_t inverted) {
  glcdFillRectangle(0, 0, GLCD_XPIXELS, GLCD_YPIXELS, inverted);
}


void drawdisplay(void) {
  uint8_t inverted = 0;

    // draw time or alarm
    uint8_t left, right;
    if (score_mode == SCORE_MODE_ALARM) {
      left = alarm_h;
      right = alarm_m;
    } else if (score_mode == SCORE_MODE_YEAR) {
      left = 20;
      right = date_y % 100;
    } else {
      left = time_h;
      right = time_m;
    }
    uint8_t am = (left < 12);
    if (time_format == TIME_12H) {
      left = (left + 23)%12 + 1;
      if(am) {
      	drawdot(GLCD_XPIXELS/2, GLCD_YPIXELS*1/10, inverted);
      } else {
      	drawdot(GLCD_XPIXELS/2, GLCD_YPIXELS*1/10, !inverted);
      }
    }
    else
      drawdot(GLCD_XPIXELS/2, GLCD_YPIXELS*1/10, !inverted);
      

    // Draw time
    glcdDrawBitmap(&DigitsLarge[(left/10) * 25 * 5], 3, 0, 25, 5);
    glcdDrawBitmap(&DigitsLarge[(left%10) * 25 * 5], 31, 0, 25, 5);
    
    glcdDrawBitmap(&DigitsLarge[(right/10) * 25 * 5], 72, 0, 25, 5);
    glcdDrawBitmap(&DigitsLarge[(right%10) * 25 * 5], 100, 0, 25, 5);

    // Draw date
    glcdDrawBitmap(&Weekdays[dotw(date_m, date_d, date_y) * 47 * 3], 1, 5, 47, 3);

    if (region == REGION_US) {
      left = date_m;
      right = date_d;
    } else {
      left = date_d;
      right = date_m;
    }
    glcdDrawBitmap(&DigitsSmall[(left/10) * 14 * 3], 59, 5, 14, 3);
    glcdDrawBitmap(&DigitsSmall[(left%10) * 14 * 3], 75, 5, 14, 3);
    glcdFillRectangle(91, 56, 4, 2, ON);
    glcdFillRectangle(92, 55, 2, 4, ON);
    glcdDrawBitmap(&DigitsSmall[(right/10) * 14 * 3], 97, 5, 14, 3);
    glcdDrawBitmap(&DigitsSmall[(right%10) * 14 * 3], 113, 5, 14, 3);
    
    if (second_changed && time_s%2 && (score_mode != SCORE_MODE_YEAR)) {
      drawdot(GLCD_XPIXELS/2, 15, 0);
      drawdot(GLCD_XPIXELS/2, 30, 0);
    } else {
      drawdot(GLCD_XPIXELS/2, 15, 1);
      drawdot(GLCD_XPIXELS/2, 30, 1);
    }
}


void step(void) {
}

void drawdot(uint8_t x, uint8_t y, uint8_t inverted) {
  glcdFillCircle(x, y, DOTRADIUS, !inverted);
}

uint8_t dotw(uint8_t mon, uint8_t day, uint8_t yr)
{
  uint16_t month, year;

    // Calculate day of the week
    
    month = mon;
    year = 2000 + yr;
    if (mon < 3)  {
      month += 12;
      year -= 1;
    }
    return (day + (2 * month) + (6 * (month+1)/10) + year + (year/4) - (year/100) + (year/400) + 1) % 7;
}
