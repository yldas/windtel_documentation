/*
 * WindTel_DataAcquisition.h
 *
 *  Created on: Apr 21, 2019
 *      Author: Luis O. Vega Maisonet
 */

#ifndef WINDTEL_DATAACQUISITION_H_
#define WINDTEL_DATAACQUISITION_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char pressureCommand[32] = "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0";
char dynamicCommand[14];
char balanceCommand[8];
char durationTimeBuffer[6];
char results_for_transmission[4];
int results_for_transmission_index = 0;
//Converts the integer parameter num to a string, where parameter digits dictate the amount of digits it has
void convertToChars(int num, int digits){
    results_for_transmission_index = 0;
    int temp1 = num;
    int multiple = 1;
    int i = 0;
    int count = 0;
    if(num == 0){
        for(; i < digits; i++){
            results_for_transmission[results_for_transmission_index] = '0';
            results_for_transmission_index++;
        }
    }else{
        while(temp1 != 0){
            multiple *= 10;
            temp1 /= 10;
            count++;
        }
        temp1 = num;
        if(temp1 < 0){
            temp1 *= -1;
        }
        int digit;
        int j = 0;
        while(multiple > 1 || j < digits){
            if((digits - count) > j){
                results_for_transmission[results_for_transmission_index] = '0';
                results_for_transmission_index++;
            }else{
                multiple/= 10;
                digit = temp1/multiple;
                if(digit == 0){
                    results_for_transmission[results_for_transmission_index] = '0';
                    results_for_transmission_index++;
                }else{
                    results_for_transmission[results_for_transmission_index] = digit+'0';
                    results_for_transmission_index++;
                }
                temp1 %= multiple;
            }
            j++;
        }
    }
}

/* Converts a float number to a char array. */
void convertFloatToChars(float number){
    int integer = number;
    float temp = number;
    temp -= integer;
    temp *= 100;
    int fraction = temp;
    convertToChars(integer, 6);
    results_for_transmission[results_for_transmission_index] = '@';
    results_for_transmission_index++;
    convertToChars(fraction, 2);
}
void acquirePressureModuleData(int selectedPressureSensors[],int numberOfSamples,char durationBuff[5]){
    char sampleBuff[5];
    if(selectedPressureSensors[1] || selectedPressureSensors[11]){
        int i = 0;
        for(i = 0;i<20;i++)
            pressureCommand[i] = '1';
    }
    else{
        if(selectedPressureSensors[2])
            pressureCommand[0] = '1';
        if(selectedPressureSensors[3])
            pressureCommand[2] = '1';
        if(selectedPressureSensors[4])
            pressureCommand[4] = '1';
        if(selectedPressureSensors[5])
            pressureCommand[6] = '1';
        if(selectedPressureSensors[6])
            pressureCommand[8] = '1';
        if(selectedPressureSensors[7])
            pressureCommand[10] = '1';
        if(selectedPressureSensors[8])
            pressureCommand[12] = '1';
        if(selectedPressureSensors[9])
            pressureCommand[14] = '1';
    }
    tostring(sampleBuff,numberOfSamples);
    transmitMessageToPressureModule("\nacquireData\n");
    transmitMessageToPressureModule(durationBuff);
    transmitMessageToPressureModule(",");
    transmitMessageToPressureModule(sampleBuff);
    transmitMessageToPressureModule(",");
    transmitMessageToPressureModule(pressureCommand);
    transmitMessageToPressureModule("\n");
    Delay(100);
    UART2_Transmitter_Mode();
    UART2_Receiver_Mode();
}

void acquireDynamicModuleData(char tempBuff[5],char humBuff[5], char speedBuff[5],char durationBuff[5]){

    transmitMessageToDynamicModule("\nacquireData\n");
    transmitMessageToDynamicModule(durationBuff);
    transmitMessageToDynamicModule(",");
    transmitMessageToDynamicModule(tempBuff);
    transmitMessageToDynamicModule(",");
    transmitMessageToDynamicModule(humBuff);
    transmitMessageToDynamicModule(",");
    transmitMessageToDynamicModule(speedBuff);
    transmitMessageToDynamicModule("\n");
    Delay(100);
    UART0_Transmitter_Mode();
    UART0_Receiver_Mode();
}

int acquireSpeedData(){
    transmitMessageToDynamicModule("\nspeedControl\n");
    Delay(100);
    UART0_Transmitter_Mode();
    UART0_Receiver_Mode();
    return 1;
}
void acquireBalanceModuleData(int numberOfSamples,int acquiredMeasurementDuration, int rodLength){
    convertToChars(numberOfSamples, 2);
    char balanceCommand[8];
    balanceCommand[0] = 'S';
    balanceCommand[1] = 'N';
    balanceCommand[2] = results_for_transmission[0];
    balanceCommand[3] = results_for_transmission[1];
    convertToChars(rodLength, 3);
    balanceCommand[4] = results_for_transmission[0];
    balanceCommand[5] = results_for_transmission[1];
    balanceCommand[6] = results_for_transmission[2];
    balanceCommand[7] = '\n';

    transmitMessageToBalanceModule(balanceCommand);
    Delay(100);
    UART1_Transmitter_Mode();
    UART1_Receiver_Mode();
}

int toInt(char a[]) {
    int c, sign, offset, n;

    if (a[0] == '-') {  // Handle negative integers
        sign = -1;
    }

    if (sign == -1) {  // Set starting position to convert
        offset = 1;
    }
    else {
        offset = 0;
    }

    n = 0;

    for (c = offset; a[c] != '\0'; c++) {
        n = n * 10 + a[c] - '0';
    }

    if (sign == -1) {
        n = -n;
    }

    return n;
}
#endif /* WINDTEL_DATAACQUISITION_H_ */
