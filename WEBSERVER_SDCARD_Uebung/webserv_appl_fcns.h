/*
 * webserv_appl_fcns.h
 *
 *  Created on: 12.05.2019
 *      Author: Grotz
 */

#include <stdint.h>

#ifndef WEBSERV_APPL_FCNS_H_
#define WEBSERV_APPL_FCNS_H_

/*********************************************************************************
 *  defines, variables and prototypes
 *********************************************************************************/
#define CMD_DISPLAY_IP_MAC    0x01

extern uint8_t g_ui8_getLightSensData;

void InitTimer0Int(void);
void InitTimer1Int(void);
void WriteDisplay(const char* IP_Address, const char* MAC_Address, uint16_t textColor, uint8_t CMD);
void convertINTtoCHAR(int n, char string[]);
void reverse(char string[]);

#endif /* WEBSERV_APPL_FCNS_H_ */
