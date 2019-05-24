/*
 * WindTel_Graphic.h
 *
 *  Created on: Mar 16, 2019
 *      Author: Luis O. Vega Maisonet
 */
/* Standard Includes */
#include <stdint.h>
#include "DriverLibFiles/driverlib.h"
/* GrLib Includes */
#include "DriverLibFiles/grlib.h"
#include "DriverLibFiles/button.h"
#include "DriverLibFiles/imageButton.h"
#include "DriverLibFiles/radioButton.h"
#include "DriverLibFiles/checkbox.h"
#include "LcdDriver/kitronix320x240x16_ssd2119_spi.h"
#include "images/images.h"
#include "DriverLibFiles/touch_P401R.h"

#ifndef WINDTEL_GRAPHIC_H_
#define WINDTEL_GRAPHIC_H_
int count = 0;
int time = 1000;
char windTunnelVelocity[5];
char rod_LengthBalance[7] = {'1','c','m',' ',' '};
char experimentDuration[10] = {'5','s','e','c',' ',' '};
char totalExperimentDuration[10] = {'5','s','e','c',' ',' '};
char sampleBuff[5] = {'1',' ',' ',' ',' '};
char acquisitionTimeBuffer[10] = {' ',' ',' ',' ',' ',' ',' ',' '};
int numberOfExperiments = 0;
char numberOfExperimentBuffer[2] = {'0',' '};
int min = 0;
int sampleCounter[5] = {1,1,1,1,1};
int measurmentsGoingToSample = 0;
int start_cursor,lastNumOption = 60;
int sampleMeasureSelection[5] = {0,0,0,0,0};
int decimal = 0;
char minChar = '0';
char decimalChar1 = '0';
char decimalChar2 = '0';
char *firstOption;
char *lastOption;

char *pressurePointer;
char *balancePointer;
char *temperaturePointer;
char *relativeHumidityPointer;
char *speedPointer = "42.141mph";
int numPressureSamples = 1;
int numForceSamples = 1;
int numTemperatureSamples = 1;
int numHumiditySamples = 1;
int numWindSpeedSamples = 1;
//Delay function takes 16 bit number (in milliseconds) and performs a delay
void Delay(uint32_t msec){
    uint32_t i = 0;
    //Converts the milliseconds to seconds and uses the system's clock speed for the delay
    uint32_t time = (msec / 1000) * (SYSTEM_CLOCK_SPEED / 15);

    //Empty loop for the delay
    for(i = 0; i < time; i++)
    {
        ;
    }
}

//Converts an integer number to an array of characters, also known as a string
void tostring(char str[], int num){
    int i, rem, len = 0, n;

    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

//toPSI(char str[]){
//
//}
//Initialization procedure of the display
void boardInit(){
    FPU_enableModule();
}

//Initializes the clock of the display
void clockInit(void){
    /* 2 flash wait states, VCORE = 1, running off DC-DC, 48 MHz */
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);
    PCM_setPowerState(PCM_AM_DCDC_VCORE1);
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    CS_setDCOFrequency(48000000);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, 1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, 1);
    CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, 1);
}

void drawMainMenu(Graphics_Context g_sContext, Graphics_Rectangle rect){
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_clearDisplay(&g_sContext);
    rect.xMax = 315;
    rect.xMin = 8;
    rect.yMax = 80;
    rect.yMin = 60;
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(&g_sContext,"Wind Tunnel Telemetry System", AUTO_STRING_LENGTH, 8,10,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Main Menu", AUTO_STRING_LENGTH, 8,35,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"1. System Reset", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"2. Perform Experiment", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"3. Speed Control", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"4. Use WindTel Application", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
}

void drawSpeedControlMenu(Graphics_Context g_sContext, Graphics_Rectangle rect){
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_clearDisplay(&g_sContext);
    rect.xMax = 315;
    rect.xMin = 8;
    rect.yMax = 80;
    rect.yMin = 60;
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(&g_sContext,"Wind Tunnel Telemetry System", AUTO_STRING_LENGTH, 8,10,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Speed Control Menu", AUTO_STRING_LENGTH, 8,35,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"1. Turn On Wind Tunnel Fan", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"2. Turn Off Wind Tunnel Fan", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"3. Return to Main Menu", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Current Wind Speed:", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
    char balanceCommand[3] = "WY\n";
    P1->OUT |= BIT5;
    transmitMessageToBalanceModule(balanceCommand);
    Delay(100);
    UART1_Transmitter_Mode();
    P1->OUT &= ~BIT5;
}

void updateWindSpeed(Graphics_Context g_sContext,char inBytes[]){
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(&g_sContext,inBytes, AUTO_STRING_LENGTH, 212,135,GRAPHICS_OPAQUE_TEXT);
    Graphics_drawString(&g_sContext,"mph    ", AUTO_STRING_LENGTH, 250,135,GRAPHICS_OPAQUE_TEXT);
}
void setAtmosphericPressure(char *pressureRXData){
    pressurePointer = pressureRXData;
}

void setBalanceForces(char *balanceForcesRXData){
    balancePointer = balanceForcesRXData;
}

void updateTimeDuration(Graphics_Context g_sContext,int secs,Graphics_Rectangle rect){
    if(secs < 60){
        if(secs<10){
            tostring(experimentDuration,secs);
            experimentDuration[1] = 's';
            experimentDuration[2] = 'e';
            experimentDuration[3] = 'c';
            experimentDuration[4] = ' ';
            experimentDuration[5] = ' ';
        }
        else{
            tostring(experimentDuration,secs);
            experimentDuration[2] = 's';
            experimentDuration[3] = 'e';
            experimentDuration[4] = 'c';
            experimentDuration[5] = ' ';
            experimentDuration[6] = ' ';
        }
    }
    else{
        min = secs/60;
        if(min == 1)
            decimal = secs - 60;
        else if(min == 2)
            decimal = secs - 120;
        else if(min == 3)
            decimal = secs - 180;
        else if(min == 4)
            decimal = secs - 240;
        else if(min == 5)
            decimal = secs - 300;
        minChar = '0' + min;
        decimalChar1 = '0' + decimal/10;
        decimalChar2 = '0' + 10*((decimal/10.0) - (decimal/10));
        experimentDuration[0] = minChar;
        experimentDuration[1] = '.';
        experimentDuration[2] = decimalChar1;
        experimentDuration[3] = decimalChar2;
        experimentDuration[4] = 'm';
        experimentDuration[5] = 'i';
        experimentDuration[6] = 'n';
        experimentDuration[7] = ' ';
        experimentDuration[8] = ' ';
    }
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    rect.xMax = 315;
    rect.xMin = 8;
    rect.yMax = 105;
    rect.yMin = 85;
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    if(secs != 5)
        Graphics_drawString(&g_sContext,experimentDuration, AUTO_STRING_LENGTH,140,85,GRAPHICS_TRANSPARENT_TEXT);
    else
        Graphics_drawString(&g_sContext,"5sec", AUTO_STRING_LENGTH,140,85,GRAPHICS_TRANSPARENT_TEXT);

    int i = 0;
    for(i = 0; i<10;i++)
        totalExperimentDuration[i] = experimentDuration[i];
}

void clearNumberOfExperiments(){
    numberOfExperiments = 0;
    numberOfExperimentBuffer[0] = '0';
    numberOfExperimentBuffer[1] = ' ';
}
void drawPerformExperimentMenu(Graphics_Context g_sContext, Graphics_Rectangle rect){
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_clearDisplay(&g_sContext);
    rect.xMax = 315;
    rect.xMin = 8;
    rect.yMax = 105;
    rect.yMin = 85;
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(&g_sContext,"Wind Tunnel Telemetry System", AUTO_STRING_LENGTH, 8,10,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Perform Experiment Menu", AUTO_STRING_LENGTH, 8,35,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"1.Enter the rod length:", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,rod_LengthBalance, AUTO_STRING_LENGTH,140,85,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"2.Confirm the selected length", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"3.Return to Main Menu", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
}

void updateRodLength(Graphics_Context g_sContext,int rod_Length,Graphics_Rectangle rect){
    tostring(rod_LengthBalance,rod_Length);
    if(rod_Length<10){
        rod_LengthBalance[1] = 'c';
        rod_LengthBalance[2] = 'm';
        rod_LengthBalance[3] = ' ';

    }
    else if(rod_Length<100){
        rod_LengthBalance[2] = 'c';
        rod_LengthBalance[3] = 'm';
        rod_LengthBalance[4] = ' ';

    }
    else{
        rod_LengthBalance[3] = 'c';
        rod_LengthBalance[4] = 'm';
        rod_LengthBalance[5] = ' ';
        rod_LengthBalance[6] = ' ';

    }
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    rect.xMax = 315;
    rect.xMin = 8;
    rect.yMax = 105;
    rect.yMin = 85;
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    if(rod_Length)
        Graphics_drawString(&g_sContext,rod_LengthBalance, 5,140,85,GRAPHICS_TRANSPARENT_TEXT);
    else
        Graphics_drawString(&g_sContext,"1cm", AUTO_STRING_LENGTH,140,85,GRAPHICS_TRANSPARENT_TEXT);

}

void drawParameterMenu(Graphics_Context g_sContext, Graphics_Rectangle rect){
    Graphics_clearDisplay(&g_sContext);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    rect.xMax = 315;
    rect.xMin = 8;
    rect.yMax = 80;
    rect.yMin = 60;
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(&g_sContext,"Wind Tunnel Telemetry System", AUTO_STRING_LENGTH, 8,10,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Select parameters to measure", AUTO_STRING_LENGTH, 8,35,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"1.Barometric Pressure", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"2.Wind Exerted Forces", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"3.Relative Humidity      [ ]", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"4.Temperature           [ ]", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"5.Wind Speed            [ ]", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"6.Proceed with the experiment", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"7.Return to Main Menu", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
}

void drawPressureMenu(Graphics_Context g_sContext, Graphics_Rectangle rect){
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_clearDisplay(&g_sContext);
    rect.xMax = 112;
    rect.xMin = 8;
    rect.yMax = 80;
    rect.yMin = 60;
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(&g_sContext,"Wind Tunnel Telemetry System", AUTO_STRING_LENGTH, 8,10,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Select pressure sensors", AUTO_STRING_LENGTH, 8,35,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"All Sensors [ ]", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Sensor  1 [ ]", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Sensor  2 [ ]", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Sensor  3 [ ]", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Sensor  4 [ ]", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Sensor  5 [ ]", AUTO_STRING_LENGTH, 150,85,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Sensor  6 [ ]", AUTO_STRING_LENGTH, 150,110,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Sensor  7 [ ]", AUTO_STRING_LENGTH, 150,135,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Sensor  8 [ ]", AUTO_STRING_LENGTH, 150,160,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"More Sensors", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Return to Parameter Menu", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);

}

void drawMorePressureMenu(Graphics_Context g_sContext, Graphics_Rectangle rect){
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_clearDisplay(&g_sContext);
    rect.xMax = 112;
    rect.xMin = 8;
    rect.yMax = 80;
    rect.yMin = 60;
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(&g_sContext,"Wind Tunnel Telemetry System", AUTO_STRING_LENGTH, 8,10,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Select pressure sensors", AUTO_STRING_LENGTH, 8,35,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"All Sensors [ ]", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Sensor  9 [ ]", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Sensor 10 [ ]", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Sensor 11 [ ]", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Sensor 12 [ ]", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Sensor 13 [ ]", AUTO_STRING_LENGTH, 150,85,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Sensor 14 [ ]", AUTO_STRING_LENGTH, 150,110,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Sensor 15 [ ]", AUTO_STRING_LENGTH, 150,135,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Sensor 16 [ ]", AUTO_STRING_LENGTH, 150,160,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"More Sensors", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Return to Parameter Menu", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
}

void drawForceMenu(Graphics_Context g_sContext, Graphics_Rectangle rect){
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_clearDisplay(&g_sContext);
    rect.xMax = 140;
    rect.xMin = 8;
    rect.yMax = 80;
    rect.yMin = 60;
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(&g_sContext,"Wind Tunnel Telemetry System", AUTO_STRING_LENGTH, 8,10,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Select forces", AUTO_STRING_LENGTH, 8,35,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"1.All Forces    [  ]", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"2.Drag Front   [  ]", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"3.Drag Back    [  ]", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"4.Side Right    [  ]", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"5.Side Left     [  ]", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"6.Up            [  ]", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"7.Return to Parameter Menu", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);

}

void drawExperimentDurationMenu(Graphics_Context g_sContext, Graphics_Rectangle rect){
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_clearDisplay(&g_sContext);
    rect.xMax = 315;
    rect.xMin = 8;
    rect.yMax = 105;
    rect.yMin = 85;
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(&g_sContext,"Wind Tunnel Telemetry System", AUTO_STRING_LENGTH, 8,10,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Experiment Duration Menu", AUTO_STRING_LENGTH, 8,35,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"1.Enter the experiment duration:", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,experimentDuration, AUTO_STRING_LENGTH,140,85,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"2.Confirm the selected duration", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"3.Return to Parameter Menu", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"4.Return to Main Menu", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
    int i = 0;
    for(i = 0; i<10;i++)
        totalExperimentDuration[i] = experimentDuration[i];
}

void drawSampleMenu(Graphics_Context g_sContext, Graphics_Rectangle rect, bool measurePressure,
                    bool measureForce,bool measureTemperature, bool measureRelativeHumidity,bool measureWindSpeed){
    start_cursor = 60;
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_clearDisplay(&g_sContext);
    rect.xMax = 200;
    rect.xMin = 8;
    rect.yMax = 80;
    rect.yMin = 60;
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(&g_sContext,"Wind Tunnel Telemetry System", AUTO_STRING_LENGTH, 8,10,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Select Samples", AUTO_STRING_LENGTH, 8,35,GRAPHICS_TRANSPARENT_TEXT);
    int sampleCounterIndex = 0;
    numPressureSamples = 1;
    numForceSamples = 1;
    numTemperatureSamples = 1;
    numHumiditySamples = 1;
    numWindSpeedSamples = 1;
    if(measurePressure){
        Graphics_drawString(&g_sContext,"Pressure:", AUTO_STRING_LENGTH, 8,start_cursor,GRAPHICS_TRANSPARENT_TEXT);
        tostring(sampleBuff,sampleCounter[sampleCounterIndex]);
        Graphics_drawString(&g_sContext,sampleBuff, AUTO_STRING_LENGTH, 220,start_cursor,GRAPHICS_TRANSPARENT_TEXT);
        start_cursor += 25;
        sampleCounterIndex++;
    }
    if(measureForce){
        Graphics_drawString(&g_sContext,"Forces:", AUTO_STRING_LENGTH, 8,start_cursor,GRAPHICS_TRANSPARENT_TEXT);
        tostring(sampleBuff,sampleCounter[sampleCounterIndex]);
        Graphics_drawString(&g_sContext,sampleBuff, AUTO_STRING_LENGTH, 220,start_cursor,GRAPHICS_TRANSPARENT_TEXT);
        start_cursor += 25;
        sampleCounterIndex++;
    }
    if(measureTemperature){
        Graphics_drawString(&g_sContext,"Temperature:", AUTO_STRING_LENGTH, 8,start_cursor,GRAPHICS_TRANSPARENT_TEXT);
        tostring(sampleBuff,sampleCounter[sampleCounterIndex]);
        Graphics_drawString(&g_sContext,sampleBuff, AUTO_STRING_LENGTH, 220,start_cursor,GRAPHICS_TRANSPARENT_TEXT);
        start_cursor += 25;
        sampleCounterIndex++;
    }
    if(measureRelativeHumidity){
        Graphics_drawString(&g_sContext,"Relative Humidity:", AUTO_STRING_LENGTH, 8,start_cursor,GRAPHICS_TRANSPARENT_TEXT);
        tostring(sampleBuff,sampleCounter[sampleCounterIndex]);
        Graphics_drawString(&g_sContext,sampleBuff, AUTO_STRING_LENGTH, 220,start_cursor,GRAPHICS_TRANSPARENT_TEXT);
        start_cursor += 25;
        sampleCounterIndex++;
    }
    if(measureWindSpeed){
        Graphics_drawString(&g_sContext,"Wind Speed:", AUTO_STRING_LENGTH, 8,start_cursor,GRAPHICS_TRANSPARENT_TEXT);
        tostring(sampleBuff,sampleCounter[sampleCounterIndex]);
        Graphics_drawString(&g_sContext,sampleBuff, AUTO_STRING_LENGTH, 220,start_cursor,GRAPHICS_TRANSPARENT_TEXT);
        start_cursor += 25;
        sampleCounterIndex++;
    }
    Graphics_drawString(&g_sContext,"Confirm the selected samples", AUTO_STRING_LENGTH, 8,start_cursor,GRAPHICS_TRANSPARENT_TEXT);
    start_cursor += 25;
    Graphics_drawString(&g_sContext,"Return to Previous Menu", AUTO_STRING_LENGTH, 8,start_cursor,GRAPHICS_TRANSPARENT_TEXT);
}

void setMeasurmentsToSample(bool measurePressure,
                            bool measureForce,bool measureTemperature, bool measureRelativeHumidity,bool measureWindSpeed){
    measurmentsGoingToSample = 0;
    sampleMeasureSelection[0] = 0;
    sampleMeasureSelection[1] = 0;
    sampleMeasureSelection[2] = 0;
    sampleMeasureSelection[3] = 0;
    sampleMeasureSelection[4] = 0;

    if(measurePressure){
        measurmentsGoingToSample++;
        sampleMeasureSelection[0] = 1;
    }

    if(measureForce){
        measurmentsGoingToSample++;
        sampleMeasureSelection[1] = 1;
    }

    if(measureTemperature){
        measurmentsGoingToSample++;
        sampleMeasureSelection[2] = 1;
    }

    if(measureRelativeHumidity){
        measurmentsGoingToSample++;
        sampleMeasureSelection[3] = 1;
    }

    if(measureWindSpeed){
        measurmentsGoingToSample++;
        sampleMeasureSelection[4] = 1;
    }
}
int getMeasurmentsToSample(){
    return measurmentsGoingToSample;
}
void StopExperimentMessage(Graphics_Context g_sContext, Graphics_Rectangle rect){

    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    rect.xMax = 315;
    rect.xMin = 8;
    rect.yMax = 130;
    rect.yMin = 110;
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(&g_sContext,"1. Stop Experiment", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
    Graphics_drawString(&g_sContext,"   Acquiring Measurements...", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
}

void ViewResultsMessage(Graphics_Context g_sContext, Graphics_Rectangle rect){
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    rect.xMax = 315;
    rect.xMin = 8;
    rect.yMax = 130;
    rect.yMin = 110;
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(&g_sContext,"1. View Results", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
}

void updateAcquisitionTime(Graphics_Context g_sContext,int secs){
    if(secs < 60){
        if(secs<10){
            tostring(acquisitionTimeBuffer,secs);
            acquisitionTimeBuffer[1] = 's';
            acquisitionTimeBuffer[2] = 'e';
            acquisitionTimeBuffer[3] = 'c';
            acquisitionTimeBuffer[4] = ' ';
            acquisitionTimeBuffer[5] = ' ';
        }
        else{
            tostring(acquisitionTimeBuffer,secs);
            acquisitionTimeBuffer[2] = 's';
            acquisitionTimeBuffer[3] = 'e';
            acquisitionTimeBuffer[4] = 'c';
            acquisitionTimeBuffer[5] = ' ';
            acquisitionTimeBuffer[6] = ' ';
        }
    }
    else{
        min = secs/60;
        if(min == 1)
            decimal = secs - 60;
        else if(min == 2)
            decimal = secs - 120;
        else if(min == 3)
            decimal = secs - 180;
        else if(min == 4)
            decimal = secs - 240;
        else if(min == 5)
            decimal = secs - 300;
        minChar = '0' + min;
        decimalChar1 = '0' + decimal/10;
        decimalChar2 = '0' + 10*((decimal/10.0) - (decimal/10));
        acquisitionTimeBuffer[0] = minChar;
        acquisitionTimeBuffer[1] = '.';
        acquisitionTimeBuffer[2] = decimalChar1;
        acquisitionTimeBuffer[3] = decimalChar2;
        acquisitionTimeBuffer[4] = 'm';
        acquisitionTimeBuffer[5] = 'i';
        acquisitionTimeBuffer[6] = 'n';
        acquisitionTimeBuffer[7] = ' ';
        acquisitionTimeBuffer[8] = ' ';
    }
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    if(secs != 0)
        Graphics_drawString(&g_sContext,acquisitionTimeBuffer, AUTO_STRING_LENGTH,230,60,GRAPHICS_OPAQUE_TEXT);
    else
        Graphics_drawString(&g_sContext,"0sec", AUTO_STRING_LENGTH,230,60,GRAPHICS_OPAQUE_TEXT);
}

void displayNeedMeasurmentMessage(){
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
    Graphics_drawString(&g_sContext,"   Need To Select Parameters", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"    To Perform Experiments", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
}

void drawDiagnosticMessage(Graphics_Context g_sContext){
    Graphics_clearDisplay(&g_sContext);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
    Graphics_drawString(&g_sContext,"    Verifying Communication", AUTO_STRING_LENGTH, 8,75,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"           with Modules", AUTO_STRING_LENGTH, 8,100,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"          Please Wailt ...", AUTO_STRING_LENGTH, 8,125,GRAPHICS_TRANSPARENT_TEXT);


}

void drawDiagnosticMenu(Graphics_Context g_sContext,Graphics_Rectangle rect, bool diagnosticPressure, bool diagnosticBalance){
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_clearDisplay(&g_sContext);
    rect.xMax = 315;
    rect.xMin = 8;
    rect.yMax = 135;
    rect.yMin = 155;
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(&g_sContext,"Wind Tunnel Telemetry System", AUTO_STRING_LENGTH, 8,10,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Diagnostic Menu", AUTO_STRING_LENGTH, 8,35,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Communication Status", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Pressure Module: ", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Balance Module:", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
    if(diagnosticPressure)
        Graphics_drawString(&g_sContext,"OK", AUTO_STRING_LENGTH, 200,85,GRAPHICS_TRANSPARENT_TEXT);
    else
        Graphics_drawString(&g_sContext,"Failed", AUTO_STRING_LENGTH, 200,85,GRAPHICS_TRANSPARENT_TEXT);
    if(diagnosticBalance)
        Graphics_drawString(&g_sContext,"OK", AUTO_STRING_LENGTH, 200,110,GRAPHICS_TRANSPARENT_TEXT);
    else
        Graphics_drawString(&g_sContext,"Failed", AUTO_STRING_LENGTH, 200,110,GRAPHICS_TRANSPARENT_TEXT);

    Graphics_drawString(&g_sContext,"Return to Main Menu", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);

}

void drawExperimentConfirmationMenu(Graphics_Context g_sContext, Graphics_Rectangle rect){
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_clearDisplay(&g_sContext);
    rect.xMax = 200;
    rect.xMin = 8;
    rect.yMax = 130;
    rect.yMin = 110;
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(&g_sContext,"Wind Tunnel Telemetry System", AUTO_STRING_LENGTH, 8,10,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Confirm Experiment Menu", AUTO_STRING_LENGTH, 8,35,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Acquisition Time: ", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"0sec", AUTO_STRING_LENGTH, 230,60,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Duration Time:", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,totalExperimentDuration, AUTO_STRING_LENGTH, 230,85,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"1. Start Experiment", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"2. Return to Previous Menu", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"3. Return to Main Menu", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
}

void drawAcquiredPressureModuleResults(Graphics_Context g_sContext, Graphics_Rectangle rect, char p1[10],char p2[10],char p3[10],char p4[10],char p5[10],char p6[10],char p7[10],char p8[10],int resultMenu){

    if(resultMenu == 0){
        Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_setFont(&g_sContext, &g_sFontCmss20b);
        Graphics_clearDisplay(&g_sContext);
        rect.xMax = 200;
        rect.xMin = 8;
        rect.yMax = 230;
        rect.yMin = 210;
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
        Graphics_drawString(&g_sContext,"Wind Tunnel Telemetry System", AUTO_STRING_LENGTH, 8,10,GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawString(&g_sContext,"Pressure Module Results", AUTO_STRING_LENGTH, 8,35,GRAPHICS_TRANSPARENT_TEXT);

        Graphics_drawString(&g_sContext,"Sensor 1:              Pa", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);

        Graphics_drawString(&g_sContext,"Sensor 2:              Pa", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);

        Graphics_drawString(&g_sContext,"Sensor 3:              Pa", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);

        Graphics_drawString(&g_sContext,"Sensor 4:              Pa", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);

        Graphics_drawString(&g_sContext,"Sensor 5:              Pa", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);

        Graphics_drawString(&g_sContext,"Sensor 6:              Pa", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);

        int yCoordinate = 60;

        Graphics_drawString(&g_sContext,p1, 9, 120,yCoordinate,GRAPHICS_TRANSPARENT_TEXT);
        yCoordinate = yCoordinate + 25;
        Graphics_drawString(&g_sContext,p2, 9, 120,yCoordinate,GRAPHICS_TRANSPARENT_TEXT);
        yCoordinate = yCoordinate + 25;
        Graphics_drawString(&g_sContext,p3, 9, 120,yCoordinate,GRAPHICS_TRANSPARENT_TEXT);
        yCoordinate = yCoordinate + 25;
        Graphics_drawString(&g_sContext,p4, 9, 120,yCoordinate,GRAPHICS_TRANSPARENT_TEXT);
        yCoordinate = yCoordinate + 25;
        Graphics_drawString(&g_sContext,p5, 9, 120,yCoordinate,GRAPHICS_TRANSPARENT_TEXT);
        yCoordinate = yCoordinate + 25;
        Graphics_drawString(&g_sContext,p6, 9, 120,yCoordinate,GRAPHICS_TRANSPARENT_TEXT);

        Graphics_drawString(&g_sContext,"Next", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
    }
    if(resultMenu == 1){
        Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_setFont(&g_sContext, &g_sFontCmss20b);
        Graphics_clearDisplay(&g_sContext);
        rect.xMax = 200;
        rect.xMin = 8;
        rect.yMax = 230;
        rect.yMin = 210;
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
        Graphics_drawString(&g_sContext,"Wind Tunnel Telemetry System", AUTO_STRING_LENGTH, 8,10,GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawString(&g_sContext,"Pressure Module Results", AUTO_STRING_LENGTH, 8,35,GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawString(&g_sContext,"Sensor 7:              Pa", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);

        Graphics_drawString(&g_sContext,"Sensor 8:              Pa", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);

        Graphics_drawString(&g_sContext,"Sensor 9:              Pa", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);

        Graphics_drawString(&g_sContext,"Sensor 10:             Pa", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);

        Graphics_drawString(&g_sContext,"Sensor 11:             Pa", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);

        Graphics_drawString(&g_sContext,"Sensor 12:             Pa", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);

        int yCoordinate = 60;

        Graphics_drawString(&g_sContext,p7, 9, 120,yCoordinate,GRAPHICS_TRANSPARENT_TEXT);
        yCoordinate = yCoordinate + 25;
        Graphics_drawString(&g_sContext,p8, 9, 120,yCoordinate,GRAPHICS_TRANSPARENT_TEXT);
        yCoordinate = yCoordinate + 25;

        Graphics_drawString(&g_sContext,"Next", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);

    }
    if(resultMenu == 2){
        Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_setFont(&g_sContext, &g_sFontCmss20b);
        Graphics_clearDisplay(&g_sContext);
        rect.xMax = 200;
        rect.xMin = 8;
        rect.yMax = 180;
        rect.yMin = 160;
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
        Graphics_drawString(&g_sContext,"Wind Tunnel Telemetry System", AUTO_STRING_LENGTH, 8,10,GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawString(&g_sContext,"Pressure Module Results", AUTO_STRING_LENGTH, 8,35,GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawString(&g_sContext,"Sensor 13:             Pa", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);

        Graphics_drawString(&g_sContext,"Sensor 14:             Pa", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);

        Graphics_drawString(&g_sContext,"Sensor 15:             Pa", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);

        Graphics_drawString(&g_sContext,"Sensor 16:             Pa", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);

        Graphics_drawString(&g_sContext,"Next: ", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
    }
}

void drawAcquiredBalanceModuleResults(Graphics_Context g_sContext, Graphics_Rectangle rect, char ForceArray[25]){

    char force1[5];
    char force2[5];
    char force3[5];
    char force4[5];
    char force5[5];
    force1[0] = ForceArray[0];
    force1[1] = ForceArray[1];
    force1[2] = ForceArray[2];
    force1[3] = ForceArray[3];
    force1[4] = ForceArray[4];
    force2[0] = ForceArray[5];
    force2[1] = ForceArray[6];
    force2[2] = ForceArray[7];
    force2[3] = ForceArray[8];
    force2[4] = ForceArray[9];
    force3[0] = ForceArray[10];
    force3[1] = ForceArray[11];
    force3[2] = ForceArray[12];
    force3[3] = ForceArray[13];
    force3[4] = ForceArray[14];
    force4[0] = ForceArray[15];
    force4[1] = ForceArray[16];
    force4[2] = ForceArray[17];
    force4[3] = ForceArray[18];
    force4[4] = ForceArray[19];
    force5[0] = ForceArray[20];
    force5[1] = ForceArray[21];
    force5[2] = ForceArray[22];
    force5[3] = ForceArray[23];
    force5[4] = ForceArray[24];

    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_clearDisplay(&g_sContext);
    rect.xMax = 200;
    rect.xMin = 8;
    rect.yMax = 230;
    rect.yMin = 210;
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(&g_sContext,"Wind Tunnel Telemetry System", AUTO_STRING_LENGTH, 8,10,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Pressure Module Results", AUTO_STRING_LENGTH, 8,35,GRAPHICS_TRANSPARENT_TEXT);

    Graphics_drawString(&g_sContext,"Drag Back Force:", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,force2, AUTO_STRING_LENGTH, 200,60,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"lb", AUTO_STRING_LENGTH, 250,60,GRAPHICS_OPAQUE_TEXT);
    Graphics_drawString(&g_sContext,"Drag Front Force: ", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,force1, AUTO_STRING_LENGTH, 200,85,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"lb", AUTO_STRING_LENGTH, 250,85,GRAPHICS_OPAQUE_TEXT);
    Graphics_drawString(&g_sContext,"Lift Force:", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,force5, AUTO_STRING_LENGTH, 200,110,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"lb", AUTO_STRING_LENGTH, 250,110,GRAPHICS_OPAQUE_TEXT);
    Graphics_drawString(&g_sContext,"Left Side Force: ", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,force4, AUTO_STRING_LENGTH, 200,135,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"lb", AUTO_STRING_LENGTH, 250,135,GRAPHICS_OPAQUE_TEXT);
    Graphics_drawString(&g_sContext,"Right Side Force: ", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,force3, AUTO_STRING_LENGTH, 200,160,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"lb", AUTO_STRING_LENGTH, 250,160,GRAPHICS_OPAQUE_TEXT);

    Graphics_drawString(&g_sContext,"Next", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
}

void drawRemoteModeMenu(Graphics_Context g_sContext, Graphics_Rectangle rect){
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_NAVY);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_clearDisplay(&g_sContext);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
    Graphics_drawString(&g_sContext,"WindTel Remote Mode", AUTO_STRING_LENGTH,50,85,GRAPHICS_OPAQUE_TEXT);
    Graphics_drawString(&g_sContext,"Please Standby", AUTO_STRING_LENGTH,90,110,GRAPHICS_OPAQUE_TEXT);

}
void drawAcquiredDynamicModuleResults(Graphics_Context g_sContext, Graphics_Rectangle rect, char tempResult[10],char humidResult[10],char windSpeedResult[10]){

    Graphics_clearDisplay(&g_sContext);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(&g_sContext,tempResult, AUTO_STRING_LENGTH, 180,60,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,humidResult, AUTO_STRING_LENGTH, 180,85,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,windSpeedResult, AUTO_STRING_LENGTH, 180,110,GRAPHICS_TRANSPARENT_TEXT);

    rect.xMax = 200;
    rect.xMin = 8;
    rect.yMax = 230;
    rect.yMin = 210;
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(&g_sContext,"Wind Tunnel Telemetry System", AUTO_STRING_LENGTH, 8,10,GRAPHICS_OPAQUE_TEXT);
    Graphics_drawString(&g_sContext,"Dynamic Module Results", AUTO_STRING_LENGTH, 8,35,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Tempeture:", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"C", AUTO_STRING_LENGTH, 250,60,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Humidity:", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"%", AUTO_STRING_LENGTH, 250,85,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Wind Speed: ", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"mph", AUTO_STRING_LENGTH, 250,110,GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,"Next", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
}

void increaseExpRepetitions(int secs){
    if(numberOfExperiments<5){
        numberOfExperiments++;
        tostring(numberOfExperimentBuffer,numberOfExperiments);
        Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_setFont(&g_sContext, &g_sFontCmss20b);
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
        Graphics_drawString(&g_sContext,numberOfExperimentBuffer, AUTO_STRING_LENGTH, 230,110,GRAPHICS_OPAQUE_TEXT);

        int intTotalExpTime = (numberOfExperiments+1)*secs;
        if(intTotalExpTime < 60){
            if(intTotalExpTime<10){
                tostring(totalExperimentDuration,(numberOfExperiments+1)*intTotalExpTime);
                totalExperimentDuration[1] = 's';
                totalExperimentDuration[2] = 'e';
                totalExperimentDuration[3] = 'c';
                totalExperimentDuration[4] = ' ';
                totalExperimentDuration[5] = ' ';
            }
            else{
                tostring(totalExperimentDuration,intTotalExpTime);
                totalExperimentDuration[2] = 's';
                totalExperimentDuration[3] = 'e';
                totalExperimentDuration[4] = 'c';
                totalExperimentDuration[5] = ' ';
                totalExperimentDuration[6] = ' ';
            }
        }
        else{
            min = intTotalExpTime/60;
            if(min == 1)
                decimal = intTotalExpTime - 60;
            else if(min == 2)
                decimal = intTotalExpTime - 120;
            else if(min == 3)
                decimal = intTotalExpTime - 180;
            else if(min == 4)
                decimal = intTotalExpTime - 240;
            else if(min == 5)
                decimal = intTotalExpTime - 300;
            minChar = '0' + min;
            decimalChar1 = '0' + decimal/10;
            decimalChar2 = '0' + 10*((decimal/10.0) - (decimal/10));
            totalExperimentDuration[0] = minChar;
            totalExperimentDuration[1] = '.';
            totalExperimentDuration[2] = decimalChar1;
            totalExperimentDuration[3] = decimalChar2;
            totalExperimentDuration[4] = 'm';
            totalExperimentDuration[5] = 'i';
            totalExperimentDuration[6] = 'n';
            totalExperimentDuration[7] = ' ';
            totalExperimentDuration[8] = ' ';
        }
        Graphics_drawString(&g_sContext,totalExperimentDuration, AUTO_STRING_LENGTH, 230,85,GRAPHICS_OPAQUE_TEXT);
    }

}

void decreaseExpRepetitions(int secs){
    if(numberOfExperiments != 0){
        numberOfExperiments--;
        tostring(numberOfExperimentBuffer,numberOfExperiments);
        Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_setFont(&g_sContext, &g_sFontCmss20b);
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
        if(!numberOfExperiments)
            Graphics_drawString(&g_sContext,"0", AUTO_STRING_LENGTH, 230,110,GRAPHICS_OPAQUE_TEXT);
        else
            Graphics_drawString(&g_sContext,numberOfExperimentBuffer, AUTO_STRING_LENGTH, 230,110,GRAPHICS_OPAQUE_TEXT);

        int intTotalExpTime = (numberOfExperiments+1)*secs;
        if(intTotalExpTime < 60){
            if(intTotalExpTime<10){
                tostring(totalExperimentDuration,(numberOfExperiments+1)*intTotalExpTime);
                totalExperimentDuration[1] = 's';
                totalExperimentDuration[2] = 'e';
                totalExperimentDuration[3] = 'c';
                totalExperimentDuration[4] = ' ';
                totalExperimentDuration[5] = ' ';
            }
            else{
                tostring(totalExperimentDuration,intTotalExpTime);
                totalExperimentDuration[2] = 's';
                totalExperimentDuration[3] = 'e';
                totalExperimentDuration[4] = 'c';
                totalExperimentDuration[5] = ' ';
                totalExperimentDuration[6] = ' ';
            }
        }
        else{
            min = intTotalExpTime/60;
            if(min == 1)
                decimal = intTotalExpTime - 60;
            else if(min == 2)
                decimal = intTotalExpTime - 120;
            else if(min == 3)
                decimal = intTotalExpTime - 180;
            else if(min == 4)
                decimal = intTotalExpTime - 240;
            else if(min == 5)
                decimal = intTotalExpTime - 300;
            minChar = '0' + min;
            decimalChar1 = '0' + decimal/10;
            decimalChar2 = '0' + 10*((decimal/10.0) - (decimal/10));
            totalExperimentDuration[0] = minChar;
            totalExperimentDuration[1] = '.';
            totalExperimentDuration[2] = decimalChar1;
            totalExperimentDuration[3] = decimalChar2;
            totalExperimentDuration[4] = 'm';
            totalExperimentDuration[5] = 'i';
            totalExperimentDuration[6] = 'n';
            totalExperimentDuration[7] = ' ';
            totalExperimentDuration[8] = ' ';
        }
        Graphics_drawString(&g_sContext,totalExperimentDuration, AUTO_STRING_LENGTH, 230,85,GRAPHICS_OPAQUE_TEXT);
    }

}
void selectSensor(Graphics_Context g_sContext, Graphics_Rectangle rect,int currentOptionNum){
    int cursorPosition = 85 + (currentOptionNum-2)*25;
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_GREEN);
    if(currentOptionNum == 1)
        Graphics_drawString(&g_sContext,"x", AUTO_STRING_LENGTH, 127,60,GRAPHICS_TRANSPARENT_TEXT);
    else if(currentOptionNum < 6)
        Graphics_drawString(&g_sContext,"x", AUTO_STRING_LENGTH, 113,cursorPosition,GRAPHICS_TRANSPARENT_TEXT);
    else{
        cursorPosition = 87 + (currentOptionNum-6)*25;
        Graphics_drawString(&g_sContext,"x", AUTO_STRING_LENGTH, 255,cursorPosition,GRAPHICS_TRANSPARENT_TEXT);
    }
}

void deselectSensor(Graphics_Context g_sContext, Graphics_Rectangle rect,int currentOptionNum){
    int cursorPosition = 87 + (currentOptionNum-2)*25;
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_GREEN);
    if(currentOptionNum == 1)
        Graphics_drawString(&g_sContext," ", AUTO_STRING_LENGTH, 127,62,GRAPHICS_OPAQUE_TEXT);
    else if(currentOptionNum < 6)
        Graphics_drawString(&g_sContext," ", AUTO_STRING_LENGTH, 113,cursorPosition,GRAPHICS_OPAQUE_TEXT);
    else{
        cursorPosition = 87 + (currentOptionNum-6)*25;
        Graphics_drawString(&g_sContext," ", AUTO_STRING_LENGTH, 255,cursorPosition,GRAPHICS_OPAQUE_TEXT);
    }
}

void selectForce(Graphics_Context g_sContext, Graphics_Rectangle rect,int currentOptionNum){
    int cursorPosition = 60 + (currentOptionNum-1)*25;
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_GREEN);
    Graphics_drawString(&g_sContext," x", AUTO_STRING_LENGTH, 160,cursorPosition,GRAPHICS_TRANSPARENT_TEXT);
}

void deselectForce(Graphics_Context g_sContext, Graphics_Rectangle rect,int currentOptionNum){
    int cursorPosition = 60 + (currentOptionNum-1)*25;
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_GREEN);
    Graphics_drawString(&g_sContext,"  ", AUTO_STRING_LENGTH, 161,cursorPosition,GRAPHICS_OPAQUE_TEXT);

}

void selectTempHumid(Graphics_Context g_sContext, Graphics_Rectangle rect,int currentOptionNum){
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_GREEN);
    if(currentOptionNum == 3)
        Graphics_drawString(&g_sContext,"x", AUTO_STRING_LENGTH, 252,110,GRAPHICS_TRANSPARENT_TEXT);
    if(currentOptionNum == 4)
        Graphics_drawString(&g_sContext,"x", AUTO_STRING_LENGTH, 252,135,GRAPHICS_TRANSPARENT_TEXT);
    if(currentOptionNum == 5)
        Graphics_drawString(&g_sContext,"x", AUTO_STRING_LENGTH, 252,160,GRAPHICS_TRANSPARENT_TEXT);

}

void deselectTempHumid(Graphics_Context g_sContext, Graphics_Rectangle rect,int currentOptionNum){
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_GREEN);
    if(currentOptionNum == 3)
        Graphics_drawString(&g_sContext," ", AUTO_STRING_LENGTH, 252,110,GRAPHICS_OPAQUE_TEXT);
    if(currentOptionNum == 4)
        Graphics_drawString(&g_sContext," ", AUTO_STRING_LENGTH, 252,135,GRAPHICS_OPAQUE_TEXT);
    if(currentOptionNum == 5)
        Graphics_drawString(&g_sContext," ", AUTO_STRING_LENGTH, 252,160,GRAPHICS_OPAQUE_TEXT);
}

void nextForce(Graphics_Context g_sContext, Graphics_Rectangle rect,int menuIndex,int currentOptionNum){
    if(currentOptionNum == 6){
        rect.xMax = 140;
        rect.xMin = 8;
        rect.yMax = 205;
        rect.yMin = 185;

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);

        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 230;
        rect.yMin = 210;
    }
    else if(currentOptionNum == 7){
        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 230;
        rect.yMin = 210;

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);

        rect.xMax = 140;
        rect.xMin = 8;
        rect.yMax = 80;
        rect.yMin = 60;
    }
    else{
        rect.xMax = 140;
        rect.xMin = 8;
        rect.yMax = 80 + 25*(currentOptionNum-1);
        rect.yMin = 60 + 25*(currentOptionNum-1);

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);


        rect.xMax = 140;
        rect.xMin = 8;
        rect.yMax = 80 + 25*currentOptionNum;
        rect.yMin = 60 + 25*currentOptionNum;

    }

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);


    if(currentOptionNum == 1){
        Graphics_drawString(&g_sContext,"1.All Forces", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawString(&g_sContext,"2.Drag Front", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
    }
    else if(currentOptionNum == 2){
        Graphics_drawString(&g_sContext,"2.Drag Front", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawString(&g_sContext,"3.Drag Back", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
    }
    else if(currentOptionNum == 3){
        Graphics_drawString(&g_sContext,"3.Drag Back", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawString(&g_sContext,"4.Side Right", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
    }
    else if(currentOptionNum == 4){
        Graphics_drawString(&g_sContext,"4.Side Right", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawString(&g_sContext,"5.Side Left", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
    }
    else if(currentOptionNum == 5){
        Graphics_drawString(&g_sContext,"5.Side Left", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawString(&g_sContext,"6.Up", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);
    }
    else if(currentOptionNum == 6){
        Graphics_drawString(&g_sContext,"6.Up", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawString(&g_sContext,"7.Return to Parameter Menu", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
    }
    else if(currentOptionNum == 7){
        Graphics_drawString(&g_sContext,"7.Return to Parameter Menu", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawString(&g_sContext,"1.All Forces", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
    }
}

void previousForce(Graphics_Context g_sContext, Graphics_Rectangle rect,int menuIndex,int currentOptionNum){
    if(currentOptionNum == 1){
        rect.xMax = 140;
        rect.xMin = 8;
        rect.yMax = 80;
        rect.yMin = 60;

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);

        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 230;
        rect.yMin = 210;
    }
    else if(currentOptionNum == 7){
        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 230;
        rect.yMin = 210;

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);
        rect.xMax = 140;
        rect.xMin = 8;
        rect.yMax = 205;
        rect.yMin = 185;
    }
    else{
        rect.xMax = 140;
        rect.xMin = 8;
        rect.yMax = 80 + 25*(currentOptionNum-1);
        rect.yMin = 60 + 25*(currentOptionNum-1);

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);


        rect.xMax = 140;
        rect.xMin = 8;
        rect.yMax = 80 + 25*(currentOptionNum-2);
        rect.yMin = 60 + 25*(currentOptionNum-2);

    }

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);


    if(currentOptionNum == 1){
        Graphics_drawString(&g_sContext,"1.All Forces", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawString(&g_sContext,"7.Return to Parameter Menu", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
    }
    else if(currentOptionNum == 2){
        Graphics_drawString(&g_sContext,"1.All Forces", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawString(&g_sContext,"2.Drag Front", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
    }
    else if(currentOptionNum == 3){
        Graphics_drawString(&g_sContext,"2.Drag Front", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawString(&g_sContext,"3.Drag Back", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
    }
    else if(currentOptionNum == 4){
        Graphics_drawString(&g_sContext,"3.Drag Back", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawString(&g_sContext,"4.Side Right", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
    }
    else if(currentOptionNum == 5){
        Graphics_drawString(&g_sContext,"4.Side Right", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawString(&g_sContext,"5.Side Left", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
    }
    else if(currentOptionNum == 6){
        Graphics_drawString(&g_sContext,"5.Side Left", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawString(&g_sContext,"6.Up", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);
    }
    else if(currentOptionNum == 7){
        Graphics_drawString(&g_sContext,"6.Up", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawString(&g_sContext,"7.Return to Parameter Menu", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
    }
}

void nextSensor(Graphics_Context g_sContext, Graphics_Rectangle rect,int menuIndex,int currentOptionNum){
    if(currentOptionNum == 1){
        rect.xMax = 112;
        rect.xMin = 8;
        rect.yMax = 80 + 25*(currentOptionNum-1);
        rect.yMin = 60 + 25*(currentOptionNum-1);

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);

        rect.xMax = 130;
        rect.xMin = 8;
        rect.yMax = 80 + 25*currentOptionNum;
        rect.yMin = 60 + 25*currentOptionNum;

    }
    if(currentOptionNum < 5){
        rect.xMax = 100;
        rect.xMin = 8;
        rect.yMax = 80 + 25*(currentOptionNum-1);
        rect.yMin = 60 + 25*(currentOptionNum-1);

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);

        rect.xMax = 100;
        rect.xMin = 8;
        rect.yMax = 80 + 25*currentOptionNum;
        rect.yMin = 60 + 25*currentOptionNum;

    }
    else if(currentOptionNum == 5){
        rect.xMax = 100;
        rect.xMin = 8;
        rect.yMax = 80 + 25*(currentOptionNum-1);
        rect.yMin = 60 + 25*(currentOptionNum-1);

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);

        rect.xMax = 243;
        rect.xMin = 150;
        rect.yMax = 105 + 25*(currentOptionNum-5);
        rect.yMin = 85 + 25*(currentOptionNum-5);
    }
    else if((currentOptionNum > 5) && (currentOptionNum <= 8)){
        rect.xMax = 243;
        rect.xMin = 150;
        rect.yMax = 105 + 25*(currentOptionNum-6);
        rect.yMin = 85 + 25*(currentOptionNum-6);

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);

        rect.xMax = 243;
        rect.xMin = 150;
        rect.yMax = 105 + 25*(currentOptionNum-5);
        rect.yMin = 85 + 25*(currentOptionNum-5);
    }
    else if(currentOptionNum == 9){
        rect.xMax = 243;
        rect.xMin = 150;
        rect.yMax = 80 + 25*(currentOptionNum-5);
        rect.yMin = 60 + 25*(currentOptionNum-5);

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);

        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 80 + 25*(currentOptionNum-4);
        rect.yMin = 60 + 25*(currentOptionNum-4);
    }
    else if(currentOptionNum == 10){
        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 80 + 25*(currentOptionNum-5);
        rect.yMin = 60 + 25*(currentOptionNum-5);

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);
        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 80 + 25*(currentOptionNum-4);
        rect.yMin = 60 + 25*(currentOptionNum-4);
    }
    else if(currentOptionNum == 11){
        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 80 + 25*(currentOptionNum-5);
        rect.yMin = 60 + 25*(currentOptionNum-5);

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);

        rect.xMax = 112;
        rect.xMin = 8;
        rect.yMax = 80;
        rect.yMin = 60;

    }

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);

    if(menuIndex == 7){
        if(currentOptionNum == 1){
            Graphics_drawString(&g_sContext,"All Sensors [ ]", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor  1", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 2){
            Graphics_drawString(&g_sContext,"Sensor  1", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor  2", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 3){
            Graphics_drawString(&g_sContext,"Sensor  2", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor  3", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 4){
            Graphics_drawString(&g_sContext,"Sensor  3", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor  4", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 5){
            Graphics_drawString(&g_sContext,"Sensor  4", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor  5", AUTO_STRING_LENGTH, 150,85,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 6){
            Graphics_drawString(&g_sContext,"Sensor  5", AUTO_STRING_LENGTH, 150,85,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor  6", AUTO_STRING_LENGTH, 150,110,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 7){
            Graphics_drawString(&g_sContext,"Sensor  6", AUTO_STRING_LENGTH, 150,110,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor  7", AUTO_STRING_LENGTH, 150,135,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 8){
            Graphics_drawString(&g_sContext,"Sensor  7", AUTO_STRING_LENGTH, 150,135,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor  8", AUTO_STRING_LENGTH, 150,160,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 9){
            Graphics_drawString(&g_sContext,"Sensor  8", AUTO_STRING_LENGTH, 150,160,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"More Sensors", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 10){
            Graphics_drawString(&g_sContext,"More Sensors", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Return to Parameter Menu", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 11){
            Graphics_drawString(&g_sContext,"All Sensors [ ]", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Return to Parameter Menu", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
        }
    }
    if(menuIndex == 8){
        if(currentOptionNum == 1){
            Graphics_drawString(&g_sContext,"All Sensors [ ]", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor  9", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 2){
            Graphics_drawString(&g_sContext,"Sensor  9", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor 10", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 3){
            Graphics_drawString(&g_sContext,"Sensor 10", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor 11", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 4){
            Graphics_drawString(&g_sContext,"Sensor 11", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor 12", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 5){
            Graphics_drawString(&g_sContext,"Sensor 12", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor 13", AUTO_STRING_LENGTH, 150,85,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 6){
            Graphics_drawString(&g_sContext,"Sensor 13", AUTO_STRING_LENGTH, 150,85,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor 14", AUTO_STRING_LENGTH, 150,110,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 7){
            Graphics_drawString(&g_sContext,"Sensor 14", AUTO_STRING_LENGTH, 150,110,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor 15", AUTO_STRING_LENGTH, 150,135,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 8){
            Graphics_drawString(&g_sContext,"Sensor 15", AUTO_STRING_LENGTH, 150,135,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor 16", AUTO_STRING_LENGTH, 150,160,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 9){
            Graphics_drawString(&g_sContext,"Sensor 16", AUTO_STRING_LENGTH, 150,160,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"More Sensors", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 10){
            Graphics_drawString(&g_sContext,"More Sensors", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Return to Parameter Menu", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 11){
            Graphics_drawString(&g_sContext,"All Sensors [ ]", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Return to Parameter Menu", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
        }
    }
}

void previousSensor(Graphics_Context g_sContext, Graphics_Rectangle rect,int menuIndex,int currentOptionNum){

    if(currentOptionNum == 1){
        rect.xMax = 112;
        rect.xMin = 8;
        rect.yMax = 80;
        rect.yMin = 60;

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);

        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 230;
        rect.yMin = 210;
    }
    else if(currentOptionNum == 2){
        rect.xMax = 100;
        rect.xMin = 8;
        rect.yMax = 105;
        rect.yMin = 85;

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);

        rect.xMax = 112;
        rect.xMin = 8;
        rect.yMax = 80;
        rect.yMin = 60;
    }
    else if(currentOptionNum < 6){
        rect.xMax = 100;
        rect.xMin = 8;
        rect.yMax = 80 + 25*(currentOptionNum-1);
        rect.yMin = 60 + 25*(currentOptionNum-1);

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);

        rect.xMax = 100;
        rect.xMin = 8;
        rect.yMax = 80 + 25*(currentOptionNum-2);
        rect.yMin = 60 + 25*(currentOptionNum-2);

    }
    else if(currentOptionNum == 6){
        rect.xMax = 243;
        rect.xMin = 150;
        rect.yMax = 105 + 25*(currentOptionNum-6);
        rect.yMin = 85 + 25*(currentOptionNum-6);

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);

        rect.xMax = 100;
        rect.xMin = 8;
        rect.yMax = 80 + 25*(currentOptionNum-2);
        rect.yMin = 60 + 25*(currentOptionNum-2);

    }
    else if((currentOptionNum > 5) && (currentOptionNum <= 9)){
        rect.xMax = 243;
        rect.xMin = 150;
        rect.yMax = 105 + 25*(currentOptionNum-6);
        rect.yMin = 85 + 25*(currentOptionNum-6);

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);

        rect.xMax = 243;
        rect.xMin = 150;
        rect.yMax = 105 + 25*(currentOptionNum-7);
        rect.yMin = 85 + 25*(currentOptionNum-7);

    }
    else if(currentOptionNum == 10){
        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 205;
        rect.yMin = 185;

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);

        rect.xMax = 243;
        rect.xMin = 150;
        rect.yMax = 180;
        rect.yMin = 160;
    }
    else if(currentOptionNum == 11){
        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 230;
        rect.yMin = 210;

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);
        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 205;
        rect.yMin = 185;
    }

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);

    if(menuIndex == 7){
        if(currentOptionNum == 1){
            Graphics_drawString(&g_sContext,"All Sensors [ ]", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Return to Parameter Menu", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 2){
            Graphics_drawString(&g_sContext,"All Sensors [ ]", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor  1", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 3){
            Graphics_drawString(&g_sContext,"Sensor  1", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor  2", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 4){
            Graphics_drawString(&g_sContext,"Sensor  2", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor  3", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 5){
            Graphics_drawString(&g_sContext,"Sensor  3", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor  4", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 6){
            Graphics_drawString(&g_sContext,"Sensor  4", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor  5", AUTO_STRING_LENGTH, 150,85,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 7){
            Graphics_drawString(&g_sContext,"Sensor  5", AUTO_STRING_LENGTH, 150,85,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor  6", AUTO_STRING_LENGTH, 150,110,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 8){
            Graphics_drawString(&g_sContext,"Sensor  6", AUTO_STRING_LENGTH, 150,110,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor  7", AUTO_STRING_LENGTH, 150,135,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 9){
            Graphics_drawString(&g_sContext,"Sensor  7", AUTO_STRING_LENGTH, 150,135,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor  8", AUTO_STRING_LENGTH, 150,160,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 10){
            Graphics_drawString(&g_sContext,"Sensor  8", AUTO_STRING_LENGTH, 150,160,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"More Sensors", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 11){
            Graphics_drawString(&g_sContext,"More Sensors", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Return to Parameter Menu", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
        }
    }

    if(menuIndex == 8){
        if(currentOptionNum == 1){
            Graphics_drawString(&g_sContext,"All Sensors [ ]", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Return to Parameter Menu", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 2){
            Graphics_drawString(&g_sContext,"All Sensors [ ]", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor  9", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 3){
            Graphics_drawString(&g_sContext,"Sensor  9", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor 10", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 4){
            Graphics_drawString(&g_sContext,"Sensor 10", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor 11", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 5){
            Graphics_drawString(&g_sContext,"Sensor 11", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor 12", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 6){
            Graphics_drawString(&g_sContext,"Sensor 12", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor 13", AUTO_STRING_LENGTH, 150,85,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 7){
            Graphics_drawString(&g_sContext,"Sensor 13", AUTO_STRING_LENGTH, 150,85,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor 14", AUTO_STRING_LENGTH, 150,110,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 8){
            Graphics_drawString(&g_sContext,"Sensor 14", AUTO_STRING_LENGTH, 150,110,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor 15", AUTO_STRING_LENGTH, 150,135,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 9){
            Graphics_drawString(&g_sContext,"Sensor 15", AUTO_STRING_LENGTH, 150,135,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Sensor 16", AUTO_STRING_LENGTH, 150,160,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 10){
            Graphics_drawString(&g_sContext,"Sensor 16", AUTO_STRING_LENGTH, 150,160,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"More Sensors", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 11){
            Graphics_drawString(&g_sContext,"More Sensors", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Return to Parameter Menu", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
        }
    }
}


void nextSample(Graphics_Context g_sContext, Graphics_Rectangle rect, int menuIndex,int currentOptionNum)
{
    char *menuString[5] = {"Pressure:","Forces:","Temperature:","Relative Humidity:","Wind Speed:"};
    int menuStringIndexInUsed[5] = {0,0,0,0,0};
    int index,j = 0;
    start_cursor = 60;

    if(currentOptionNum < measurmentsGoingToSample){
        rect.xMax = 200;
        rect.xMin = 8;
        rect.yMax = 80 + 25*(currentOptionNum-1);
        rect.yMin = 60 + 25*(currentOptionNum-1);

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);

        rect.xMax = 200;
        rect.xMin = 8;
        rect.yMax = 80 + 25*currentOptionNum;
        rect.yMin = 60 + 25*currentOptionNum;

    }
    else if(currentOptionNum == measurmentsGoingToSample){
        rect.xMax = 200;
        rect.xMin = 8;
        rect.yMax = 80 + 25*(currentOptionNum-1);
        rect.yMin = 60 + 25*(currentOptionNum-1);

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);

        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 80 + 25*currentOptionNum;
        rect.yMin = 60 + 25*currentOptionNum;

    }
    else if(currentOptionNum == measurmentsGoingToSample+1){
        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 80 + 25*(currentOptionNum-1);
        rect.yMin = 60 + 25*(currentOptionNum-1);

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);

        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 80 + 25*currentOptionNum;
        rect.yMin = 60 + 25*currentOptionNum;

    }
    else if(currentOptionNum == measurmentsGoingToSample+2){
        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 80 + 25*(currentOptionNum-1);
        rect.yMin = 60 + 25*(currentOptionNum-1);

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);

        rect.xMax = 200;
        rect.xMin = 8;
        rect.yMax = 80;
        rect.yMin = 60;

    }


    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);

    if(currentOptionNum < measurmentsGoingToSample){
        for(index = 0;index<5;index++){
            if(sampleMeasureSelection[index]){
                menuStringIndexInUsed[j] = index;
                j++;
            }
        }

        index = 0;
        firstOption = menuString[menuStringIndexInUsed[index]];

        while(index < measurmentsGoingToSample){
            Graphics_drawString(&g_sContext,menuString[menuStringIndexInUsed[index]], AUTO_STRING_LENGTH, 8,start_cursor,GRAPHICS_TRANSPARENT_TEXT);
            start_cursor += 25;
            index++;
            lastOption = menuString[menuStringIndexInUsed[index-1]];
        }
        lastNumOption = start_cursor-25;
    }
    else{
        if(currentOptionNum == measurmentsGoingToSample){
            if(currentOptionNum == 1){
                lastNumOption = 60;
                for(index = 0;index<5;index++){
                    if(sampleMeasureSelection[index]){
                        menuStringIndexInUsed[j] = index;
                        j++;
                    }
                }

                index = 0;
                firstOption = menuString[menuStringIndexInUsed[index]];

                while(index < measurmentsGoingToSample){
                    Graphics_drawString(&g_sContext,menuString[menuStringIndexInUsed[index]], AUTO_STRING_LENGTH, 8,start_cursor,GRAPHICS_TRANSPARENT_TEXT);
                    start_cursor += 25;
                    index++;
                    lastOption = menuString[menuStringIndexInUsed[index-1]];
                }
            }
            Graphics_drawString(&g_sContext,lastOption, AUTO_STRING_LENGTH, 8,lastNumOption,GRAPHICS_TRANSPARENT_TEXT);
            lastNumOption += 25;
            Graphics_drawString(&g_sContext,"Confirm the selected samples", AUTO_STRING_LENGTH, 8,lastNumOption,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == measurmentsGoingToSample+1){
            Graphics_drawString(&g_sContext,"Confirm the selected samples", AUTO_STRING_LENGTH, 8,lastNumOption,GRAPHICS_TRANSPARENT_TEXT);
            lastNumOption += 25;
            Graphics_drawString(&g_sContext,"Return to Previous Menu", AUTO_STRING_LENGTH, 8,lastNumOption,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == measurmentsGoingToSample+2){
            Graphics_drawString(&g_sContext,firstOption, AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            lastNumOption = 60 + 25*(measurmentsGoingToSample+1);
            Graphics_drawString(&g_sContext,"Return to Previous Menu", AUTO_STRING_LENGTH, 8,lastNumOption,GRAPHICS_TRANSPARENT_TEXT);
        }
    }
}

void previousSample(Graphics_Context g_sContext, Graphics_Rectangle rect, int menuIndex,int currentOptionNum)
{

    char *menuString[5] = {"Pressure:","Forces:","Temperature:","Relative Humidity:","Wind Speed:"};
    int menuStringIndexInUsed[5] = {0,0,0,0,0};
    int index,j = 0;
    for(index = 0;index<5;index++){
        if(sampleMeasureSelection[index]){
            menuStringIndexInUsed[j] = index;
            j++;
        }
    }
    firstOption = menuString[menuStringIndexInUsed[0]];
    index = 0;
    start_cursor = 60;
    if(currentOptionNum == 1){
        rect.xMax = 200;
        rect.xMin = 8;
        rect.yMax = 80;
        rect.yMin = 60;

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);

        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 80 + 25*(measurmentsGoingToSample+1);
        rect.yMin = 60 + 25*(measurmentsGoingToSample+1);

    }
    else if(currentOptionNum < measurmentsGoingToSample){
        rect.xMax = 200;
        rect.xMin = 8;
        rect.yMax = 80 + 25*(currentOptionNum-1);
        rect.yMin = 60 + 25*(currentOptionNum-1);

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);

        rect.xMax = 200;
        rect.xMin = 8;
        rect.yMax = 80 + 25*(currentOptionNum-2);
        rect.yMin = 60 + 25*(currentOptionNum-2);

    }
    else if(currentOptionNum == measurmentsGoingToSample){
        rect.xMax = 200;
        rect.xMin = 8;
        rect.yMax = 80 + 25*(currentOptionNum-1);
        rect.yMin = 60 + 25*(currentOptionNum-1);

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);

        rect.xMax = 200;
        rect.xMin = 8;
        rect.yMax = 80 + 25*(currentOptionNum-2);
        rect.yMin = 60 + 25*(currentOptionNum-2);

    }
    else if(currentOptionNum == measurmentsGoingToSample+1){
        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 80 + 25*(currentOptionNum-1);
        rect.yMin = 60 + 25*(currentOptionNum-1);

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);

        rect.xMax = 200;
        rect.xMin = 8;
        rect.yMax = 80 + 25*(currentOptionNum-2);
        rect.yMin = 60 + 25*(currentOptionNum-2);
    }
    else if(currentOptionNum > measurmentsGoingToSample+1){
        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 80 + 25*(currentOptionNum-1);
        rect.yMin = 60 + 25*(currentOptionNum-1);

        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);

        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 80 + 25*(currentOptionNum-2);
        rect.yMin = 60 + 25*(currentOptionNum-2);
    }

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    if(currentOptionNum < (measurmentsGoingToSample+1) && currentOptionNum > 1){
        while(index < measurmentsGoingToSample){
            Graphics_drawString(&g_sContext,menuString[menuStringIndexInUsed[index]], AUTO_STRING_LENGTH, 8,start_cursor,GRAPHICS_TRANSPARENT_TEXT);
            start_cursor += 25;
            index++;
            lastOption = menuString[menuStringIndexInUsed[index-1]];
            lastNumOption = start_cursor-25;
        }
    }
    else{
        while(index < measurmentsGoingToSample){
            start_cursor += 25;
            index++;
            lastOption = menuString[menuStringIndexInUsed[index-1]];
            lastNumOption = start_cursor-25;
        }

        if(currentOptionNum == 1){
            start_cursor = 60 + 25*(measurmentsGoingToSample+1);
            Graphics_drawString(&g_sContext,firstOption, AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Return to Previous Menu", AUTO_STRING_LENGTH, 8,start_cursor,GRAPHICS_TRANSPARENT_TEXT);

        }
        else if(currentOptionNum == measurmentsGoingToSample+1){
            Graphics_drawString(&g_sContext,lastOption, AUTO_STRING_LENGTH, 8,lastNumOption,GRAPHICS_TRANSPARENT_TEXT);
            lastNumOption += 25;
            Graphics_drawString(&g_sContext,"Confirm the selected samples", AUTO_STRING_LENGTH, 8,lastNumOption,GRAPHICS_TRANSPARENT_TEXT);
            lastNumOption -= 25;
        }
        else if(currentOptionNum == measurmentsGoingToSample+2){
            lastNumOption += 25;
            Graphics_drawString(&g_sContext,"Confirm the selected samples", AUTO_STRING_LENGTH, 8,lastNumOption,GRAPHICS_TRANSPARENT_TEXT);
            lastNumOption += 25;
            Graphics_drawString(&g_sContext,"Return to Previous Menu", AUTO_STRING_LENGTH, 8,lastNumOption,GRAPHICS_TRANSPARENT_TEXT);
            lastNumOption -= 25;
        }
        else if(currentOptionNum < measurmentsGoingToSample){
            Graphics_drawString(&g_sContext,"Confirm the selected samples", AUTO_STRING_LENGTH, 8,lastNumOption,GRAPHICS_TRANSPARENT_TEXT);
            lastNumOption += 25;
            Graphics_drawString(&g_sContext,"Return to Previous Menu", AUTO_STRING_LENGTH, 8,lastNumOption,GRAPHICS_TRANSPARENT_TEXT);
        }
    }
}
void increaseSampleSize(Graphics_Context g_sContext, int experimentTimeInSeconds, int currentOptionNum){
    int yCoordinate = 60 + 25*(currentOptionNum-1);
    int sampleTimeContraint;
    char *menuString[5] = {"Pressure:","Forces:","Temperature:","Relative Humidity:","Wind Speed:"};
    int menuStringIndexInUsed[5] = {0,0,0,0,0};
    int index,j = 0;
    for(index = 0;index<5;index++){
        if(sampleMeasureSelection[index]){
            menuStringIndexInUsed[j] = index;
            j++;
        }
    }

    if(sampleMeasureSelection[menuStringIndexInUsed[currentOptionNum-1]] && menuStringIndexInUsed[currentOptionNum-1] == 0){
        if(sampleCounter[currentOptionNum-1] < experimentTimeInSeconds){
            sampleCounter[currentOptionNum-1]++;
            numPressureSamples++;
        }
    }
    else if(sampleMeasureSelection[menuStringIndexInUsed[currentOptionNum-1]] && menuStringIndexInUsed[currentOptionNum-1] == 1){
        if(sampleCounter[currentOptionNum-1] < experimentTimeInSeconds){
            sampleCounter[currentOptionNum-1]++;
            numForceSamples++;
        }
    }
    else if(sampleMeasureSelection[menuStringIndexInUsed[currentOptionNum-1]] && menuStringIndexInUsed[currentOptionNum-1] == 2){
        if(sampleCounter[currentOptionNum-1] < experimentTimeInSeconds/5){
            sampleCounter[currentOptionNum-1]++;
            numTemperatureSamples++;
        }

    }
    else if(sampleMeasureSelection[menuStringIndexInUsed[currentOptionNum-1]] && menuStringIndexInUsed[currentOptionNum-1] == 3){
        if(sampleCounter[currentOptionNum-1] < experimentTimeInSeconds/5){
            sampleCounter[currentOptionNum-1]++;
            numHumiditySamples++;
        }
    }
    else if(sampleMeasureSelection[menuStringIndexInUsed[currentOptionNum-1]] && menuStringIndexInUsed[currentOptionNum-1] == 4){
        if(sampleCounter[currentOptionNum-1] < experimentTimeInSeconds){
            sampleCounter[currentOptionNum-1]++;
            numWindSpeedSamples++;
        }
    }

    tostring(sampleBuff,sampleCounter[currentOptionNum-1]);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(&g_sContext,sampleBuff, AUTO_STRING_LENGTH, 220,yCoordinate,GRAPHICS_OPAQUE_TEXT);
}

void decrementSampleSize(Graphics_Context g_sContext, int currentOptionNum){
    int yCoordinate = 60 + 25*(currentOptionNum-1);
    int sampleTimeContraint;
    char *menuString[5] = {"Pressure:","Forces:","Temperature:","Relative Humidity:","Wind Speed:"};
    int menuStringIndexInUsed[5] = {0,0,0,0,0};
    int index,j = 0;
    for(index = 0;index<5;index++){
        if(sampleMeasureSelection[index]){
            menuStringIndexInUsed[j] = index;
            j++;
        }
    }

    if(sampleMeasureSelection[currentOptionNum-1] && menuStringIndexInUsed[currentOptionNum-1] == 0){
        if(sampleCounter[currentOptionNum-1] > 0){
            sampleCounter[currentOptionNum-1]--;
            numPressureSamples--;
        }
    }
    else if(sampleMeasureSelection[currentOptionNum-1] && menuStringIndexInUsed[currentOptionNum-1] == 1){
        if(sampleCounter[currentOptionNum-1] > 0){
            sampleCounter[currentOptionNum-1]--;
            numForceSamples--;
        }
    }
    else if(sampleMeasureSelection[currentOptionNum-1] && menuStringIndexInUsed[currentOptionNum-1] == 2){
        if(sampleCounter[currentOptionNum-1] > 0){
            sampleCounter[currentOptionNum-1]--;
            numTemperatureSamples--;
        }

    }
    else if(sampleMeasureSelection[currentOptionNum-1] && menuStringIndexInUsed[currentOptionNum-1] == 3){
        if(sampleCounter[currentOptionNum-1] > 0){
            sampleCounter[currentOptionNum-1]--;
            numHumiditySamples--;
        }
    }
    else if(sampleMeasureSelection[currentOptionNum-1] && menuStringIndexInUsed[currentOptionNum-1] == 4){
        if(sampleCounter[currentOptionNum-1] > 0){
            sampleCounter[currentOptionNum-1]--;
            numWindSpeedSamples--;
        }
    }

    tostring(sampleBuff,sampleCounter[currentOptionNum-1]);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(&g_sContext,sampleBuff, AUTO_STRING_LENGTH, 220,yCoordinate,GRAPHICS_OPAQUE_TEXT);
}

void resetSampleCounters(){
    sampleCounter[0] = 1;
    sampleCounter[1] = 1;
    sampleCounter[2] = 1;
    sampleCounter[3] = 1;
    sampleCounter[4] = 1;
}
void nextOption(Graphics_Context g_sContext, Graphics_Rectangle rect, int menuIndex,int currentOptionNum,int rod_Length)
{
    if(menuIndex == 6 && currentOptionNum > 1 && currentOptionNum < 6){
        if(currentOptionNum == 2){
            rect.xMax = 315;
            rect.xMin = 8;
            rect.yMax = 80 + 25*(currentOptionNum-1);
            rect.yMin = 60 + 25*(currentOptionNum-1);
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            Graphics_drawRectangle(&g_sContext,&rect);
            Graphics_fillRectangle(&g_sContext,&rect);
            rect.xMax = 200;
            rect.xMin = 8;
            rect.yMax = 80 + 25*currentOptionNum;
            rect.yMin = 60 + 25*currentOptionNum;
        }
        else if(currentOptionNum == 3 || currentOptionNum == 4){
            rect.xMax = 200;
            rect.xMin = 8;
            rect.yMax = 80 + 25*(currentOptionNum-1);
            rect.yMin = 60 + 25*(currentOptionNum-1);
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            Graphics_drawRectangle(&g_sContext,&rect);
            Graphics_fillRectangle(&g_sContext,&rect);
            rect.xMax = 200;
            rect.xMin = 8;
            rect.yMax = 80 + 25*currentOptionNum;
            rect.yMin = 60 + 25*currentOptionNum;
        }
        else if(currentOptionNum == 5){
            rect.xMax = 200;
            rect.xMin = 8;
            rect.yMax = 80 + 25*(currentOptionNum-1);
            rect.yMin = 60 + 25*(currentOptionNum-1);
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            Graphics_drawRectangle(&g_sContext,&rect);
            Graphics_fillRectangle(&g_sContext,&rect);
            rect.xMax = 315;
            rect.xMin = 8;
            rect.yMax = 80 + 25*currentOptionNum;
            rect.yMin = 60 + 25*currentOptionNum;
        }
    }
    else{
        rect.xMax = 315;
        rect.xMin = 8;

        if(menuIndex != 3 && menuIndex != 10){
            rect.yMax = 80 + 25*(currentOptionNum-1);
            rect.yMin = 60 + 25*(currentOptionNum-1);
        }
        else{
            rect.yMax = 105 + 25*(currentOptionNum-1);
            rect.yMin = 85 + 25*(currentOptionNum-1);
        }
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);
        if((menuIndex == 1 && currentOptionNum == 4) || (menuIndex == 4 && currentOptionNum == 3) || (menuIndex == 6 && currentOptionNum == 7)){
            rect.xMax = 315;
            rect.xMin = 8;
            rect.yMax = 80;
            rect.yMin = 60;
        }
        else if(menuIndex == 3 || menuIndex == 10){
            if((menuIndex == 3 &&currentOptionNum == 3) || (currentOptionNum == 4 && menuIndex == 10)){
                rect.xMax = 315;
                rect.xMin = 8;
                rect.yMax = 105;
                rect.yMin = 85;
            }
            else
            {
                rect.xMax = 315;
                rect.xMin = 8;
                rect.yMax = 105 + 25*currentOptionNum;
                rect.yMin = 85 + 25*currentOptionNum;
            }
        }
        else{
            rect.xMax = 315;
            rect.xMin = 8;
            rect.yMax = 80 + 25*currentOptionNum;
            rect.yMin = 60 + 25*currentOptionNum;
        }
    }


    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);

    if(menuIndex == 1){
        if(currentOptionNum == 1){
            Graphics_drawString(&g_sContext,"1. System Reset", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"2. Perform Experiment", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 2){
            Graphics_drawString(&g_sContext,"2. Perform Experiment", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"3. Speed Control", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 3){
            Graphics_drawString(&g_sContext,"3. Speed Control", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"4. Use WindTel Application", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 4){
            Graphics_drawString(&g_sContext,"1. System Reset", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"4. Use WindTel Application", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
        }
    }

    if(menuIndex == 3){
        if(currentOptionNum == 1){
            Graphics_drawString(&g_sContext,"1.Enter the rod length:", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            if(rod_Length)
                Graphics_drawString(&g_sContext,rod_LengthBalance, 5,140,85,GRAPHICS_TRANSPARENT_TEXT);
            else
                Graphics_drawString(&g_sContext,"1cm", AUTO_STRING_LENGTH,140,85,GRAPHICS_TRANSPARENT_TEXT);

            Graphics_drawString(&g_sContext,"2.Confirm the selected length", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 2){
            Graphics_drawString(&g_sContext,"2.Confirm the selected length", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"3.Return to Main Menu", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 3){
            Graphics_drawString(&g_sContext,"1.Enter the rod length:", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            if(rod_Length)
                Graphics_drawString(&g_sContext,rod_LengthBalance, 5,140,85,GRAPHICS_TRANSPARENT_TEXT);
            else
                Graphics_drawString(&g_sContext,"1cm", AUTO_STRING_LENGTH,140,85,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"3.Return to Main Menu", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
        }
    }
    if(menuIndex == 4){
        if(currentOptionNum == 1){
            Graphics_drawString(&g_sContext,"1. Turn On Wind Tunnel Fan", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"2. Turn Off Wind Tunnel Fan", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 2){
            Graphics_drawString(&g_sContext,"2. Turn Off Wind Tunnel Fan", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"3. Return to Main Menu", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 3){
            Graphics_drawString(&g_sContext,"1. Turn On Wind Tunnel Fan", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"3. Return to Main Menu", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
        }
    }

    if(menuIndex == 6){
        if(currentOptionNum == 1){
            Graphics_drawString(&g_sContext,"1.Barometric Pressure", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"2.Wind Exerted Forces", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 2){
            Graphics_drawString(&g_sContext,"2.Wind Exerted Forces", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"3.Relative Humidity", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 3){
            Graphics_drawString(&g_sContext,"3.Relative Humidity", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"4.Temperature", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 4){
            Graphics_drawString(&g_sContext,"4.Temperature", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"5.Wind Speed", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 5){
            Graphics_drawString(&g_sContext,"5.Wind Speed", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"6.Proceed with the experiment", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 6){
            Graphics_drawString(&g_sContext,"6.Proceed with the experiment", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"7.Return to Main Menu", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 7){
            Graphics_drawString(&g_sContext,"1.Barometric Pressure", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"7.Return to Main Menu", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
        }
    }
    if(menuIndex == 10){
        if(currentOptionNum == 1){
            Graphics_drawString(&g_sContext,experimentDuration, AUTO_STRING_LENGTH,140,85,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"2.Confirm the selected duration", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 2){
            Graphics_drawString(&g_sContext,"2.Confirm the selected duration", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"3.Return to Parameter Menu", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 3){
            Graphics_drawString(&g_sContext,"3.Return to Parameter Menu", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"4.Return to Main Menu", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 4){
            Graphics_drawString(&g_sContext,experimentDuration, AUTO_STRING_LENGTH,140,85,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"4.Return to Main Menu", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
        }
    }
    if(menuIndex == 11){
        if(currentOptionNum == 1){
            Graphics_drawString(&g_sContext,"Pressure:", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Forces:", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 2){
            Graphics_drawString(&g_sContext,"Forces:", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Temperature:", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 3){
            Graphics_drawString(&g_sContext,"Temperature:", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Relative Humidity:", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 4){
            Graphics_drawString(&g_sContext,"Relative Humidity:", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Wind Speed", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 5){
            Graphics_drawString(&g_sContext,"Wind Speed", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Confirm the selected samples", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 6){
            Graphics_drawString(&g_sContext,"Confirm the selected samples", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Return to Exp.Duration Menu", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 7){
            Graphics_drawString(&g_sContext,"Pressure:", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"Return to Exp.Duration Menu", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
        }
    }
}
void previousOption(Graphics_Context g_sContext, Graphics_Rectangle rect, int menuIndex,int currentOptionNum,int rod_Length)
{
    if(menuIndex == 6 && currentOptionNum > 2 && currentOptionNum < 7){

        if(currentOptionNum == 3){
            rect.xMax = 200;
            rect.xMin = 8;
            rect.yMax = 80 + 25*(currentOptionNum-1);
            rect.yMin = 60 + 25*(currentOptionNum-1);
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            Graphics_drawRectangle(&g_sContext,&rect);
            Graphics_fillRectangle(&g_sContext,&rect);
            rect.xMax = 315;
            rect.xMin = 8;
            rect.yMax = 80 + 25*(currentOptionNum-2);
            rect.yMin = 60 + 25*(currentOptionNum-2);
        }
        else if(currentOptionNum == 4 || currentOptionNum == 5){
            rect.xMax = 200;
            rect.xMin = 8;
            rect.yMax = 80 + 25*(currentOptionNum-1);
            rect.yMin = 60 + 25*(currentOptionNum-1);
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            Graphics_drawRectangle(&g_sContext,&rect);
            Graphics_fillRectangle(&g_sContext,&rect);
            rect.xMax = 200;
            rect.xMin = 8;
            rect.yMax = 80 + 25*(currentOptionNum-2);
            rect.yMin = 60 + 25*(currentOptionNum-2);
        }
        else if(currentOptionNum == 6){
            rect.xMax = 315;
            rect.xMin = 8;
            rect.yMax = 80 + 25*(currentOptionNum-1);
            rect.yMin = 60 + 25*(currentOptionNum-1);
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            Graphics_drawRectangle(&g_sContext,&rect);
            Graphics_fillRectangle(&g_sContext,&rect);
            rect.xMax = 200;
            rect.xMin = 8;
            rect.yMax = 80 + 25*(currentOptionNum-2);
            rect.yMin = 60 + 25*(currentOptionNum-2);
        }
    }
    else if(menuIndex == 11){

        if(currentOptionNum == 1){
            rect.xMax = 200;
            rect.xMin = 8;
            rect.yMax = 80;
            rect.yMin = 60;
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            Graphics_drawRectangle(&g_sContext,&rect);
            Graphics_fillRectangle(&g_sContext,&rect);
            rect.xMax = 315;
            rect.xMin = 8;
            rect.yMax = 230;
            rect.yMin = 210;
        }
        else if(currentOptionNum < 5){
            rect.xMax = 200;
            rect.xMin = 8;
            rect.yMax = 80 + 25*(currentOptionNum-1);
            rect.yMin = 60 + 25*(currentOptionNum-1);
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            Graphics_drawRectangle(&g_sContext,&rect);
            Graphics_fillRectangle(&g_sContext,&rect);
            rect.xMax = 200;
            rect.xMin = 8;
            rect.yMax = 80 + 25*(currentOptionNum-2);
            rect.yMin = 60 + 25*(currentOptionNum-2);
        }
        else if(currentOptionNum == 5){
            rect.xMax = 315;
            rect.xMin = 8;
            rect.yMax = 80 + 25*(currentOptionNum-1);
            rect.yMin = 60 + 25*(currentOptionNum-1);
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            Graphics_drawRectangle(&g_sContext,&rect);
            Graphics_fillRectangle(&g_sContext,&rect);
            rect.xMax = 200;
            rect.xMin = 8;
            rect.yMax = 80 + 25*(currentOptionNum-2);
            rect.yMin = 60 + 25*(currentOptionNum-2);
        }
        else if(currentOptionNum > 5){
            rect.xMax = 315;
            rect.xMin = 8;
            rect.yMax = 80 + 25*(currentOptionNum-1);
            rect.yMin = 60 + 25*(currentOptionNum-1);
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            Graphics_drawRectangle(&g_sContext,&rect);
            Graphics_fillRectangle(&g_sContext,&rect);
            rect.xMax = 315;
            rect.xMin = 8;
            rect.yMax = 80 + 25*(currentOptionNum-2);
            rect.yMin = 60 + 25*(currentOptionNum-2);
        }
    }
    else
    {
        rect.xMax = 315;
        rect.xMin = 8;
        if(menuIndex !=3 && menuIndex != 10){
            rect.yMax = 80 + 25*(currentOptionNum-1);
            rect.yMin = 60 + 25*(currentOptionNum-1);
        }
        else{
            rect.yMax = 105 + 25*(currentOptionNum-1);
            rect.yMin = 85 + 25*(currentOptionNum-1);
        }
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);

        if((menuIndex == 1 && currentOptionNum == 1)){
            rect.xMax = 315;
            rect.xMin = 8;
            rect.yMax = 155;
            rect.yMin = 135;
        }
        else if(menuIndex == 3){
            if(currentOptionNum == 1){
                rect.xMax = 315;
                rect.xMin = 8;
                rect.yMax = 155;
                rect.yMin = 135;
            }
            else
            {
                rect.xMax = 315;
                rect.xMin = 8;
                rect.yMax = 105 + 25*(currentOptionNum-2);
                rect.yMin = 85 + 25*(currentOptionNum-2);
            }
        }
        else if(menuIndex == 4 && currentOptionNum == 1){
            rect.xMax = 315;
            rect.xMin = 8;
            rect.yMax = 130;
            rect.yMin = 110;
        }
        else if(menuIndex == 6 && currentOptionNum == 1){
            rect.xMax = 315;
            rect.xMin = 8;
            rect.yMax = 230;
            rect.yMin = 210;
        }
        else if(menuIndex == 10){
            if(currentOptionNum == 1){
                rect.xMax = 315;
                rect.xMin = 8;
                rect.yMax = 180;
                rect.yMin = 160;
            }
            else
            {
                rect.xMax = 315;
                rect.xMin = 8;
                rect.yMax = 105 + 25*(currentOptionNum-2);
                rect.yMin = 85 + 25*(currentOptionNum-2);
            }
        }
        else{
            rect.xMax = 315;
            rect.xMin = 8;
            rect.yMax = 80 + 25*(currentOptionNum-2);
            rect.yMin = 60 + 25*(currentOptionNum-2);
        }
    }
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    if(menuIndex == 1){
        if(currentOptionNum == 1){
            Graphics_drawString(&g_sContext,"1. System Reset", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"4. Use WindTel Application", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 2){
            Graphics_drawString(&g_sContext,"1. System Reset", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"2. Perform Experiment", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 3){
            Graphics_drawString(&g_sContext,"2. Perform Experiment", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"3. Speed Control", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 4){
            Graphics_drawString(&g_sContext,"3. Speed Control", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"4. Use WindTel Application", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
        }
    }
    if(menuIndex == 3){
        if(currentOptionNum == 1){
            if(rod_Length)
                Graphics_drawString(&g_sContext,rod_LengthBalance, 5,140,85,GRAPHICS_TRANSPARENT_TEXT);
            else
                Graphics_drawString(&g_sContext,"1cm", AUTO_STRING_LENGTH,140,85,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"3.Return to Main Menu", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 2){
            if(rod_Length)
                Graphics_drawString(&g_sContext,rod_LengthBalance, 5,140,85,GRAPHICS_TRANSPARENT_TEXT);
            else
                Graphics_drawString(&g_sContext,"1cm", AUTO_STRING_LENGTH,140,85,GRAPHICS_TRANSPARENT_TEXT);

            Graphics_drawString(&g_sContext,"2.Confirm the selected length", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 3){
            Graphics_drawString(&g_sContext,"2.Confirm the selected length", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"3.Return to Main Menu", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
        }
    }
    if(menuIndex == 4){
        if(currentOptionNum == 1){
            Graphics_drawString(&g_sContext,"1. Turn On Wind Tunnel Fan", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"3. Return to Main Menu", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 2){
            Graphics_drawString(&g_sContext,"1. Turn On Wind Tunnel Fan", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"2. Turn Off Wind Tunnel Fan", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 3){
            Graphics_drawString(&g_sContext,"2. Turn Off Wind Tunnel Fan", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"3. Return to Main Menu", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
        }

    }
    if(menuIndex == 6){
        if(currentOptionNum == 1){
            Graphics_drawString(&g_sContext,"1.Barometric Pressure", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"7.Return to Main Menu", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 2){
            Graphics_drawString(&g_sContext,"1.Barometric Pressure", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"2.Wind Exerted Forces", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 3){
            Graphics_drawString(&g_sContext,"2.Wind Exerted Forces", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"3.Relative Humidity", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 4){
            Graphics_drawString(&g_sContext,"3.Relative Humidity", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"4.Temperature", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 5){
            Graphics_drawString(&g_sContext,"4.Temperature", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"5.Wind Speed", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 6){
            Graphics_drawString(&g_sContext,"5.Wind Speed", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"6.Proceed with the experiment", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 7){
            Graphics_drawString(&g_sContext,"6.Proceed with the experiment", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"7.Return to Main Menu", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
        }
    }
    if(menuIndex == 10){
        if(currentOptionNum == 1){
            Graphics_drawString(&g_sContext,experimentDuration, AUTO_STRING_LENGTH,140,85,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"4.Return to Main Menu", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 2){
            Graphics_drawString(&g_sContext,experimentDuration, AUTO_STRING_LENGTH,140,85,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"2.Confirm the selected duration", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 3){
            Graphics_drawString(&g_sContext,"2.Confirm the selected duration", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"3.Return to Parameter Menu", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 4){
            Graphics_drawString(&g_sContext,"3.Return to Parameter Menu", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"4.Return to Main Menu", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
        }
    }

    if(menuIndex == 11){
        if(currentOptionNum == 1){
            Graphics_drawString(&g_sContext,"1.Pressure:", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"7.Return to Main Menu", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 2){
            Graphics_drawString(&g_sContext,"1.Pressure:", AUTO_STRING_LENGTH, 8,60,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"2.Forces:", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 3){
            Graphics_drawString(&g_sContext,"2.Forces:", AUTO_STRING_LENGTH, 8,85,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"3.Temperature:", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 4){
            Graphics_drawString(&g_sContext,"3.Temperature:", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"4.Relative Humidity:", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 5){
            Graphics_drawString(&g_sContext,"4.Relative Humidity:", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"5.Confirm the selected samples", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 6){
            Graphics_drawString(&g_sContext,"5.Confirm the selected samples", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"6.Return to Exp.Duration Menu", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);
        }
        else if(currentOptionNum == 7){
            Graphics_drawString(&g_sContext,"6.Return to Exp.Duration Menu", AUTO_STRING_LENGTH, 8,185,GRAPHICS_TRANSPARENT_TEXT);
            Graphics_drawString(&g_sContext,"7.Return to Main Menu", AUTO_STRING_LENGTH, 8,210,GRAPHICS_TRANSPARENT_TEXT);
        }
    }
}
void nextConfirmation(Graphics_Context g_sContext, Graphics_Rectangle rect,int currentOptionNum)
{
    if(currentOptionNum == 1){
        rect.xMax = 200;
        rect.xMin = 8;
        rect.yMax = 130 + 25*(currentOptionNum-1);
        rect.yMin = 110 + 25*(currentOptionNum-1);
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);
        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 130 + 25*currentOptionNum;
        rect.yMin = 110 + 25*currentOptionNum;
    }
    else if(currentOptionNum > 1 && currentOptionNum != 3){
        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 130 + 25*(currentOptionNum-1);
        rect.yMin = 110 + 25*(currentOptionNum-1);
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);
        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 130 + 25*currentOptionNum;
        rect.yMin = 110 + 25*currentOptionNum;
    }
    else if(currentOptionNum == 3){
        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 130 + 25*(currentOptionNum-1);
        rect.yMin = 110 + 25*(currentOptionNum-1);
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);
        rect.xMax = 200;
        rect.xMin = 8;
        rect.yMax = 130;
        rect.yMin = 110;
    }

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);

    if(currentOptionNum == 1){
        Graphics_drawString(&g_sContext,"1. Start Experiment", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawString(&g_sContext,"2. Return to Previous Menu", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
    }
    else if(currentOptionNum == 2){
        Graphics_drawString(&g_sContext,"2. Return to Previous Menu", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawString(&g_sContext,"3. Return to Main Menu", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
    }
    else if(currentOptionNum == 3){
        Graphics_drawString(&g_sContext,"3. Return to Main Menu", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawString(&g_sContext,"1. Start Experiment", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
    }
}

void previousConfirmation(Graphics_Context g_sContext, Graphics_Rectangle rect,int currentOptionNum)
{
    if(currentOptionNum == 1){
        rect.xMax = 200;
        rect.xMin = 8;
        rect.yMax = 130;
        rect.yMin = 110;
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);
        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 180;
        rect.yMin = 160;
    }
    else if(currentOptionNum == 2){
        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 155;
        rect.yMin = 135;
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);
        rect.xMax = 200;
        rect.xMin = 8;
        rect.yMax = 130;
        rect.yMin = 110;
    }
    else if(currentOptionNum == 3){
        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 180;
        rect.yMin = 160;
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawRectangle(&g_sContext,&rect);
        Graphics_fillRectangle(&g_sContext,&rect);
        rect.xMax = 315;
        rect.xMin = 8;
        rect.yMax = 155;
        rect.yMin = 135;
    }


    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawRectangle(&g_sContext,&rect);
    Graphics_fillRectangle(&g_sContext,&rect);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);


    if(currentOptionNum == 1){
        Graphics_drawString(&g_sContext,"1. Start Experiment", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawString(&g_sContext,"3. Return to Main Menu", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
    }
    else if(currentOptionNum == 2){
        Graphics_drawString(&g_sContext,"1. Start Experiment", AUTO_STRING_LENGTH, 8,110,GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawString(&g_sContext,"2. Return to Previous Menu", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
    }
    else if(currentOptionNum == 3){
        Graphics_drawString(&g_sContext,"2. Return to Previous Menu", AUTO_STRING_LENGTH, 8,135,GRAPHICS_TRANSPARENT_TEXT);
        Graphics_drawString(&g_sContext,"3. Return to Main Menu", AUTO_STRING_LENGTH, 8,160,GRAPHICS_TRANSPARENT_TEXT);
    }
}
int getPressureSamples(){
    return numPressureSamples;
}
int getForceSamples(){
    return numForceSamples;
}
int getTempSamples(){
    return numTemperatureSamples;
}
int getHumiditySamples(){
    return numHumiditySamples;
}
int getWindSpeedSamples(){
    return numWindSpeedSamples;
}

#endif /* WINDTEL_GRAPHIC_H_ */
