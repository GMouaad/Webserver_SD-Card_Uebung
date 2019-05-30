/*
 * I2C_LightSens.h
 *
 *  Created on: 11.04.2019
 *      Author: Grotz
 */

#ifndef I2C_LIGHTSENS_H_
#define I2C_LIGHTSENS_H_

/*********************************************************************************
 *  defines, variables and prototypes
 *********************************************************************************/
#define SLAVE_ADDRESS       0x44        //Slave Address of the OPT3001
#define MAX_INTENSITY       10000       //Max Lightintesinty
#define REG_CONFIG          0x01
#define REG_RESULT          0x00
#define CMD_WRITE           false
#define CMD_READ            true

extern uint32_t ui32_LUXMeasData;

void InitOPT3001(uint32_t ui32SysClkFreq);
uint8_t OPT3001CheckConv();
float OPT3001ReadData();
uint8_t OPT3001WriteData();

#endif /* I2C_LIGHTSENS_H_ */
