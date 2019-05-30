/*********************************************************************************
 * \file   webserv_appl_fcns.c
 * \author Daniel Grotz
 * \date   12.05.2019
 *
 *      This file contains the added functions for the Webserver implementation.
 *      In addition to use the Light Sensor you have to add the file
 *      I2C_LightSens.c to the Project.
 *      This file contains the following functions:
 *      - void InitTimer0Int(void)
 *      - void InitTimer1Int(void)
 *      - void WriteDisplay(const char* String, uint16_t textColor, uint8_t CMD)
 *      - void convertINTtoCHAR(int32_t i32value, char string[])
 *      - void reverse(char string[])
 *      - void ReadDataIntHandler(void)
 *
 *********************************************************************************/

/*********************************************************************************
 *  Includes
 *********************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "drivers/CFAF128128B0145T.h"
#include "webserv_appl_fcns.h"

/*********************************************************************************
 *  Variables
 *********************************************************************************/
extern uint32_t g_ui32SysClock;
extern uint32_t g_ui32IPAddress;
int16_t g_i16_yhigh = 45;           //predefined high for display values
uint8_t g_ui8_getLightSensData = 0;

/*********************************************************************************
 * \brief Init of the FATFS Timer interrupt
 *
 *    This function initializes the timer interrupt for the timout
 *    Setting of the timer timeout with 0.01s.
 *********************************************************************************/

/* todo: Hier Initialisierung des Timers einfügen*/

/*********************************************************************************
 * \brief Init of the Read Data timer interrupt
 *
 *    This function initializes the timer interrupt to set the "read data flag"
 *    Setting of the timer timeout with 1s.
 *********************************************************************************/
void InitTimer1Int(void)
{
    uint32_t ui32_Period;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    ui32_Period = (g_ui32SysClock/2);                        //60MHz -> Period of 0.5s

    TimerLoadSet(TIMER1_BASE, TIMER_A, ui32_Period - 1);     //-1 wegen null-count

    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER1_BASE, TIMER_A);
    IntEnable(INT_TIMER1A);
    //IntMasterEnable();
}

/*********************************************************************************
 * \brief Writes stings to the display
 *
 *     This function writes a string to the display in a predefined color.
 *     With the CMD parameter you can write the static IP and MAC header
 *
 * \param   const char* String   pointer to string to write
 *          const char* MAC_Address  pointer to MAC Address
 *          uint16_t textColor   define the text color
 *          uint8_t              command with defines type of string to write
 * \return  void
 *********************************************************************************/
void WriteDisplay(const char* String, const char* MAC_Address, uint16_t textColor, uint8_t CMD)
{
    uint16_t ui16_ybuffer = g_i16_yhigh;
    if (CMD == 0)
    {
        if (g_i16_yhigh > 120)
        {
            CFAF128128B0145T_clear(CFAF128128B0145T_color_black);
            DisplayIPnMACAddress(g_ui32IPAddress);
            g_i16_yhigh = 45;
        }
        CFAF128128B0145T_text(10, g_i16_yhigh, String, textColor,
                              CFAF128128B0145T_color_black, 1, 1);
        g_i16_yhigh = g_i16_yhigh + 10; /* Strings have distance of 10 */
    }

    if (CMD == CMD_DISPLAY_IP_MAC)
    {
        g_i16_yhigh = 2;
        CFAF128128B0145T_text(10, g_i16_yhigh, "IP Address", textColor,
                              CFAF128128B0145T_color_black, 1, 1);
        CFAF128128B0145T_text(10, g_i16_yhigh + 10, String, textColor,
                              CFAF128128B0145T_color_black, 1, 1);
        CFAF128128B0145T_text(10, g_i16_yhigh + 20, "MAC Address", textColor,
                              CFAF128128B0145T_color_black, 1, 1);
        CFAF128128B0145T_text(10, g_i16_yhigh + 30, MAC_Address, textColor,
                              CFAF128128B0145T_color_black, 1, 1);
        CFAF128128B0145T_rectangle(0, 0, 126, 42, textColor);
        g_i16_yhigh = ui16_ybuffer;
    }
}

/*********************************************************************************
 * \brief Converts int to string
 *
 *     This function converts an integer value to an ASCII-String value
 *
 * \param   int32_t i32_value   Integer value to convert
 *          char string[]       Converted string
 * \return  void
 *********************************************************************************/
void convertINTtoCHAR(int32_t i32value, char string[])
{
    int i, sign;
    i = 0;

    if ((sign = i32value) < 0)             // record sign
        i32value = -i32value;              // make n positive

    do {                                    // generate digits in reverse order
        string[i++] = i32value % 10 + '0';  // get next digit
    } while ((i32value /= 10) > 0);

    if (sign < 0)                           //if sign is negative add "-"
        string[i++] = '-';
    string[i] = '\0';
    reverse(string);
}

void reverse(char string[])
 {
     int i, j;
     char c;

     for (i = 0, j = strlen(string)-1; i<j; i++, j--) {
         c = string[i];
         string[i] = string[j];
         string[j] = c;
     }
 }

/*********************************************************************************
 * \brief Read data int handler
 *
 *     This function sets the flag to signal that a new light sensor values is
 *     requested
 *********************************************************************************/
void ReadDataIntHandler(void)
{
    g_ui8_getLightSensData = 1;
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
}
