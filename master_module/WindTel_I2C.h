/*
 * WindTel_I2C.h
 *
 *  Created on: Mar 29, 2019
 *      Author: Luis Obed Vega Maisonet
 */
#include "DriverLibFiles/driverlib.h"

#ifndef WINDTEL_I2C_H_
#define WINDTEL_I2C_H_

void I2C_Init(const I2C_REGISTER,const I2C_CONFIG,const SLAVE_ADDRESS, const I2C_INT_REGISTER, uint8_t* RXData, const NUM_OF_REC_BYTES){

    memset(RXData, 0x00, NUM_OF_REC_BYTES);
    /* Initializing I2C Master to SMCLK at 100khz with no autostop */
    MAP_I2C_initMaster(I2C_REGISTER, I2C_CONFIG);

    /* Specify slave address */
    MAP_I2C_setSlaveAddress(I2C_REGISTER, SLAVE_ADDRESS);

    /* Enable I2C Module to start operations */
    MAP_I2C_enableModule(I2C_REGISTER);

    MAP_Interrupt_enableInterrupt(I2C_INT_REGISTER);
}

void I2C_StartCommunication(const I2C_REGISTER, uint8_t* TXData){
    /* Making sure the last transaction has been completely sent out */
    while (MAP_I2C_masterIsStopSent(I2C_REGISTER));

    MAP_Interrupt_enableSleepOnIsrExit();

    /* Send start and the first byte of the transmit buffer. */
    MAP_I2C_masterSendMultiByteStart(I2C_REGISTER, TXData[0]);
    Delay(1000);

    /* Sent the first byte, now we need to initiate the read */
    MAP_I2C_masterReceiveStart(I2C_REGISTER);
    MAP_I2C_enableInterrupt(I2C_REGISTER, EUSCI_B_I2C_RECEIVE_INTERRUPT0);
}

#endif /* WINDTEL_I2C_H_ */
