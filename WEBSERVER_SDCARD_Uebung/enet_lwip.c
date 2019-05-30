//*****************************************************************************
//
// enet_lwip.c - Sample WebServer Application using lwIP.
//
// Copyright (c) 2013-2017 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.4.178 of the EK-TM4C129EXL Firmware Package.
//
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/flash.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "utils/locator.h"
#include "utils/lwiplib.h"
#include "utils/ustdlib.h"
#include "utils/uartstdio.h"
#include "httpserver_raw/httpd.h"
#include "drivers/pinout.h"
#include "drivers/CFAF128128B0145T.h"
#include "webserv_appl_fcns.h"
#include "string.h"
#include "I2C_LightSens.h"

//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>Ethernet with lwIP (enet_lwip)</h1>
//!
//! This example application demonstrates the operation of the Tiva
//! Ethernet controller using the lwIP TCP/IP Stack.  DHCP is used to obtain
//! an Ethernet address.  If DHCP times out without obtaining an address,
//! AutoIP will be used to obtain a link-local address.  The address that is
//! selected will be shown on the UART.
//!
//! UART0, connected to the ICDI virtual COM port and running at 115,200,
//! 8-N-1, is used to display messages from this application. Use the
//! following command to re-build the any file system files that change.
//!
//!     ../../../../tools/bin/makefsfile -i fs -o enet_fsdata.h -r -h -q
//!
//! For additional details on lwIP, refer to the lwIP web page at:
//! http://savannah.nongnu.org/projects/lwip/
//
//*****************************************************************************

//*****************************************************************************
//
// Defines for setting up the system clock.
//
//*****************************************************************************
#define SYSTICKHZ               100
#define SYSTICKMS               (1000 / SYSTICKHZ)

//*****************************************************************************
//
// Interrupt priority definitions.  The top 3 bits of these values are
// significant with lower values indicating higher priority interrupts.
//
//*****************************************************************************
#define SYSTICK_INT_PRIORITY    0x80
#define ETHERNET_INT_PRIORITY   0xC0

//*****************************************************************************
//
// The current IP address.
//
//*****************************************************************************
uint32_t g_ui32IPAddress;

//*****************************************************************************
//
// The current MAC address
//
//*****************************************************************************
uint32_t ui32User0, ui32User1;

//*****************************************************************************
//
// The system clock frequency.
//
//*****************************************************************************
uint32_t g_ui32SysClock;

//*****************************************************************************
//
//  Light sensor flag to signal that data should be read
//
//*****************************************************************************
extern uint8_t g_ui8_getLightSensData;

//*****************************************************************************
//
//  Measured data of the light sensor
//
//*****************************************************************************
uint32_t ui32_LUXMeasData = 0;

//*****************************************************************************
//
// Volatile global flag to manage LED blinking, since it is used in interrupt
// and main application.  The LED blinks at the rate of SYSTICKHZ.
//
//*****************************************************************************
volatile bool g_bLED;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

//*****************************************************************************
// Display an lwIP type IP Address and device MAC-Address
//*****************************************************************************
void
DisplayIPnMACAddress(uint32_t ui32Addr)
{
    uint8_t ui8_count = 0;
    char ui8_IpBuffer[15];
    char ui8_IpBuffer2[15];
    char ui8_MACBuffer[20];
    char ui8_MACBuffer2[20];

    /* Convert the IP Address into a string */
    convertINTtoCHAR((ui32Addr&0xff), ui8_IpBuffer);
    for(ui8_count = 1; ui8_count < 4; ui8_count++)
    {
        strcat(ui8_IpBuffer, ".");
        convertINTtoCHAR(((ui32Addr>>(8*ui8_count))&0xff), ui8_IpBuffer2);
        strcat(ui8_IpBuffer, ui8_IpBuffer2);
    }


    /* Convert the MAC Address into a string (output decimal not hex)*/
    convertINTtoCHAR((ui32User0&0xff), ui8_MACBuffer);
    for(ui8_count = 1; ui8_count < 3; ui8_count++)
    {
        strcat(ui8_MACBuffer, ".");
        convertINTtoCHAR(((ui32User0>>(8*ui8_count))&0xff), ui8_MACBuffer2);
        strcat(ui8_MACBuffer, ui8_MACBuffer2);
    }

    strcat(ui8_MACBuffer, ".");
    convertINTtoCHAR((ui32User1&0xff), ui8_MACBuffer2);
    strcat(ui8_MACBuffer, ui8_MACBuffer2);
    for(ui8_count = 1; ui8_count < 3; ui8_count++)
    {
        strcat(ui8_MACBuffer, ".");
        convertINTtoCHAR(((ui32User1>>(8*ui8_count))&0xff), ui8_MACBuffer2);
        strcat(ui8_MACBuffer, ui8_MACBuffer2);
    }
    /* Display the IP-Adress */
    WriteDisplay(ui8_IpBuffer, ui8_MACBuffer, CFAF128128B0145T_color_green, CMD_DISPLAY_IP_MAC);
}

//*****************************************************************************
//
// Required by lwIP library to support any host-related timer functions.
//
//*****************************************************************************
void
lwIPHostTimerHandler(void)
{
    uint32_t ui32NewIPAddress;

    //
    // Get the current IP address.
    //
    ui32NewIPAddress = lwIPLocalIPAddrGet();

    //
    // See if the IP address has changed.
    //
    if(ui32NewIPAddress != g_ui32IPAddress)
    {
        //
        // See if there is an IP address assigned.
        //
        if(ui32NewIPAddress == 0xffffffff)
        {
            //
            // Indicate that there is no link.
            //
            WriteDisplay("Waiting for link", 0, CFAF128128B0145T_color_white, 0);
        }
        else if(ui32NewIPAddress == 0)
        {
            //
            // There is no IP address, so indicate that the DHCP process is
            // running.
            //
            WriteDisplay("Get IP address", 0, CFAF128128B0145T_color_white, 0);
        }
        else
        {
            //
            // Display the new IP address.
            //
            WriteDisplay("Open a browser and", 0, CFAF128128B0145T_color_green, 0);
            WriteDisplay("enter IP address", 0, CFAF128128B0145T_color_green, 0);
            DisplayIPnMACAddress(ui32NewIPAddress);
        }

        //
        // Save the new IP address.
        //
        g_ui32IPAddress = ui32NewIPAddress;
    }

    //
    // If there is not an IP address.
    //
    if((ui32NewIPAddress == 0) || (ui32NewIPAddress == 0xffffffff))
    {
        //
        // Do nothing and keep waiting.
        //
    }
}

//*****************************************************************************
//
// The interrupt handler for the SysTick interrupt.
//
//*****************************************************************************
void
SysTickIntHandler(void)
{
    //
    // Call the lwIP timer handler.
    //
    lwIPTimer(SYSTICKMS);

    //
    // Tell the application to change the state of the LED (in other words
    // blink).
    //
    g_bLED = true;
}

//*****************************************************************************
//
// This example demonstrates the use of the Ethernet Controller.
//
//*****************************************************************************
int
main(void)
{
    uint8_t pui8MACArray[8];

    //
    // Make sure the main oscillator is enabled because this is required by
    // the PHY.  The system must have a 25MHz crystal attached to the OSC
    // pins. The SYSCTL_MOSC_HIGHFREQ parameter is used when the crystal
    // frequency is 10MHz or higher.
    //
    SysCtlMOSCConfigSet(SYSCTL_MOSC_HIGHFREQ);

    //
    // Run from the PLL at 120 MHz.
    //
    g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                             SYSCTL_OSC_MAIN |
                                             SYSCTL_USE_PLL |
                                             SYSCTL_CFG_VCO_480), 120000000);
    //
    // Configure the device pins.
    //
    PinoutSet(true, false);

    //
    // Configure UART and the Display
    //
    UARTStdioConfig(0, 115200, g_ui32SysClock);
    CFAF128128B0145T_init(1, g_ui32SysClock, 20000000);          /* Use boostpack1, with 2MHz-SPI clock */

    //
    // Clear the terminal and print banner.
    //
    WriteDisplay("Webserver Example", 0, CFAF128128B0145T_color_white, 0);


    //
    // Configure Port N1 for as an output for the animation LED.
    //
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);

    //
    // Initialize LED to OFF (0)
    //
    MAP_GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, ~GPIO_PIN_1);

    //
    // Configure SysTick for a periodic interrupt.
    //
    MAP_SysTickPeriodSet(g_ui32SysClock / SYSTICKHZ);
    MAP_SysTickEnable();
    MAP_SysTickIntEnable();

    //
    // Configure the hardware MAC address for Ethernet Controller filtering of
    // incoming packets.  The MAC address will be stored in the non-volatile
    // USER0 and USER1 registers.
    //
    MAP_FlashUserGet(&ui32User0, &ui32User1);
    if((ui32User0 == 0xffffffff) || (ui32User1 == 0xffffffff))
    {
        //
        // We should never get here.  This is an error if the MAC address has
        // not been programmed into the device.  Exit the program.
        // Let the user know there is no MAC address
        //
        UARTprintf("No MAC programmed!\n");
        while(1)
        {
        }
    }

    //
    // Convert the 24/24 split MAC address from NV ram into a 32/16 split MAC
    // address needed to program the hardware registers, then program the MAC
    // address into the Ethernet Controller registers.
    //
    pui8MACArray[0] = ((ui32User0 >>  0) & 0xff);
    pui8MACArray[1] = ((ui32User0 >>  8) & 0xff);
    pui8MACArray[2] = ((ui32User0 >> 16) & 0xff);
    pui8MACArray[3] = ((ui32User1 >>  0) & 0xff);
    pui8MACArray[4] = ((ui32User1 >>  8) & 0xff);
    pui8MACArray[5] = ((ui32User1 >> 16) & 0xff);


    //
    // Initialize the lwIP library, using DHCP.
    //
    lwIPInit(g_ui32SysClock, pui8MACArray, 0, 0, 0, IPADDR_USE_DHCP);

    //
    // Setup the device locator service.
    //
    LocatorInit();
    LocatorMACAddrSet(pui8MACArray);
    LocatorAppTitleSet("EK-TM4C1294XL enet_io");

    //
    // Initialize a sample httpd server.
    //
    httpd_init();

    //
    // Set the interrupt priorities.  We set the SysTick interrupt to a higher
    // priority than the Ethernet interrupt to ensure that the file system
    // tick is processed if SysTick occurs while the Ethernet handler is being
    // processed.  This is very likely since all the TCP/IP and HTTP work is
    // done in the context of the Ethernet interrupt.
    //
    MAP_IntPrioritySet(INT_EMAC0, ETHERNET_INT_PRIORITY);
    MAP_IntPrioritySet(FAULT_SYSTICK, SYSTICK_INT_PRIORITY);

    /* initialize the I2C interface for light sensor*/
    InitOPT3001(g_ui32SysClock);

    /* initialize timer used for SD-Communication */
    InitTimer0Int();

    /* initialize timer used for Sensor data conversion */
    InitTimer1Int();

    /* mount the logical drive */
    fs_init();

    //
    // Loop forever, processing the LED blinking and the read of the sensor data
    // All the work is done in interrupt handlers.
    //
    while (1)
    {
        /* Wait till the SysTick Interrupt indicates to change the state of the LED */
        if (g_bLED != false)
        {
            /* Clear the flag */
            g_bLED = false;

            /* Toggle the LED */
            MAP_GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1,
                             (MAP_GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_1) ^
                             GPIO_PIN_1));
        }

        /* Read the Sensor data if flag is set */
        if (g_ui8_getLightSensData)
        {
            uint8_t ui8_ConvCheck;
            ui8_ConvCheck = OPT3001CheckConv();

            /* if conversion was successful */
            if (ui8_ConvCheck & 0b10000000)
            {
                uint32_t ui32_MeasdatBuffer;

                ui32_MeasdatBuffer = (int)OPT3001ReadData();
                /* sensor data is logical (LUX < MAX_INTENSITY) */
                if (ui32_MeasdatBuffer < MAX_INTENSITY)
                {
                    ui32_LUXMeasData = ui32_MeasdatBuffer;
                }
            }

            g_ui8_getLightSensData = 0;
        }
    }
}
