/*
 * CFAF128128B0145T.h
 *
 * Created on: 18.08.2016
 *     Author: Andreas Giemza <andreas.giemza@htwg-konstanz.de>
 *
 * License:
 *   Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
 *   https://creativecommons.org/licenses/by-nc-sa/3.0/
 *
 * Based on:
 *   LCD_screen Library Suite by Rei VILO
 *   http://embeddedcomputing.weebly.com/lcd_screen-library-suite.html
 *
 * Display             : Crystalfontz CFAF128128B-0145T
 * Display Datasheet   : https://www.crystalfontz.com/products/document/3406/CFAF128128B-0145T_Data_Sheet_2015-05-07.pdf
 * Controller          : Sitronix ST7735S
 * Controller Datasheet: http://www.crystalfontz.com/controllers/Sitronix/ST7735S/320
 *
 * Connection BoosterPack 1:
 *   PD1 MOSI
 *   PD3 CLK
 *   PF2 BACKLIGHT
 *   PH3 RST
 *   PL3 RS
 *   PN2 CS
 *
 * Connection BoosterPack 2:
 *   PQ2 MOSI
 *   PQ0 CLK
 *   PK4 BACKLIGHT
 *   PA7 RST
 *   PK7 RS
 *   PP3 CS
 */

#ifndef CFAF128128B0145T_H_
#define CFAF128128B0145T_H_

// Includes
// --------
#include <stdbool.h>
#include <stdint.h>

// Defines
// -------
#define CFAF128128B0145T_SIZE_XY    128

// Colors
// ------
static const uint16_t CFAF128128B0145T_color_black = 0b0000000000000000;
static const uint16_t CFAF128128B0145T_color_white = 0b1111111111111111;
static const uint16_t CFAF128128B0145T_color_red = 0b1111100000000000;
static const uint16_t CFAF128128B0145T_color_green = 0b0000011111100000;
static const uint16_t CFAF128128B0145T_color_blue = 0b0000000000011111;
static const uint16_t CFAF128128B0145T_color_yellow = 0b1111111111100000;
static const uint16_t CFAF128128B0145T_color_cyan = 0b0000011111111111;
static const uint16_t CFAF128128B0145T_color_orange = 0b1111101111100000;
static const uint16_t CFAF128128B0145T_color_magenta = 0b1111100000001111;
static const uint16_t CFAF128128B0145T_color_violet = 0b1111100000011111;
static const uint16_t CFAF128128B0145T_color_gray = 0b0111101111101111;
static const uint16_t CFAF128128B0145T_color_darkGray = 0b0011100111100111;

// Prototypes
// ----------

// "public"
extern void CFAF128128B0145T_init(uint8_t bosterPack, uint32_t sysClock, uint32_t spiClock);
extern void CFAF128128B0145T_invert(bool flag);
extern void CFAF128128B0145T_setOrientation(uint8_t orientation);
extern uint8_t CFAF128128B0145T_getOrientation();
extern void CFAF128128B0145T_setPenSolid(bool flag);
extern void CFAF128128B0145T_setFontSolid(bool flag);
extern void CFAF128128B0145T_rectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t colour);
extern void CFAF128128B0145T_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t colour);
extern void CFAF128128B0145T_clear(uint16_t colour);
extern void CFAF128128B0145T_point(int16_t x, int16_t y, uint16_t colour);
extern void CFAF128128B0145T_text(int16_t x0, int16_t y0, const char *s, uint16_t textColour, uint16_t backColour, uint8_t ix, uint8_t iy);
extern void CFAF128128B0145T_circle(int16_t x0, int16_t y0, int16_t radius, uint16_t colour);
extern void CFAF128128B0145T_triangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t colour);
extern uint16_t CFAF128128B0145T_calculateColour(uint8_t red, uint8_t green, uint8_t blue);
extern void CFAF128128B0145T_splitColour(uint16_t rgb, uint8_t *red, uint8_t *green, uint8_t *blue);
extern uint16_t CFAF128128B0145T_halveColour(uint16_t rgb);

#endif /* CFAF128128B0145T_H_ */
