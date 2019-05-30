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

// Includes
// --------
#include "CFAF128128B0145T.h"

#include <string.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"

#include "CFAF128128B0145T_font6x8.h"

// Defines
// -------

// Display
#define CFAF128128B0145T__MADCTL_MY  0x80
#define CFAF128128B0145T__MADCTL_MX  0x40
#define CFAF128128B0145T__MADCTL_MV  0x20
#define CFAF128128B0145T__MADCTL_ML  0x10
#define CFAF128128B0145T__MADCTL_RGB 0x08
#define CFAF128128B0145T__MADCTL_MH  0x04
#define CFAF128128B0145T__NOP        0x00
#define CFAF128128B0145T__SWRESET    0x01
#define CFAF128128B0145T__RDDID      0x04
#define CFAF128128B0145T__RDDST      0x09
#define CFAF128128B0145T__SLPIN      0x10
#define CFAF128128B0145T__SLPOUT     0x11
#define CFAF128128B0145T__PTLON      0x12
#define CFAF128128B0145T__NORON      0x13
#define CFAF128128B0145T__INVOFF     0x20
#define CFAF128128B0145T__INVON      0x21
#define CFAF128128B0145T__GAMSET     0x26
#define CFAF128128B0145T__DISPOFF    0x28
#define CFAF128128B0145T__DISPON     0x29
#define CFAF128128B0145T__CASET      0x2A
#define CFAF128128B0145T__RASET      0x2B
#define CFAF128128B0145T__RAMWR      0x2C
#define CFAF128128B0145T__RGBSET     0x2d
#define CFAF128128B0145T__RAMRD      0x2E
#define CFAF128128B0145T__PTLAR      0x30
#define CFAF128128B0145T__MADCTL     0x36
#define CFAF128128B0145T__COLMOD     0x3A
#define CFAF128128B0145T__SETPWCTR   0xB1
#define CFAF128128B0145T__SETDISPL   0xB2
#define CFAF128128B0145T__FRMCTR3    0xB3
#define CFAF128128B0145T__SETCYC     0xB4
#define CFAF128128B0145T__SETBGP     0xb5
#define CFAF128128B0145T__SETVCOM    0xB6
#define CFAF128128B0145T__SETSTBA    0xC0
#define CFAF128128B0145T__SETID      0xC3
#define CFAF128128B0145T__GETHID     0xd0
#define CFAF128128B0145T__SETGAMMA   0xE0

// Helpers
#define CFAF128128B0145T__lowByte(w)  ((uint8_t) ((w) & 0xff))
#define CFAF128128B0145T__highByte(w) ((uint8_t) ((w) >> 8))
#define CFAF128128B0145T__bitRead(value, bit) (((value) >> (bit)) & 0x01)

// Variables
//----------

// "private"
uint8_t CFAF128128B0145T__boosterPack;
uint32_t CFAF128128B0145T__sysClock;
uint32_t CFAF128128B0145T__spiClock;
uint32_t CFAF128128B0145T__spiDevice;
uint32_t CFAF128128B0145T__csPort;
uint8_t CFAF128128B0145T__csPin;
uint32_t CFAF128128B0145T__rstPort;
uint8_t CFAF128128B0145T__rstPin;
uint32_t CFAF128128B0145T__rsPort;
uint8_t CFAF128128B0145T__rsPin;
uint8_t CFAF128128B0145T__orientation;
bool CFAF128128B0145T__penSolid, CFAF128128B0145T__fontSolid;

// Prototypes
// ----------

// "private"
void CFAF128128B0145T__triangleArea(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t colour);
void CFAF128128B0145T__swapINT16(int16_t *a, int16_t *b);
void CFAF128128B0145T__setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void CFAF128128B0145T__fastFill(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t colour);
void CFAF128128B0145T__delay(uint32_t ms);
void CFAF128128B0145T__writeCommand(uint8_t command8);
void CFAF128128B0145T__writeRegister(uint8_t command8, uint8_t data8);
void CFAF128128B0145T__writeData(uint8_t data8);
void CFAF128128B0145T__writeData16(uint16_t data16);
void CFAF128128B0145T__writeData88(uint8_t dataHigh8, uint8_t dataLow8);
void CFAF128128B0145T__writeData8888(uint8_t dataHigh8, uint8_t dataLow8, uint8_t data8_3, uint8_t data8_4);
inline void CFAF128128B0145T__setRST(bool flag);
inline void CFAF128128B0145T__setRS(bool flag);
inline void CFAF128128B0145T__setCS(bool flag);

// Functions
// ---------

// "public"
void CFAF128128B0145T_init(uint8_t bosterPack, uint32_t sysClock, uint32_t spiClock) {
    uint32_t initialData = 0;

    CFAF128128B0145T__boosterPack = bosterPack;
    CFAF128128B0145T__sysClock = sysClock;
    CFAF128128B0145T__spiClock = spiClock;

    // Enable and configure SPI
    // ------------------------
    if (CFAF128128B0145T__boosterPack == 1) {
        CFAF128128B0145T__spiDevice = SSI2_BASE;

        SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);
        while (!SysCtlPeripheralReady(SYSCTL_PERIPH_SSI2)) {
        }
        SSIDisable(CFAF128128B0145T__spiDevice);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
        while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)) {
        }
        GPIOPinConfigure(GPIO_PD3_SSI2CLK);
        GPIOPinConfigure(GPIO_PD1_SSI2XDAT0);
        GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_3 | GPIO_PIN_1);
    } else {
        CFAF128128B0145T__spiDevice = SSI3_BASE;

        SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI3);
        while (!SysCtlPeripheralReady(SYSCTL_PERIPH_SSI3)) {
        }
        SSIDisable(CFAF128128B0145T__spiDevice);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);
        while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOQ)) {
        }
        GPIOPinConfigure(GPIO_PQ0_SSI3CLK);
        GPIOPinConfigure(GPIO_PQ2_SSI3XDAT0);
        GPIOPinTypeSSI(GPIO_PORTQ_BASE, GPIO_PIN_0 | GPIO_PIN_2);
    }

    SSIConfigSetExpClk(CFAF128128B0145T__spiDevice, sysClock,
    SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, spiClock, 8);
    SSIEnable(CFAF128128B0145T__spiDevice);
    while (SSIDataGetNonBlocking(CFAF128128B0145T__spiDevice, &initialData)) {
    }

    // Enable other GPIOS
    // ------------------
    if (CFAF128128B0145T__boosterPack == 1) {
        // Setup RST pin
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
        while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOH)) {
        }
        CFAF128128B0145T__rstPort = GPIO_PORTH_BASE;
        CFAF128128B0145T__rstPin = GPIO_PIN_3;

        // Setup RS pin
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
        while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOL)) {
        }
        CFAF128128B0145T__rsPort = GPIO_PORTL_BASE;
        CFAF128128B0145T__rsPin = GPIO_PIN_3;

        // Setup CS pin
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
        while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION)) {
        }
        CFAF128128B0145T__csPort = GPIO_PORTN_BASE;
        CFAF128128B0145T__csPin = GPIO_PIN_2;
    } else {
        // Setup RST pin
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
        while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)) {
        }
        CFAF128128B0145T__rstPort = GPIO_PORTA_BASE;
        CFAF128128B0145T__rstPin = GPIO_PIN_7;

        // Setup RS pin
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
        while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOK)) {
        }
        CFAF128128B0145T__rsPort = GPIO_PORTK_BASE;
        CFAF128128B0145T__rsPin = GPIO_PIN_7;

        // Setup CS pin
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
        while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOP)) {
        }
        CFAF128128B0145T__csPort = GPIO_PORTP_BASE;
        CFAF128128B0145T__csPin = GPIO_PIN_3;
    }

    GPIOPinTypeGPIOOutput(CFAF128128B0145T__rstPort, CFAF128128B0145T__rstPin);
    GPIOPinTypeGPIOOutput(CFAF128128B0145T__rsPort, CFAF128128B0145T__rsPin);
    GPIOPinTypeGPIOOutput(CFAF128128B0145T__csPort, CFAF128128B0145T__csPin);

    // Init display
    CFAF128128B0145T__setRST(true);
    CFAF128128B0145T__delay(100);
    CFAF128128B0145T__setRST(false);
    CFAF128128B0145T__delay(50);
    CFAF128128B0145T__setRST(true);
    CFAF128128B0145T__delay(120);
    CFAF128128B0145T__writeCommand(CFAF128128B0145T__SWRESET);
    CFAF128128B0145T__delay(150);
    CFAF128128B0145T__writeCommand(CFAF128128B0145T__SLPOUT);
    CFAF128128B0145T__delay(200);
    CFAF128128B0145T__writeRegister(CFAF128128B0145T__GAMSET, 0x04);
    CFAF128128B0145T__writeCommand(CFAF128128B0145T__SETPWCTR);
    CFAF128128B0145T__writeData88(0x0A, 0x14);
    CFAF128128B0145T__writeCommand(CFAF128128B0145T__SETSTBA);
    CFAF128128B0145T__writeData88(0x0A, 0x00);
    CFAF128128B0145T__writeRegister(CFAF128128B0145T__COLMOD, 0x05);
    CFAF128128B0145T__delay(10);
    CFAF128128B0145T__writeRegister(CFAF128128B0145T__MADCTL,
    CFAF128128B0145T__MADCTL_RGB);
    CFAF128128B0145T__writeCommand(CFAF128128B0145T__CASET);
    CFAF128128B0145T__writeData8888(0x00, 0x00, 0x00, 0x79);
    CFAF128128B0145T__writeCommand(CFAF128128B0145T__RASET);
    CFAF128128B0145T__writeData8888(0x00, 0x00, 0x00, 0x79);
    CFAF128128B0145T__writeCommand(CFAF128128B0145T__NORON);
    CFAF128128B0145T__delay(10);
    CFAF128128B0145T__writeCommand(CFAF128128B0145T__DISPON);
    CFAF128128B0145T__delay(120);
    CFAF128128B0145T__writeCommand(CFAF128128B0145T__RAMWR);
    CFAF128128B0145T_setOrientation(0);
    CFAF128128B0145T_setPenSolid(false);
    CFAF128128B0145T_setFontSolid(true);
    CFAF128128B0145T_clear(CFAF128128B0145T_color_black);
}

void CFAF128128B0145T_invert(bool flag) {
    CFAF128128B0145T__writeCommand(flag ? CFAF128128B0145T__INVON : CFAF128128B0145T__INVOFF);
}

void CFAF128128B0145T_setOrientation(uint8_t orientation) {
    CFAF128128B0145T__orientation = orientation % 4;
    CFAF128128B0145T__writeCommand(CFAF128128B0145T__MADCTL);
    switch (CFAF128128B0145T__orientation) {
        case 0:
            CFAF128128B0145T__writeData(
            CFAF128128B0145T__MADCTL_MX | CFAF128128B0145T__MADCTL_MY | CFAF128128B0145T__MADCTL_RGB);
            break;
        case 1:
            CFAF128128B0145T__writeData(
            CFAF128128B0145T__MADCTL_MY | CFAF128128B0145T__MADCTL_MV | CFAF128128B0145T__MADCTL_RGB);
            break;
        case 2:
            CFAF128128B0145T__writeData(CFAF128128B0145T__MADCTL_RGB);
            break;
        case 3:
            CFAF128128B0145T__writeData(
            CFAF128128B0145T__MADCTL_MX | CFAF128128B0145T__MADCTL_MV | CFAF128128B0145T__MADCTL_RGB);
            break;
    }
}

uint8_t CFAF128128B0145T_getOrientation() {
    return CFAF128128B0145T__orientation;
}

void CFAF128128B0145T_setPenSolid(bool flag) {
    CFAF128128B0145T__penSolid = flag;
}

void CFAF128128B0145T_setFontSolid(bool flag) {
    CFAF128128B0145T__fontSolid = flag;
}

void CFAF128128B0145T_rectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t colour) {
    if (CFAF128128B0145T__penSolid == false) {
        CFAF128128B0145T_line(x1, y1, x1, y2, colour);
        CFAF128128B0145T_line(x1, y1, x2, y1, colour);
        CFAF128128B0145T_line(x1, y2, x2, y2, colour);
        CFAF128128B0145T_line(x2, y1, x2, y2, colour);
    } else {
        CFAF128128B0145T__fastFill(x1, y1, x2, y2, colour);
    }
}

void CFAF128128B0145T_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t colour) {
    if ((x1 == x2) && (y1 == y2)) {
        CFAF128128B0145T_point(x1, y1, colour);
    } else if ((x1 == x2) || (y1 == y2)) {
        CFAF128128B0145T__fastFill(x1, y1, x2, y2, colour);
    } else {
        bool flag = abs(y2 - y1) > abs(x2 - x1);
        if (flag) {
            CFAF128128B0145T__swapINT16(&x1, &y1);
            CFAF128128B0145T__swapINT16(&x2, &y2);
        }
        if (x1 > x2) {
            CFAF128128B0145T__swapINT16(&x1, &x2);
            CFAF128128B0145T__swapINT16(&y1, &y2);
        }
        int16_t dx = x2 - x1;
        int16_t dy = abs(y2 - y1);
        int16_t err = dx / 2;
        int16_t ystep;
        if (y1 < y2)
            ystep = 1;
        else
            ystep = -1;
        for (; x1 <= x2; x1++) {
            if (flag)
                CFAF128128B0145T_point(y1, x1, colour);
            else
                CFAF128128B0145T_point(x1, y1, colour);
            err -= dy;
            if (err < 0) {
                y1 += ystep;
                err += dx;
            }
        }
    }
}

void CFAF128128B0145T_clear(uint16_t colour) {
    uint8_t oldOrientation = CFAF128128B0145T__orientation;
    bool oldPenSolid = CFAF128128B0145T__penSolid;
    CFAF128128B0145T_setOrientation(0);
    CFAF128128B0145T_setPenSolid(1);
    CFAF128128B0145T_rectangle(0, 0, CFAF128128B0145T_SIZE_XY - 1,
    CFAF128128B0145T_SIZE_XY - 1, colour);
    CFAF128128B0145T_setOrientation(oldOrientation);
    CFAF128128B0145T_setPenSolid(oldPenSolid);
}

void CFAF128128B0145T_point(int16_t x, int16_t y, uint16_t colour) {
    if ((x < 0) || (x >= CFAF128128B0145T_SIZE_XY) || (y < 0) || (y >= CFAF128128B0145T_SIZE_XY)) {
        return;
    }

    uint16_t ux = (uint16_t) x;
    uint16_t uy = (uint16_t) y;

    CFAF128128B0145T__setWindow(ux, uy, ux + 1, uy + 1);
    CFAF128128B0145T__writeData16(colour);
}

void CFAF128128B0145T_text(int16_t x0, int16_t y0, const char *s, uint16_t textColour, uint16_t backColour, uint8_t ix, uint8_t iy) {
    if ((x0 >= CFAF128128B0145T_SIZE_XY) || (y0 >= CFAF128128B0145T_SIZE_XY) || ((y0 + (CFAF128128B0145T__FONT_SIZE_Y * iy)) < 0)) {
        return;
    }

    uint8_t c;
    uint8_t line;
    uint16_t x, y;
    uint8_t i, j, k;

    if ((ix > 1) || (iy > 1) || !CFAF128128B0145T__fontSolid) {
        if ((ix > 1) || (iy > 1)) {
            bool oldPenSolid = CFAF128128B0145T__penSolid;
            CFAF128128B0145T_setPenSolid(true);
            for (k = 0; k < strlen(s); k++) {
                x = x0 + 6 * k * ix;
                y = y0;
                c = s[k] - ' ';
                for (i = 0; i < 6; i++) {
                    line = CFAF128128B0145T__font6x8[c][i];
                    for (j = 0; j < 8; j++) {
                        if (CFAF128128B0145T__bitRead(line, j))
                            CFAF128128B0145T_rectangle(x + i * ix, y + j * iy, x + i * ix + ix - 1, y + j * iy + iy - 1, textColour);
                        else if (CFAF128128B0145T__fontSolid)
                            CFAF128128B0145T_rectangle(x + i * ix, y + j * iy, x + i * ix + ix - 1, y + j * iy + iy - 1, backColour);
                    }
                }
            }
            CFAF128128B0145T_setPenSolid(oldPenSolid);
        } else {
            for (k = 0; k < strlen(s); k++) {
                c = s[k] - ' ';
                for (i = 0; i < 6; i++) {
                    line = CFAF128128B0145T__font6x8[c][i];
                    for (j = 0; j < 8; j++)
                        if (CFAF128128B0145T__bitRead(line, j))
                            CFAF128128B0145T_point(x0 + 6 * k + i, y0 + j, textColour);
                        else if (CFAF128128B0145T__fontSolid)
                            CFAF128128B0145T_point(x0 + 6 * k + i, y0 + j, backColour);
                }
            }
        }
    } else {
        uint8_t c;
        uint8_t line;
        uint8_t i, j, k;
        uint8_t highTextColour = CFAF128128B0145T__highByte(textColour);
        uint8_t lowTextColour = CFAF128128B0145T__lowByte(textColour);
        uint8_t highBackColour = CFAF128128B0145T__highByte(backColour);
        uint8_t lowBackColour = CFAF128128B0145T__lowByte(backColour);
        for (k = 0; k < strlen(s); k++) {
            c = s[k] - ' ';

            if (((x0 + CFAF128128B0145T__FONT_SIZE_X * k) > 0) && ((x0 + CFAF128128B0145T__FONT_SIZE_X * (k + 1) - 1) < CFAF128128B0145T_SIZE_XY) && (y0 > 0)
                    && ((y0 + CFAF128128B0145T__FONT_SIZE_Y - 1) < CFAF128128B0145T_SIZE_XY)) {
                CFAF128128B0145T__setWindow(x0 + CFAF128128B0145T__FONT_SIZE_X * k, y0, x0 + CFAF128128B0145T__FONT_SIZE_X * (k + 1) - 1,
                        y0 + CFAF128128B0145T__FONT_SIZE_Y - 1);
                for (j = 0; j < 8; j++) {
                    for (i = 0; i < 6; i++) {
                        line = CFAF128128B0145T__font6x8[c][i];
                        if (CFAF128128B0145T__bitRead(line, j)) {
                            CFAF128128B0145T__writeData88(highTextColour, lowTextColour);
                        } else {
                            CFAF128128B0145T__writeData88(highBackColour, lowBackColour);
                        }
                    }
                }
            } else {
                for (i = 0; i < 6; i++) {
                    line = CFAF128128B0145T__font6x8[c][i];
                    for (j = 0; j < 8; j++)
                        if (CFAF128128B0145T__bitRead(line, j))
                            CFAF128128B0145T_point(x0 + 6 * k + i, y0 + j, textColour);
                        else if (CFAF128128B0145T__fontSolid)
                            CFAF128128B0145T_point(x0 + 6 * k + i, y0 + j, backColour);
                }
            }
        }
    }
}

void CFAF128128B0145T_circle(int16_t x0, int16_t y0, int16_t radius, uint16_t colour) {
    int16_t f = 1 - radius;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * radius;
    int16_t x = 0;
    int16_t y = radius;
    if (CFAF128128B0145T__penSolid == false) {
        CFAF128128B0145T_point(x0, y0 + radius, colour);
        CFAF128128B0145T_point(x0, y0 - radius, colour);
        CFAF128128B0145T_point(x0 + radius, y0, colour);
        CFAF128128B0145T_point(x0 - radius, y0, colour);
        while (x < y) {
            if (f >= 0) {
                y--;
                ddF_y += 2;
                f += ddF_y;
            }
            x++;
            ddF_x += 2;
            f += ddF_x;
            CFAF128128B0145T_point(x0 + x, y0 + y, colour);
            CFAF128128B0145T_point(x0 - x, y0 + y, colour);
            CFAF128128B0145T_point(x0 + x, y0 - y, colour);
            CFAF128128B0145T_point(x0 - x, y0 - y, colour);
            CFAF128128B0145T_point(x0 + y, y0 + x, colour);
            CFAF128128B0145T_point(x0 - y, y0 + x, colour);
            CFAF128128B0145T_point(x0 + y, y0 - x, colour);
            CFAF128128B0145T_point(x0 - y, y0 - x, colour);
        }
    } else {
        while (x < y) {
            if (f >= 0) {
                y--;
                ddF_y += 2;
                f += ddF_y;
            }
            x++;
            ddF_x += 2;
            f += ddF_x;
            CFAF128128B0145T_line(x0 + x, y0 + y, x0 - x, y0 + y, colour);
            CFAF128128B0145T_line(x0 + x, y0 - y, x0 - x, y0 - y, colour);
            CFAF128128B0145T_line(x0 + y, y0 - x, x0 + y, y0 + x, colour);
            CFAF128128B0145T_line(x0 - y, y0 - x, x0 - y, y0 + x, colour);
        }
        CFAF128128B0145T_setPenSolid(true);
        CFAF128128B0145T_rectangle(x0 - x, y0 - y, x0 + x, y0 + y, colour);
    }
}

void CFAF128128B0145T_triangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t colour) {
    if (CFAF128128B0145T__penSolid) {
        bool b = true;
        while (b) {
            b = false;
            if ((b == false) && (y1 > y2)) {
                CFAF128128B0145T__swapINT16(&x1, &x2);
                CFAF128128B0145T__swapINT16(&y1, &y2);
                b = true;
            }
            if ((b == false) && (y2 > y3)) {
                CFAF128128B0145T__swapINT16(&x3, &x2);
                CFAF128128B0145T__swapINT16(&y3, &y2);
                b = true;
            }
        }
        if (y2 == y3) {
            CFAF128128B0145T__triangleArea(x1, y1, x2, y2, x3, y3, colour);
        } else if (y1 == y2) {
            CFAF128128B0145T__triangleArea(x3, y3, x1, y1, x2, y2, colour);
        } else {
            uint16_t x4 = (uint16_t) ((int32_t) x1 + (y2 - y1) * (x3 - x1) / (y3 - y1));
            uint16_t y4 = y2;
            CFAF128128B0145T__triangleArea(x1, y1, x2, y2, x4, y4, colour);
            CFAF128128B0145T__triangleArea(x3, y3, x2, y2, x4, y4, colour);
        }
    } else {
        CFAF128128B0145T_line(x1, y1, x2, y2, colour);
        CFAF128128B0145T_line(x2, y2, x3, y3, colour);
        CFAF128128B0145T_line(x3, y3, x1, y1, colour);
    }
}

uint16_t CFAF128128B0145T_calculateColour(uint8_t red, uint8_t green, uint8_t blue) {
    return (red >> 3) << 11 | (green >> 2) << 5 | (blue >> 3);
}

void CFAF128128B0145T_splitColour(uint16_t rgb, uint8_t *red, uint8_t *green, uint8_t *blue) {
    *red = (rgb & 0b1111100000000000) >> 11 << 3;
    *green = (rgb & 0b0000011111100000) >> 5 << 2;
    *blue = (rgb & 0b0000000000011111) << 3;
}

uint16_t CFAF128128B0145T_halveColour(uint16_t rgb) {
    return ((rgb & 0b1111100000000000) >> 12 << 11 | (rgb & 0b0000011111100000) >> 6 << 5 | (rgb & 0b0000000000011111) >> 1);
}

// "private"
void CFAF128128B0145T__triangleArea(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t colour) {
    int i;
    int16_t x4 = x1;
    int16_t y4 = y1;
    int16_t x5 = x1;
    int16_t y5 = y1;
    bool changed1 = false;
    bool changed2 = false;
    int16_t dx1 = abs(x2 - x1);
    int16_t dy1 = abs(y2 - y1);
    int16_t dx2 = abs(x3 - x1);
    int16_t dy2 = abs(y3 - y1);
    int16_t signx1 = (x2 >= x1) ? +1 : -1;
    int16_t signx2 = (x3 >= x1) ? +1 : -1;
    int16_t signy1 = (y2 >= y1) ? +1 : -1;
    int16_t signy2 = (y3 >= y1) ? +1 : -1;
    if (dy1 > dx1) {
        CFAF128128B0145T__swapINT16(&dx1, &dy1);
        changed1 = true;
    }
    if (dy2 > dx2) {
        CFAF128128B0145T__swapINT16(&dx2, &dy2);
        changed2 = true;
    }
    int16_t e1 = 2 * dy1 - dx1;
    int16_t e2 = 2 * dy2 - dx2;
    for (i = 0; i <= dx1; i++) {
        CFAF128128B0145T_line(x4, y4, x5, y5, colour);
        while (e1 >= 0) {
            if (changed1)
                x4 += signx1;
            else
                y4 += signy1;
            e1 = e1 - 2 * dx1;
        }
        if (changed1)
            y4 += signy1;
        else
            x4 += signx1;
        e1 = e1 + 2 * dy1;
        while (y5 != y4) {
            while (e2 >= 0) {
                if (changed2)
                    x5 += signx2;
                else
                    y5 += signy2;
                e2 = e2 - 2 * dx2;
            }
            if (changed2)
                y5 += signy2;
            else
                x5 += signx2;
            e2 = e2 + 2 * dy2;
        }
    }
}

void CFAF128128B0145T__swapINT16(int16_t *a, int16_t *b) {
    int16_t w = *a;
    *a = *b;
    *b = w;
}

void CFAF128128B0145T__setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    switch (CFAF128128B0145T__orientation) {
        case 0:
            x0 += 2;
            y0 += 3;
            x1 += 2;
            y1 += 3;
            break;
        case 1:
            x0 += 3;
            y0 += 2;
            x1 += 3;
            y1 += 2;
            break;
        case 2:
            x0 += 2;
            y0 += 1;
            x1 += 2;
            y1 += 1;
            break;
        case 3:
            x0 += 1;
            y0 += 2;
            x1 += 1;
            y1 += 2;
            break;
        default:
            break;
    }

    CFAF128128B0145T__writeCommand(CFAF128128B0145T__CASET);
    CFAF128128B0145T__writeData16(x0);
    CFAF128128B0145T__writeData16(x1);
    CFAF128128B0145T__writeCommand(CFAF128128B0145T__RASET);
    CFAF128128B0145T__writeData16(y0);
    CFAF128128B0145T__writeData16(y1);
    CFAF128128B0145T__writeCommand(CFAF128128B0145T__RAMWR);
}

void CFAF128128B0145T__fastFill(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t colour) {
    if (x1 > x2) {
        CFAF128128B0145T__swapINT16(&x1, &x2);
    }
    if (y1 > y2) {
        CFAF128128B0145T__swapINT16(&y1, &y2);
    }

    // Check if on screen
    if ((x1 > CFAF128128B0145T_SIZE_XY - 1) || (x2 < 0) || (y1 > CFAF128128B0145T_SIZE_XY - 1) || (y2 < 0)) {
        return;
    }

    if (x1 < 0) {
        x1 = 0;
    }

    if (y1 < 0) {
        y1 = 0;
    }

    if (x2 > CFAF128128B0145T_SIZE_XY - 1) {
        x2 = CFAF128128B0145T_SIZE_XY - 1;
    }

    if (y2 > CFAF128128B0145T_SIZE_XY - 1) {
        y2 = CFAF128128B0145T_SIZE_XY - 1;
    }

    uint16_t ux1 = (uint16_t) x1;
    uint16_t uy1 = (uint16_t) y1;
    uint16_t ux2 = (uint16_t) x2;
    uint16_t uy2 = (uint16_t) y2;

    CFAF128128B0145T__setWindow(ux1, uy1, ux2, uy2);

    CFAF128128B0145T__setRS(true);
    CFAF128128B0145T__setCS(false);

    uint8_t hi = CFAF128128B0145T__highByte(colour);
    uint8_t lo = CFAF128128B0145T__lowByte(colour);

    uint32_t t;

    for (t = (uint32_t) (uy2 - uy1 + 1) * (ux2 - ux1 + 1); t > 0; t--) {
        SSIDataPut(CFAF128128B0145T__spiDevice, hi);
        while (SSIBusy(CFAF128128B0145T__spiDevice)) {
        }

        SSIDataPut(CFAF128128B0145T__spiDevice, lo);
        while (SSIBusy(CFAF128128B0145T__spiDevice)) {
        }
    }

    CFAF128128B0145T__setCS(true);
}

void CFAF128128B0145T__delay(uint32_t ms) {
    SysCtlDelay(CFAF128128B0145T__sysClock / 3000 * ms);
}

void CFAF128128B0145T__writeCommand(uint8_t command8) {
    CFAF128128B0145T__setRS(false);
    CFAF128128B0145T__setCS(false);

    SSIDataPut(CFAF128128B0145T__spiDevice, command8);
    while (SSIBusy(CFAF128128B0145T__spiDevice)) {
    }

    CFAF128128B0145T__setCS(true);
}

void CFAF128128B0145T__writeRegister(uint8_t command8, uint8_t data8) {
    CFAF128128B0145T__writeCommand(command8);
    CFAF128128B0145T__writeData(data8);
}

void CFAF128128B0145T__writeData(uint8_t data8) {
    CFAF128128B0145T__setRS(true);
    CFAF128128B0145T__setCS(false);

    SSIDataPut(CFAF128128B0145T__spiDevice, data8);
    while (SSIBusy(CFAF128128B0145T__spiDevice)) {
    }

    CFAF128128B0145T__setCS(true);
}

void CFAF128128B0145T__writeData16(uint16_t data16) {
    CFAF128128B0145T__setRS(true);
    CFAF128128B0145T__setCS(false);

    SSIDataPut(CFAF128128B0145T__spiDevice, CFAF128128B0145T__highByte(data16));
    while (SSIBusy(CFAF128128B0145T__spiDevice)) {
    }

    SSIDataPut(CFAF128128B0145T__spiDevice, CFAF128128B0145T__lowByte(data16));
    while (SSIBusy(CFAF128128B0145T__spiDevice)) {
    }

    CFAF128128B0145T__setCS(true);
}

void CFAF128128B0145T__writeData88(uint8_t dataHigh8, uint8_t dataLow8) {
    CFAF128128B0145T__setRS(true);
    CFAF128128B0145T__setCS(false);

    SSIDataPut(CFAF128128B0145T__spiDevice, dataHigh8);
    while (SSIBusy(CFAF128128B0145T__spiDevice)) {
    }

    SSIDataPut(CFAF128128B0145T__spiDevice, dataLow8);
    while (SSIBusy(CFAF128128B0145T__spiDevice)) {
    }

    CFAF128128B0145T__setCS(true);
}

void CFAF128128B0145T__writeData8888(uint8_t dataHigh8, uint8_t dataLow8, uint8_t data8_3, uint8_t data8_4) {
    CFAF128128B0145T__writeData(dataHigh8);
    CFAF128128B0145T__writeData(dataLow8);
    CFAF128128B0145T__writeData(data8_3);
    CFAF128128B0145T__writeData(data8_4);
}

inline void CFAF128128B0145T__setRST(bool flag) {
    GPIOPinWrite(CFAF128128B0145T__rstPort, CFAF128128B0145T__rstPin, (flag ? CFAF128128B0145T__rstPin : 0));
}

inline void CFAF128128B0145T__setRS(bool flag) {
    GPIOPinWrite(CFAF128128B0145T__rsPort, CFAF128128B0145T__rsPin, (flag ? CFAF128128B0145T__rsPin : 0));
}

inline void CFAF128128B0145T__setCS(bool flag) {
    GPIOPinWrite(CFAF128128B0145T__csPort, CFAF128128B0145T__csPin, (flag ? CFAF128128B0145T__csPin : 0));
}
