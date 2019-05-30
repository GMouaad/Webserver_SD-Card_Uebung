/*********************************************************************************
 * \file   I2C_LightSens.c
 * \author Daniel Grotz
 * \date   12.05.2019
 *
 *      This file contains the functions to init the I2C-Interface for the
 *      communication with the light-sensor OPT3001 (http://www.ti.com/product/OPT3001)
 *      and the functions for the configuration and to read the measured data.
 *
 *      This file contains the following functions:
 *      - void InitOPT3001(uint32_t ui32SysClkFreq)
 *      - uint8_t OPT3001CheckConv()
 *      - float OPT3001ReadData()
 *
 *********************************************************************************/


/*********************************************************************************
 *  Includes
 *********************************************************************************/
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "I2C_LightSens.h"
#include "driverlib/interrupt.h"

/*********************************************************************************
 * \brief Init of Light Sensor
 *
 *     Initialize the LightSensor the used I2C-Pins for the communication.
 * \param   uint32_t ui32SysClkFreq   System clock for I2C
 * \return  void
 *********************************************************************************/
void InitOPT3001(uint32_t ui32SysClkFreq)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);                 // enable I2C0
  //  SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);                // enable GPIO port B for I2C
  //  SysCtlPeripheralReset(SYSCTL_PERIPH_GPIOB);


    // Configure the GPIO settings for the I2C pins.  This function also gives
    // The pins are assigned as follows:
    //      PB2 - I2C0 SCL
    //      PB3 - I2C0 SDA
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);


    // Initialize I2C master peripheral and set bus speed
    I2CMasterInitExpClk(I2C0_BASE, ui32SysClkFreq, true);               // true = 400 kBit/s (fast mode), false = 100 kBit/s

    // Configuration for the light sensor
    I2CMasterSlaveAddrSet(I2C0_BASE, SLAVE_ADDRESS, CMD_WRITE);         // select the I2C address of the light sensor
    I2CMasterDataPut(I2C0_BASE, REG_CONFIG);                            // send addr. of light sensor config. register

    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);       // Initiate send of data from the MCU
    SysCtlDelay(500);                                                   // wait a bit
    while (I2CMasterBusy(I2C0_BASE))
    SysCtlDelay(52);

    // Send first sensor config data byte --> I2C transmit MSB first
    // bit [15:12]: set light sensor in automatic full-scale setting mode
    // bit [11]: set the light conversion time: 0 = 100 ms, 1 = 800 ms
    // bit [10:9]: set the conversion mode: 00 = shut down, 01 = single-shot, 10/11 = Continuous conversions
    // bit [8] (read only): overflow flag
    I2CMasterDataPut(I2C0_BASE, 0b11001100);


    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);        //send next data that was just placed into FIFO
    SysCtlDelay(500); // wait a bit
    while (I2CMasterBusy(I2C0_BASE))
    SysCtlDelay(52);

    // Send second sensor config data byte
    // bit[7]: Conversation ready field (read only)
    // bit[6]: Flag high field (read only)
    // bit[5]: Flag low field (read only)
    // bit[4]: Latch field, 0 = continuously mode, 1 = interrupt mode
    // bit[3]: Polarity field, configure the INT pin: 0 = active low, 1 = active high
    // bit[2]: Mask exponent field, can set the exponent to 0b0000 if the full scale range is manually programmed
    // bit[1:0]: Fault count field
    I2CMasterDataPut(I2C0_BASE, 0b00010000);

    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    SysCtlDelay(500); // wait a bit
}


/*********************************************************************************
 * \brief Checks if conversion was successful
 *
 *     Checks if the light sensor has read new data. Returns the read
 *     configregister of the sonsor

 * \return  read config register of sensor
 *********************************************************************************/
uint8_t OPT3001CheckConv()
{
    uint8_t ui8_data_buffer[2];

    SysCtlDelay(12);
    while (I2CMasterBusy(I2C0_BASE))

    SysCtlDelay(52);                                                      //Bus free time between stop and start t_min = 1300ns.
                                                                          //The function is called intern three times. That means x = t_min * (120 MHz/3) = 52

    I2CMasterSlaveAddrSet(I2C0_BASE, SLAVE_ADDRESS, false);
    I2CMasterDataPut(I2C0_BASE, REG_CONFIG);                              // call config register
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    SysCtlDelay(12);
    while (I2CMasterBusy(I2C0_BASE))
    SysCtlDelay(52);

    I2CMasterSlaveAddrSet(I2C0_BASE, SLAVE_ADDRESS, CMD_READ);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    SysCtlDelay(12);
    while (I2CMasterBusy(I2C0_BASE))
    SysCtlDelay(52);

    ui8_data_buffer[0] = I2CMasterDataGet(I2C0_BASE); // read MSB

    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    SysCtlDelay(12);
    while (I2CMasterBusy(I2C0_BASE))
    SysCtlDelay(52);

    ui8_data_buffer[1] = I2CMasterDataGet(I2C0_BASE); // read LSB

    return ui8_data_buffer[1];
}

/*********************************************************************************
 * \brief Read light sensor value
 *
 *     This function reads an new value from the Light Sensor calculates an float
 *     value

 * \return  returns the measured value as float
 *********************************************************************************/
float OPT3001ReadData()
{
    //IntMasterDisable();
    uint16_t ui16_Result;               // result light intensity
    uint16_t ui16_Exponent;             // exponent value from sensor (raw data contains 4-bit exponent and a 12-bit mantissa)
    uint8_t ui8_data_buffer[2];         // raw data from sensor
    float lux;                          // calc value

    I2CMasterSlaveAddrSet(I2C0_BASE, SLAVE_ADDRESS, CMD_WRITE);
    I2CMasterDataPut(I2C0_BASE, REG_RESULT);        //call result register
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    SysCtlDelay(12);
    while (I2CMasterBusy(I2C0_BASE))
    SysCtlDelay(52);

    I2CMasterSlaveAddrSet(I2C0_BASE, SLAVE_ADDRESS, CMD_READ);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    SysCtlDelay(12);
    while (I2CMasterBusy(I2C0_BASE))
    SysCtlDelay(52);

    ui8_data_buffer[0] = I2CMasterDataGet(I2C0_BASE);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    SysCtlDelay(12);
    while (I2CMasterBusy(I2C0_BASE))
    SysCtlDelay(52);

    ui8_data_buffer[1] = I2CMasterDataGet(I2C0_BASE);
    ui16_Result = ui8_data_buffer[0];
    ui16_Result <<= 8;
    ui16_Result |= ui8_data_buffer[1];
    ui16_Exponent = (ui16_Result >> 12) & 0x000F;   // Exponent is saved in Bit 12-15. Mask out the exponent by '& 0x00F'
    ui16_Result = ui16_Result & 0x0FFF;             // mask out the result without exponent

    lux = ui16_Result;
    lux = 0.01 * (0x01 << ui16_Exponent) * lux;

    //IntMasterEnable();
    return lux; // return the value as an float
}
