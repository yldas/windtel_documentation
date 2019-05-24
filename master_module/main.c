/*WindTel Libraries*/
#include "DriverLibFiles/driverlib.h"
#include "WindTel_Graphic.h"
#include "WindTel_I2C.h"
#include "WindTel_UART.h"
#include "WindTel_DataAcquisition.h"
/* Standard Define Libraries */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

/* Time period for timer A1, control module interrupt*/
#define TIMER_PERIOD    11718

// Graphic library context
Graphics_Context g_sContext;
Graphics_Rectangle rect;
char pressureForWifi[1500];
float currentSpeed = 0;
int cursor = 1;
int menuIndex = 1;
int on_off = 1;
char rodLength[6];
int rod_Length = 1;
int seconds = 5;
int selectedPressureSensors[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int pressureSensorsActive = 0;
int selectedWindForces[7] = {0,0,0,0,0,0,0};
int selectedSpeed_Temp[3] ={0,0,0};
int ExperimentStatus = 0;
int secondsPassed = 0;
int acquiredMeasurementDuration = 0;
int diagnosticFlag = 0;
int communicationFailureCounter = 5;
int checkCommunication = 0;
char (PressureModuleRxBuffer[35])[300];
char (DynamicModuleRxBuffer[35])[40];
char (BalanceModuleRxBuffer[35])[25];
char (WifiModuleRxBuffer[5])[40];
int DynamicModuleSampleIndex = 0;
int PressureModuleSampleIndex = 0;
int BalanceModuleSampleIndex = 0;
char (PressPointArray[16])[9];
char (ForcesArray[5])[5];
int pressurePointIndex = 0;
int uart0MessageIndex, uart1MessageIndex, uart2MessageIndex = 0;
static bool recvInBalanceProgress,recvInDynamicProgress,recvInPressureProcess = false;
char startMarker = 'S';
char endMarker = 'E';
char receivedCharacterUART0 =' ';
char receivedCharacterUART1 =' ';
char receivedCharacterUART2 =' ';
bool diagnosticPressure, diagnosticBalance, diagnosticDynamic = false;
int numberOfDynamicSamples = 0, numberOfBalanceSamples = 0, currentDynamicSamples;
int DynamicMode,wifiMode = 0;
//Sample Variables
int humidSamples = 0;
int windSpeedSamples = 0;
int tempSamples = 0;
int wifiCounter = 0;
const int maxBytes = 512;
uint8_t inBytesDynamic[maxBytes];
uint8_t inBytesBalance[maxBytes];
uint8_t inBytesPressure[maxBytes];
uint8_t indexDynamic = 0;
uint8_t indexBalance = 0;
uint8_t indexPressure = 0;
int wait = 12;
bool forceSelected, balanceSelected, pressureSelected, tempSelected, humidSelected, speedSelected = false;
//variables going to be sent to dynamic module
char durationBuff[5];
char tempBuff[5];
char humBuff[5];
char speedBuff[5];

//variables received by dynamic module
char windSpeedResult[10];
char tempResult[10];
char humidResult[10];

//variables received by pressure module
char p1[10];
char p2[10];
char p3[10];
char p4[10];
char p5[10];
char p6[10];
char p7[10];
char p8[10];
char pTime1[10];
char pTime2[10];
char pTime3[10];
char pTime4[10];
char pTime5[10];
char pTime6[10];
char pTime7[10];
char pTime8[10];

//variables received in remote mode
char remoteDuration[5];
char remotePressureSample[5];
char remotePSensorPoint1[5];
char remotePSensorPoint2[5];
char remotePSensorPoint3[5];
char remotePSensorPoint4[5];
char remotePSensorPoint5[5];
char remotePSensorPoint6[5];
char remotePSensorPoint7[5];
char remotePSensorPoint8[5];
char remotePSensorPoint9[5];
char remotePSensorPoint10[5];
char remotePSensorPoint11[5];
char remotePSensorPoint12[5];
char remotePSensorPoint13[5];
char remotePSensorPoint14[5];
char remotePSensorPoint15[5];
char remotePSensorPoint16[5];
int pressureSamples = 0;
char remoteTemperature[5];
char remoteHumidity[5];
char remoteForceSample[5];
char remoteForce1[5];
char remoteForce2[5];
char remoteForce3[5];
char remoteForce4[5];
char remoteForce5[5];
char remoteRodLength[5];

//wifi Commands
char remotePressureCommand[21] = "S0000000000000000000E";
char remoteDynamicCommand[14];
char remoteBalanceCommand[8];
const Timer_A_UpModeConfig upConfig =
{
 TIMER_A_CLOCKSOURCE_ACLK,              // ACLK Clock Source
 TIMER_A_CLOCKSOURCE_DIVIDER_1,          // ACLK/1 = 3MHz
 99999999999,                           // 5000 tick period
 TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
 TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,    // Enable CCR0 interrupt
 TIMER_A_DO_CLEAR                        // Clear value
};

const Timer_A_UpModeConfig AcquisitionTimeConfig =
{
 TIMER_A_CLOCKSOURCE_ACLK,              // ACLK Clock Source
 TIMER_A_CLOCKSOURCE_DIVIDER_2,          // ACLK
 128000,                           // 5000 tick period
 TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
 TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,    // Enable CCR0 interrupt
 TIMER_A_DO_CLEAR                        // Clear value
};

void sendWifiCommands(void){
    seconds = toInt(remoteDuration);


    int pressureSamples = toInt(remotePressureSample);
    if(remotePSensorPoint1[0] == '1')
        selectedPressureSensors[2] = 1;
    if(remotePSensorPoint2[0] == '1')
        selectedPressureSensors[3] = 1;
    if(remotePSensorPoint3[0] == '1')
        selectedPressureSensors[4] = 1;
    if(remotePSensorPoint4[0] == '1')
        selectedPressureSensors[5] = 1;
    if(remotePSensorPoint5[0] == '1')
        selectedPressureSensors[6] = 1;
    if(remotePSensorPoint6[0] == '1')
        selectedPressureSensors[7] = 1;
    if(remotePSensorPoint7[0] == '1')
        selectedPressureSensors[8] = 1;
    if(remotePSensorPoint8[0] == '1')
        selectedPressureSensors[9] = 1;
    if(remotePSensorPoint9[0] == '1')
        selectedPressureSensors[12] = 1;
    if(remotePSensorPoint10[0] == '1')
        selectedPressureSensors[13] = 1;
    if(remotePSensorPoint11[0] == '1')
        selectedPressureSensors[14] = 1;
    if(remotePSensorPoint12[0] == '1')
        selectedPressureSensors[15] = 1;
    if(remotePSensorPoint13[0] == '1')
        selectedPressureSensors[16] = 1;
    if(remotePSensorPoint14[0] == '1')
        selectedPressureSensors[17] = 1;
    if(remotePSensorPoint15[0] == '1')
        selectedPressureSensors[18] = 1;
    if(remotePSensorPoint16[0] == '1')
        selectedPressureSensors[19] = 1;

    int i = 0;
    for(i = 0;i<5;i++){
        tempBuff[i] = remoteTemperature[i];
        humBuff[i] = remoteHumidity[i];
        speedBuff[i] = remoteTemperature[i];
        durationBuff[i] = remoteDuration[i];
    }

    tempSelected = humidSelected = speedSelected = true;

    numberOfBalanceSamples = toInt(remoteForceSample);
    rod_Length = toInt(remoteRodLength);

    acquirePressureModuleData(selectedPressureSensors,pressureSamples,seconds);

    acquireDynamicModuleData(tempBuff,humBuff,speedBuff,durationBuff);

    acquireBalanceModuleData(numberOfBalanceSamples,seconds,rod_Length);

}
void readyDynamicData(){
    sprintf(durationBuff,"%d",seconds);
    sprintf(tempBuff,"%d",tempSamples);
    sprintf(humBuff,"%d",humidSamples);
    sprintf(speedBuff,"%d",windSpeedSamples);
}
void setnumberOfDynamicSamples(void){
    tempSamples = numberOfDynamicSamples = getTempSamples();
    humidSamples = getHumiditySamples();
    windSpeedSamples = getWindSpeedSamples();
    if(numberOfDynamicSamples<humidSamples)
        numberOfDynamicSamples = humidSamples;
    if(numberOfDynamicSamples<windSpeedSamples)
        numberOfDynamicSamples = windSpeedSamples;
}

void setnumberOfBalanceSamples(void){
    numberOfBalanceSamples = getForceSamples();
}

void parseDynamicResults(void){
    char str[40];
    strcpy(str, DynamicModuleRxBuffer[0]);
    char delim[] = ",";
    char *ptr;

    ptr = strtok(str, delim);
    strcpy(tempResult, ptr);
    ptr = strtok(NULL, delim);
    ptr = strtok(NULL, delim);
    strcpy(humidResult, ptr);
    ptr = strtok(NULL, delim);
    ptr = strtok(NULL, delim);
    strcpy(windSpeedResult, ptr);
}

void parsePressureResults(void){
    char str[300];
    strcpy(str, PressureModuleRxBuffer[0]);
    char delim[] = ",";
    char *ptr;

    ptr = strtok(str, delim);

    strcpy(p1, ptr);
    ptr = strtok(NULL, delim);

    strcpy(pTime1, ptr);
    ptr = strtok(NULL, delim);

    strcpy(p2, ptr);
    ptr = strtok(NULL, delim);

    strcpy(pTime2, ptr);
    ptr = strtok(NULL, delim);

    strcpy(p3, ptr);
    ptr = strtok(NULL, delim);

    strcpy(pTime3, ptr);
    ptr = strtok(NULL, delim);

    strcpy(p4, ptr);
    ptr = strtok(NULL, delim);

    strcpy(pTime4, ptr);
    ptr = strtok(NULL, delim);

    strcpy(p5, ptr);
    ptr = strtok(NULL, delim);

    strcpy(pTime5, ptr);
    ptr = strtok(NULL, delim);

    strcpy(p6, ptr);
    ptr = strtok(NULL, delim);

    strcpy(pTime6, ptr);
    ptr = strtok(NULL, delim);

    strcpy(p7, ptr);
    ptr = strtok(NULL, delim);

    strcpy(pTime7, ptr);
    ptr = strtok(NULL, delim);

    strcpy(p8, ptr);
    ptr = strtok(NULL, delim);

    strcpy(pTime8, ptr);
    ptr = strtok(NULL, delim);
}

void parseWifiCommands(void){
    char str[40];
    char delim[] = ",";
    char *ptr1;
    char *ptr2;
    char *ptr3;
    strcpy(str, WifiModuleRxBuffer[0]);

    ptr1 = strtok(str, delim);
    strcpy(remoteDuration, ptr1);
    ptr1 = strtok(NULL, delim);

    strcpy(remotePressureSample, ptr1);
    ptr1 = strtok(NULL, delim);

    strcpy(remotePSensorPoint1, ptr1);
    ptr1 = strtok(NULL, delim);

    strcpy(remotePSensorPoint2, ptr1);
    ptr1 = strtok(NULL, delim);

    strcpy(remotePSensorPoint3, ptr1);
    ptr1 = strtok(NULL, delim);

    strcpy(remotePSensorPoint4, ptr1);
    ptr1 = strtok(NULL, delim);

    strcpy(remotePSensorPoint5, ptr1);
    ptr1 = strtok(NULL, delim);

    strcpy(remotePSensorPoint6, ptr1);
    ptr1 = strtok(NULL, delim);

    strcpy(remotePSensorPoint7, ptr1);
    ptr1 = strtok(NULL, delim);

    strcpy(remotePSensorPoint8, ptr1);
    ptr1 = strtok(NULL, delim);

    strcpy(remotePSensorPoint9, ptr1);
    ptr1 = strtok(NULL, delim);

    strcpy(remotePSensorPoint10, ptr1);
    ptr1 = strtok(NULL, delim);

    strcpy(remotePSensorPoint11, ptr1);
    ptr1 = strtok(NULL, delim);

    strcpy(remotePSensorPoint12, ptr1);
    ptr1 = strtok(NULL, delim);

    strcpy(remotePSensorPoint13, ptr1);
    ptr1 = strtok(NULL, delim);

    strcpy(remotePSensorPoint14, ptr1);
    ptr1 = strtok(NULL, delim);

    strcpy(remotePSensorPoint15, ptr1);
    ptr1 = strtok(NULL, delim);

    strcpy(remotePSensorPoint16, ptr1);
    ptr1 = strtok(NULL, delim);

    strcpy(str, WifiModuleRxBuffer[1]);

    ptr2 = strtok(str, delim);
    ptr2 = strtok(NULL, delim);

    strcpy(remoteTemperature, ptr2);
    ptr2 = strtok(NULL, delim);

    strcpy(remoteHumidity, ptr2);
    ptr2 = strtok(NULL, delim);

    strcpy(str, WifiModuleRxBuffer[2]);

    ptr3 = strtok(str, delim);
    ptr3 = strtok(NULL, delim);


    strcpy(remoteRodLength, ptr3);
    ptr3 = strtok(NULL, delim);

    strcpy(remoteForceSample, ptr3);
    ptr3 = strtok(NULL, delim);

    strcpy(remoteForce1, ptr3);
    ptr3 = strtok(NULL, delim);

    strcpy(remoteForce2, ptr3);
    ptr3 = strtok(NULL, delim);

    strcpy(remoteForce3, ptr3);
    ptr3 = strtok(NULL, delim);

    strcpy(remoteForce4, ptr3);
    ptr3 = strtok(NULL, delim);

    strcpy(remoteForce5, ptr3);
    ptr3 = strtok(NULL, delim);
}

//Main program of the master module
void main(int argc, char *argv[]){
    WDT_A_hold(WDT_A_BASE);

    /* Initialize Button Setup. */
    boardInit();
    clockInit();

    // LCD setup using Graphics Library API calls
    Kitronix320x240x16_SSD2119Init();
    Graphics_initContext(&g_sContext, &g_sKitronix320x240x16_SSD2119);

    // Control Module Pin Setup
    P4->DIR |= BIT0 + BIT1;
    P4->OUT |= BIT0 + BIT1;

    P1->DIR |= BIT5;
    P1->OUT &= ~BIT5;

    // UART Pin Setup
    uart_pin_setup();

    //Turn off LCD
    P2->OUT &= ~(BIT7);

    /* Configuring Timer_A1 for Up Mode produce 250ms pulses to increment and decrement speed*/
    MAP_Timer_A_configureUpMode(TIMER_A1_BASE, &upConfig);
    MAP_Timer_A_configureUpMode(TIMER_A0_BASE, &AcquisitionTimeConfig);

    /* Enabling interrupts and starting the timer */
    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_Interrupt_enableInterrupt(INT_TA0_0);
    MAP_Interrupt_enableInterrupt(INT_TA1_0);

    //PushButton Setup
    P5->DIR &= ~(BIT1 + BIT2 + BIT5 + BIT6 + BIT7);
    P5->IE = BIT1 + BIT2 + BIT5 + BIT6 + BIT7;
    P5->IFG = 0;

    MAP_Interrupt_setPriority(INT_EUSCIA1,0);
    MAP_Interrupt_setPriority(INT_EUSCIA0,1);
    MAP_Interrupt_setPriority(INT_EUSCIA2,2);
    drawMainMenu(g_sContext,rect);
    __no_operation();
    __enable_irq();
    NVIC->ISER[1] = 1 << ((PORT5_IRQn) & 31);
    NVIC->ISER[0] |= 1 << ((EUSCIA0_IRQn) & 31);
    NVIC->ISER[0] |= 1 << ((EUSCIA1_IRQn) & 31);
    NVIC->ISER[0] |= 1 << ((EUSCIA2_IRQn) & 31);


    while(on_off){
        __sleep();
        __no_operation();
        __enable_irq();
        NVIC->ISER[1] = 1 << ((PORT5_IRQn) & 31);
        NVIC->ISER[0] |= 1 << ((EUSCIA0_IRQn) & 31);
        NVIC->ISER[0] |= 1 << ((EUSCIA1_IRQn) & 31);
        NVIC->ISER[0] |= 1 << ((EUSCIA2_IRQn) & 31);
    }
}

void PORT5_IRQHandler(void){
    //    ON/OFF Button
    if(P5->IFG & BIT1){
        if(on_off){
            P2->OUT |= BIT7;
            on_off = 0;
        }
        else{
            P2->OUT &= ~(BIT7);
            on_off = 1;
        }
    }
    //Up Button
    if(!on_off){
        if(P5->IFG & BIT5){
            if(menuIndex == 1){
                nextOption(g_sContext, rect, menuIndex,cursor,rod_Length);
                cursor++;
                if(cursor == 5)
                    cursor = 1;
            }
            else if(menuIndex == 3){
                nextOption(g_sContext,rect,menuIndex,cursor,rod_Length);
                cursor++;
                if(cursor == 4)
                    cursor = 1;
            }
            else if(menuIndex == 4){
                nextOption(g_sContext, rect,menuIndex,cursor,rod_Length);
                cursor++;
                if(cursor == 4)
                    cursor = 1;
            }
            else if(menuIndex == 6){
                nextOption(g_sContext, rect,menuIndex,cursor,rod_Length);
                cursor++;
                if(cursor == 8)
                    cursor = 1;
            }
            else if(menuIndex == 7 || menuIndex == 8){
                nextSensor(g_sContext, rect,menuIndex,cursor);
                cursor++;
                if(cursor == 12)
                    cursor = 1;
            }
            else if(menuIndex == 9){
                nextForce(g_sContext, rect,menuIndex,cursor);
                cursor++;
                if(cursor == 8)
                    cursor = 1;
            }
            else if(menuIndex == 10){
                nextOption(g_sContext, rect,menuIndex,cursor,rod_Length);
                cursor++;
                if(cursor == 5)
                    cursor = 1;
            }
            else if(menuIndex == 11){
                nextSample(g_sContext,rect,menuIndex,cursor);
                cursor++;
                if(cursor == getMeasurmentsToSample()+3)
                    cursor = 1;
            }
            else if(menuIndex == 12){
                if(ExperimentStatus == 0){
                    nextConfirmation(g_sContext,rect,cursor);
                    cursor++;
                    if(cursor == 4)
                        cursor = 1;
                }
            }
        }
    }
    //Down Button
    if(P5->IFG & BIT2 && !on_off){
        if(menuIndex == 1){
            previousOption(g_sContext, rect, menuIndex,cursor,rod_Length);
            cursor--;
            if(cursor == 0)
                cursor = 4;
        }
        else if(menuIndex == 3){
            previousOption(g_sContext,rect,menuIndex,cursor,rod_Length);
            cursor--;
            if(cursor == 0)
                cursor = 3;
        }
        else if(menuIndex == 4){
            previousOption(g_sContext, rect,menuIndex,cursor,rod_Length);
            cursor--;
            if(cursor == 0)
                cursor = 3;
        }
        else if(menuIndex == 6){
            previousOption(g_sContext, rect,menuIndex,cursor,rod_Length);
            cursor--;
            if(cursor == 0)
                cursor = 7;
        }
        else if(menuIndex == 7 || menuIndex == 8){
            previousSensor(g_sContext, rect,menuIndex,cursor);
            cursor--;
            if(cursor == 0)
                cursor = 11;
        }
        else if(menuIndex == 9){
            previousForce(g_sContext, rect,menuIndex,cursor);
            cursor--;
            if(cursor == 0)
                cursor = 7;
        }
        else if(menuIndex == 10){
            previousOption(g_sContext, rect,menuIndex,cursor,rod_Length);
            cursor--;
            if(cursor == 0)
                cursor = 4;
        }
        else if(menuIndex == 11){
            previousSample(g_sContext, rect,menuIndex,cursor);
            cursor--;
            if(cursor == 0)
                cursor = getMeasurmentsToSample()+2;
        }
        else if(menuIndex == 12){
            if(ExperimentStatus == 0){
                previousConfirmation(g_sContext,rect,cursor);
                cursor--;
                if(cursor == 0)
                    cursor = 3;
            }
        }
    }

    //Confirm Button
    if(P5->IFG & BIT6 && !on_off){
        if(menuIndex == 1){
            if(cursor == 1){
                SYSCTL->REBOOT_CTL = 0x6901;
            }
            if(cursor == 2){
                drawPerformExperimentMenu(g_sContext, rect);
                cursor = 1;
                menuIndex = 3;
            }
            if(cursor == 3){
                drawSpeedControlMenu(g_sContext, rect);
                cursor = 1;
                menuIndex = 4;
                acquireSpeedData();
                DynamicMode = 1;
            }
            if(cursor == 4){
                wifiMode = 1;
                drawRemoteModeMenu(g_sContext, rect);
                transmitMessageToDynamicModule("\nremoteMode\n");
                UART0_Transmitter_Mode();
                UART0_Receiver_Mode();
            }
        }
        else if(menuIndex == 3){
            if(cursor == 1){
                if(rod_Length<500)
                    rod_Length++;
                updateRodLength(g_sContext,rod_Length,rect);

            }
            if(cursor == 2){
                drawParameterMenu(g_sContext,rect);
                menuIndex = 6;
                cursor = 1;
                int i = 0;
                for(i; i<3; i++)
                    if(selectedSpeed_Temp[i])
                        selectTempHumid(g_sContext, rect, i+3);
            }
            if(cursor == 3){
                drawMainMenu(g_sContext,rect);
                menuIndex = 1;
                cursor = 1;
            }
        }

        else if(menuIndex == 4){
            if(cursor == 1){
                P4->OUT &=  ~BIT0;
                Delay(2500);
                P4->OUT &=  ~BIT1;
                Delay(5000);
                P4->OUT |= BIT1;
                Delay(5000);
                P4->OUT &=  ~BIT1;
            }
            else if(cursor == 2){
                P4->OUT |=  BIT0;
                P4->OUT |=  BIT1;
            }
            else if(cursor == 3){
                transmitMessageToDynamicModule("\nstop\n");
                DynamicMode = 0;
                drawMainMenu(g_sContext,rect);
                menuIndex = 1;
                cursor = 1;
            }
        }
        else if(menuIndex == 5){
            if(cursor == 1){
                drawMainMenu(g_sContext,rect);
                menuIndex = 1;
                cursor = 1;
                diagnosticFlag = checkCommunication = 0;
                diagnosticPressure = false;
                diagnosticBalance = false;
            }
        }
        else if(menuIndex == 6){
            if(cursor == 1){
                drawPressureMenu(g_sContext,rect);
                menuIndex = 7;
                cursor = 1;
                int i = 1;
                for(i; i<= 9; i++)
                    if(selectedPressureSensors[i]){
                        selectSensor(g_sContext, rect, i);
                        pressureSensorsActive = 8;
                        pressureSelected = true;
                    }
            }
            if(cursor == 2){
                drawForceMenu(g_sContext,rect);
                menuIndex = 9;
                cursor = 1;
                int i = 0;
                for(i; i< 7; i++)
                    if(selectedWindForces[i])
                        selectForce(g_sContext, rect, i);
            }
            if(cursor == 3){
                if(selectedSpeed_Temp[0]){
                    deselectTempHumid(g_sContext, rect, cursor);
                    selectedSpeed_Temp[0] = 0;
                    humidSelected = false;
                }

                else{
                    selectTempHumid(g_sContext, rect, cursor);
                    selectedSpeed_Temp[0] = 1;
                    humidSelected = true;
                }
            }
            if(cursor == 4){
                if(selectedSpeed_Temp[1]){
                    deselectTempHumid(g_sContext, rect, cursor);
                    selectedSpeed_Temp[1] = 0;
                    tempSelected = false;
                }

                else{
                    selectTempHumid(g_sContext, rect, cursor);
                    selectedSpeed_Temp[1] = 1;
                    tempSelected = true;
                }
            }
            if(cursor == 5){
                if(selectedSpeed_Temp[2]){
                    deselectTempHumid(g_sContext, rect, cursor);
                    selectedSpeed_Temp[2] = 0;
                    speedSelected = false;
                }

                else{
                    selectTempHumid(g_sContext, rect, cursor);
                    selectedSpeed_Temp[2] = 1;
                    speedSelected = true;
                }
            }
            if(cursor == 6){
                drawExperimentDurationMenu(g_sContext,rect);
                menuIndex = 10;
                cursor = 1;
            }
            if(cursor == 7){
                drawMainMenu(g_sContext,rect);
                menuIndex = 1;
                cursor = 1;
            }
        }
        else if(menuIndex == 7){
            if(cursor == 1){
                if(selectedPressureSensors[cursor+10] == 1 || selectedPressureSensors[cursor] == 1){
                    int z = 1;
                    for(z = 1; z < 10;z++){
                        deselectSensor(g_sContext, rect, z);
                        selectedPressureSensors[z] = 0;
                    }
                    for(z = 1; z < 10;z++){
                        deselectSensor(g_sContext, rect, z);
                        selectedPressureSensors[z+10] = 0;
                    }
                    pressureSensorsActive = 0;
                    pressureSelected = false;
                }

                else{
                    int z = 1;
                    for(z = 1; z < 10;z++){
                        selectSensor(g_sContext, rect, z);
                        selectedPressureSensors[z] = 1;
                    }
                    for(z = 1; z < 10;z++){
                        selectSensor(g_sContext, rect, z);
                        selectedPressureSensors[z+10] = 1;
                    }
                    pressureSelected = true;
                    pressureSensorsActive = 16;
                }
            }
            else if(cursor > 1 && cursor < 10){
                if(selectedPressureSensors[cursor] == 1){
                    deselectSensor(g_sContext, rect, cursor);
                    selectedPressureSensors[cursor] = 0;
                    pressureSensorsActive--;
                    if(pressureSensorsActive == 0){
                        pressureSelected = false;
                    }
                }

                else{
                    selectSensor(g_sContext, rect, cursor);
                    selectedPressureSensors[cursor] = 1;
                    pressureSelected = true;
                    pressureSensorsActive++;
                }

            }

            if(cursor == 10){
                drawMorePressureMenu(g_sContext,rect);
                menuIndex = 8;
                cursor = 1;
                int i = 10;
                for(i; i<= 19; i++)
                    if(selectedPressureSensors[i])
                        selectSensor(g_sContext, rect, i-10);

            }
            if(cursor == 11){
                drawParameterMenu(g_sContext,rect);
                menuIndex = 6;
                cursor = 1;
                int i = 0;
                for(i; i<3; i++)
                    if(selectedSpeed_Temp[i])
                        selectTempHumid(g_sContext, rect, i+3);
            }
        }
        else if(menuIndex == 8){
            if(cursor == 1){
                if(selectedPressureSensors[cursor+10] == 1 || selectedPressureSensors[cursor] == 1){
                    int z = 1;
                    for(z = 1; z < 10;z++){
                        deselectSensor(g_sContext, rect, z);
                        selectedPressureSensors[z] = 0;
                    }
                    for(z = 1; z < 10;z++){
                        deselectSensor(g_sContext, rect, z);
                        selectedPressureSensors[z+10] = 0;
                    }

                    pressureSelected = false;
                }

                else{
                    int z = 1;
                    for(z = 1; z < 10;z++){
                        selectSensor(g_sContext, rect, z);
                        selectedPressureSensors[z] = 1;
                    }
                    for(z = 1; z < 10;z++){
                        selectSensor(g_sContext, rect, z);
                        selectedPressureSensors[z+10] = 1;
                    }
                    pressureSelected = true;
                }
            }
            else if(cursor > 1 && cursor < 10){
                if(selectedPressureSensors[cursor+10] == 1){
                    deselectSensor(g_sContext, rect, cursor);
                    selectedPressureSensors[cursor+10] = 0;
                    pressureSelected = false;
                }

                else{
                    selectSensor(g_sContext, rect, cursor);
                    selectedPressureSensors[cursor+10] = 1;
                    pressureSelected = true;
                }

            }
            else if(cursor == 10){

            }
            else if(cursor == 11){
                drawParameterMenu(g_sContext,rect);
                menuIndex = 6;
                int i = 0;
                cursor = 1;
                for(i; i<3; i++)
                    if(selectedSpeed_Temp[i])
                        selectTempHumid(g_sContext, rect, i+3);
            }
        }
        else if(menuIndex == 9){
            if(cursor == 1){
                if(selectedWindForces[cursor] == 1){
                    int z = 1;
                    for(z = 1; z < 7;z++){
                        deselectForce(g_sContext, rect, z);
                        selectedWindForces[z] = 0;
                    }
                    balanceSelected = false;
                }
                else{
                    int z = 1;
                    for(z = 1; z < 7;z++){
                        selectForce(g_sContext, rect, z);
                        selectedWindForces[z] = 1;
                    }
                    balanceSelected = true;
                }
            }
            else if(cursor > 1 && cursor < 7){
                if(selectedWindForces[cursor]){
                    deselectForce(g_sContext, rect, cursor);
                    selectedWindForces[cursor] = 0;
                    balanceSelected = false;
                }
                else{
                    selectForce(g_sContext, rect, cursor);
                    selectedWindForces[cursor] = 1;
                    balanceSelected = true;
                }
            }

            else if(cursor == 7){
                drawParameterMenu(g_sContext,rect);
                menuIndex = 6;
                cursor = 1;
                int i = 0;
                for(i; i<3; i++)
                    if(selectedSpeed_Temp[i])
                        selectTempHumid(g_sContext, rect, i+3);
            }
        }
        else if(menuIndex == 10){
            if(cursor == 1){
                if(seconds<300)
                    seconds++;
                updateTimeDuration(g_sContext,seconds,rect);

            }
            else if(cursor == 2){
                setMeasurmentsToSample(pressureSelected,balanceSelected, tempSelected, humidSelected, speedSelected);
                if(getMeasurmentsToSample() == 0){
                    displayNeedMeasurmentMessage();
                }
                else{
                    drawSampleMenu(g_sContext,rect, pressureSelected, balanceSelected, tempSelected, humidSelected, speedSelected);
                    menuIndex = 11;
                    cursor = 1;
                }
            }
            else if(cursor == 3){
                drawParameterMenu(g_sContext,rect);
                menuIndex = 6;
                cursor = 1;
                int i = 0;
                for(i; i<3; i++)
                    if(selectedSpeed_Temp[i])
                        selectTempHumid(g_sContext, rect, i+3);
            }
            else if(cursor == 4){
                drawMainMenu(g_sContext,rect);
                menuIndex = 1;
                cursor = 1;
            }
        }
        else if(menuIndex == 11){
            if(cursor == getMeasurmentsToSample()+2){
                drawExperimentDurationMenu(g_sContext,rect);
                menuIndex = 10;
                cursor = 1;
                resetSampleCounters();
            }
            else if(cursor == getMeasurmentsToSample()+1){
                drawExperimentConfirmationMenu(g_sContext,rect);
                menuIndex = 12;
                cursor = 1;
                setnumberOfDynamicSamples();
                readyDynamicData();
                setnumberOfBalanceSamples();
            }
            else if(cursor <= getMeasurmentsToSample()){
                increaseSampleSize(g_sContext, seconds, cursor);
            }
        }

        else if(menuIndex == 12){
            if(cursor == 1){
                if(ExperimentStatus == 1){
                    MAP_Timer_A_stopTimer(TIMER_A0_BASE);
                    drawExperimentConfirmationMenu(g_sContext,rect);
                    cursor = 1;
                    menuIndex = 12;
                    ExperimentStatus = 0;
                    clearNumberOfExperiments();
                }
                else if(ExperimentStatus == 2)
                {
                    if(pressureSelected){
                        drawAcquiredPressureModuleResults(g_sContext,rect,p1,p2,p3,p4,p5,p6,p7,p8,0);
                        menuIndex = 13;
                    }else if(tempSelected || humidSelected || speedSelected){
                        drawAcquiredDynamicModuleResults(g_sContext,rect,tempResult,humidResult,windSpeedResult);
                        menuIndex = 16;
                    }else if(balanceSelected){
                        drawAcquiredBalanceModuleResults(g_sContext,rect,BalanceModuleRxBuffer[0]);
                        menuIndex = 1;
                    }
                    cursor = 1;
                    ExperimentStatus = 0;
                    clearNumberOfExperiments();
                }
                else{
                    ExperimentStatus = 1;
                    StopExperimentMessage(g_sContext,rect);
                    secondsPassed = (numberOfExperiments+1)*seconds;
                    acquiredMeasurementDuration = secondsPassed;
                    uart0MessageIndex, uart1MessageIndex, uart2MessageIndex = 0;
                    MAP_Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
                    /* Enabling MASTER interrupts */
                    MAP_Interrupt_enableMaster();
                }
            }
            else if(cursor == 2){
                drawSampleMenu(g_sContext,rect, pressureSelected, balanceSelected, tempSelected, humidSelected, speedSelected);
                menuIndex = 11;
                cursor = 1;
                clearNumberOfExperiments();
            }
            else if(cursor == 3){
                drawMainMenu(g_sContext,rect);
                menuIndex = 1;
                cursor = 1;
                clearNumberOfExperiments();
            }
        }
        else if(menuIndex == 13){
            drawAcquiredPressureModuleResults(g_sContext,rect,p1,p2,p3,p4,p5,p6,p7,p8,1);
            menuIndex = 14;
        }
        else if(menuIndex == 14){
            drawAcquiredPressureModuleResults(g_sContext,rect,p1,p2,p3,p4,p5,p6,p7,p8,2);
            menuIndex = 15;
        }
        else if(menuIndex == 15){
            if(tempSelected || humidSelected || speedSelected){
                drawAcquiredDynamicModuleResults(g_sContext,rect,tempResult,humidResult,windSpeedResult);
                menuIndex = 16;
            }
            else if(balanceSelected){
                drawAcquiredBalanceModuleResults(g_sContext,rect,ForcesArray);
                menuIndex = 17;
            }
            else{
                menuIndex = 1;
                cursor = 1;
                drawMainMenu(g_sContext,rect);
            }
        }else if(menuIndex == 16){
            if(balanceSelected){
                drawAcquiredBalanceModuleResults(g_sContext,rect,ForcesArray);
                menuIndex = 17;
            }
            else{
                menuIndex = 1;
                cursor = 1;
                drawMainMenu(g_sContext,rect);
            }
        }
        else if(menuIndex == 17){
            menuIndex = 1;
            cursor = 1;
            drawMainMenu(g_sContext,rect);
        }
    }
    if(P5->IFG & BIT7 && !on_off){
        if(menuIndex == 3){
            if(cursor == 1){
                if(rod_Length>1)
                    rod_Length--;
                updateRodLength(g_sContext,rod_Length,rect);

            }
        }
        if(menuIndex == 10){
            if(seconds>5)
                seconds--;
            updateTimeDuration(g_sContext,seconds,rect);

        }
        if(menuIndex == 11){
            decrementSampleSize(g_sContext,cursor);
        }
        if(menuIndex == 12){
            decreaseExpRepetitions(seconds);
        }
    }

    P5->IFG = 0;
}

void TA0_0_IRQHandler(void)
{
    if(secondsPassed){
        secondsPassed--;
        updateAcquisitionTime(g_sContext,acquiredMeasurementDuration-secondsPassed);
        if((acquiredMeasurementDuration-secondsPassed) == 1){
            P1->OUT |= BIT5;
            if(pressureSelected){
                uart2MessageIndex = 0;
                acquirePressureModuleData(selectedPressureSensors,sampleCounter[0],durationBuff);
            }
            if(tempSelected || humidSelected || speedSelected){
                uart0MessageIndex = 0;
                acquireDynamicModuleData(tempBuff,humBuff,speedBuff,durationBuff);
            }
            if(balanceSelected){
                uart1MessageIndex = 0;
                acquireBalanceModuleData(numberOfBalanceSamples,acquiredMeasurementDuration,rod_Length);
            }
        }

    }
    else{
        ExperimentStatus = 2;
        parseDynamicResults();
        parsePressureResults();
        ViewResultsMessage(g_sContext,rect);
        MAP_Timer_A_stopTimer(TIMER_A0_BASE);
        P1->OUT &= ~BIT5;
    }


    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0);
}

void EUSCIA0_IRQHandler(void){
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);

    MAP_UART_clearInterruptFlag(EUSCI_A0_BASE, status);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        char inByte = MAP_UART_receiveData(EUSCI_A0_BASE);
        if (inByte == '\n') {

            if(DynamicMode){
                updateWindSpeed(g_sContext,inBytesDynamic);
            }

            if(tempSelected || humidSelected || speedSelected){
                strcpy(DynamicModuleRxBuffer[uart0MessageIndex],inBytesDynamic);
            }
            uart0MessageIndex++;
            indexDynamic = 0;
        }

        else if (indexDynamic >= maxBytes - 1) {
            indexDynamic = 0;
        }
        else {
            inBytesDynamic[indexDynamic++] = inByte;
        }

    }
}

void EUSCIA1_IRQHandler(void){
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A1_BASE);

    MAP_UART_clearInterruptFlag(EUSCI_A1_BASE, status);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        char inByte = MAP_UART_receiveData(EUSCI_A1_BASE);
        if (inByte == '\n') {
            int i = 0;
            for(i = 0; i < 25; i++)
                BalanceModuleRxBuffer[uart1MessageIndex][i] = inBytesBalance[i];
            indexBalance = 0;
            uart1MessageIndex++;
        }
        else if (indexBalance >= maxBytes - 1) {
            indexBalance = 0;
        }
        else {
            inBytesBalance[indexBalance++] = inByte;
        }
    }
}

void EUSCIA2_IRQHandler(void){
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A2_BASE);

    MAP_UART_clearInterruptFlag(EUSCI_A2_BASE, status);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        char inByte = MAP_UART_receiveData(EUSCI_A2_BASE);
        if (inByte == '\n') {

            if(pressureSelected){
                strcpy(PressureModuleRxBuffer[uart2MessageIndex],inBytesPressure);
                uart2MessageIndex++;
            }
            if(uart2MessageIndex == 2){

            }
            indexPressure = 0;
        }
        else if (indexPressure >= maxBytes - 1) {
            indexPressure = 0;
        }
        else {
            inBytesPressure[indexPressure++] = inByte;
        }
    }
}
