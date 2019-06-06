//*****************************************************************************
//
// enet_fs.c - File System Processing for lwIP Web Server Apps.
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
#include <string.h>
#include "inc/hw_memmap.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/ssi.h"
#include "driverlib/gpio.h"
#include "utils/lwiplib.h"
#include "utils/ustdlib.h"
#include "httpserver_raw/httpd.h"
#include "httpserver_raw/fs.h"
#include "httpserver_raw/fsdata.h"
#include "fatfs\src\ff.h"
#include "fatfs\src\diskio.h"
#include "drivers/CFAF128128B0145T.h"
#include "webserv_appl_fcns.h"
#include "utils/ustdlib.h"
#include "utils/uartstdio.h"
#include "webserv_appl_fcns.h"

//*****************************************************************************
//
// The following are data structures used by FatFs.
//
//*****************************************************************************
static FATFS g_sFatFs;

//*****************************************************************************
//
//  Measured data of the light sensor
//
//*****************************************************************************
extern uint32_t ui32_LUXMeasData;
//*****************************************************************************
//
// The number of milliseconds that has passed since the last disk_timerproc()
// call.
//
//*****************************************************************************
static uint32_t ui32TickCounter = 0;

//*****************************************************************************
//
// Initialize the file system.
//
//*****************************************************************************
void fs_init(void)
{
    //
    // Initialize and mount the Fat File System.
    //
    f_mount(0, &g_sFatFs);
}

//*****************************************************************************
//
// File System tick handler.
//
//*****************************************************************************
void fs_tick(uint32_t ui32TickMS)
{
    //
    // Increment the tick counter.
    //
    ui32TickCounter += ui32TickMS;

    //
    // Check to see if the FAT FS tick needs to run.
    //
    if (ui32TickCounter >= 10)
    {
        ui32TickCounter = 0;
        disk_timerproc();
    }
}

//*****************************************************************************
//
// Open a file and return a handle to the file, if found. Or return the measured
// data of the light sensor. Otherwise, return NULL.
//
//*****************************************************************************
struct fs_file *
fs_open(const char *pcName)
{
    //const struct fsdata_file *psTree;
    struct fs_file *psFile = NULL;
    char FilNamebuf[30];
    FIL *psFatFile = NULL;
    FRESULT fresult = FR_OK;
    DSTATUS DriveStatus;

    //
    // Allocate memory for the file system structure.
    //
    psFile = mem_malloc(sizeof(struct fs_file));
    if (psFile == NULL)
    {
        return (NULL);
    }

    if (ustrncmp(pcName, "/get_data", 9) == 0)
    {
        static char pcBuf[5];

        convertINTtoCHAR(ui32_LUXMeasData, pcBuf);

        psFile->data = pcBuf;
        psFile->len = strlen(pcBuf);
        psFile->index = psFile->len;
        psFile->pextension = NULL;
        return (psFile);
    }
    // TODO: Compare URI here

    else
    {

        //
        // Allocate memory for the Fat File system handle.
        //
        psFatFile = mem_malloc(sizeof(FIL));
        if (psFatFile == NULL)
        {
            mem_free(psFile);
            return (NULL);
        }

        //
        // Attempt to open the file on the Fat File System.
        //

        /* todo: Implementieren der funktion f_open*/

        if (fresult == FR_OK)
        {
            strcpy(FilNamebuf, "Valid");
            WriteDisplay(strcat(FilNamebuf, pcName), 0,
                         CFAF128128B0145T_color_white, 0);
            psFile->data = NULL;
            psFile->len = 0;
            psFile->index = 0;
            psFile->pextension = psFatFile;
            return (psFile);
        }

        strcpy(FilNamebuf, "Invalid");
        WriteDisplay(strcat(FilNamebuf, pcName), 0,
                     CFAF128128B0145T_color_white, 0);

        //
        // If we get here, we failed to find the file on the Fat File System,
        // so free up the Fat File system handle/object.
        //
        mem_free(psFatFile);
        mem_free(psFile);
        return (NULL);
    }
}

//*****************************************************************************
//
// Close an opened file designated by the handle.
//
//*****************************************************************************
void fs_close(struct fs_file *psFile)
{
    //
    // Close the file
    //
    /* todo:implementieren der Funktion f_close */
    //
    // If a Fat file was opened, free its object.
    //
    if (psFile->pextension)
    {
        mem_free(psFile->pextension);
    }
    //
    // Free the main file system object.
    //
    mem_free(psFile);
}

//*****************************************************************************
//
// Read the next chunck of data from the file.  Return the count of data
// that was read.  Return 0 if no data is currently available.  Return
// a -1 if at the end of file.
//
//*****************************************************************************
int fs_read(struct fs_file *psFile, char *pcBuffer, int iCount)
{
    int iAvailable;
    UINT uiBytesRead;
    FRESULT fresult;

    //
    // Check to see if a Fat File was opened and process it.
    //
    if (psFile->pextension)
    {
        //
        // Read the data.
        //

        /*todo: Implementieren der f_read funktion*/

        if ((fresult != FR_OK) || (uiBytesRead == 0))
        {
            return (-1);
        }
        return ((int) uiBytesRead);
    }

    //
    // Check to see if more data is available.
    //
    if (psFile->index == psFile->len)
    {
        //
        // There is no remaining data.  Return a -1 for EOF indication.
        //
        return (-1);
    }

    //
    // Determine how much data we can copy.  The minimum of the 'iCount'
    // parameter or the available data in the file system buffer.
    //
    iAvailable = psFile->len - psFile->index;
    if (iAvailable > iCount)
    {
        iAvailable = iCount;
    }

    //
    // Copy the data.
    //
    memcpy(pcBuffer, psFile->data + iAvailable, iAvailable);
    psFile->index += iAvailable;

    //
    // Return the count of data that we copied.
    //
    return (iAvailable);
}

//*****************************************************************************
//
// Determine the number of bytes left to read from the file.
//
//*****************************************************************************
int fs_bytes_left(struct fs_file *psFile)
{
    //
    // Check to see if a Fat File was opened and process it.
    //
    if (psFile->pextension)
    {
        //
        // Return the number of bytes left to be read from the Fat File.
        //
        return (f_size((FIL * )psFile->pextension)
                - f_tell((FIL * )psFile->pextension));
    }

    //
    // Return the number of bytes left to be read from this file.
    //
    return (psFile->len - psFile->index);
}
