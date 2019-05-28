/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include "math.h"
#include <stdlib.h>
#include <string.h>

unsigned int ADC_value13[6] = {0, 0, 0, 0, 0, 0};   //Digital code of ADC channel 13 conversion, four samples are acquired
unsigned int ADC_value12[6] = {0, 0, 0, 0, 0, 0};   //Digital code of ADC channel 12 conversion, four samples are acquired
unsigned int ADC_value11[6] = {0, 0, 0, 0, 0, 0};   //Digital code of ADC channel 11 conversion, four samples are acquired
unsigned int ADC_value10[6] = {0, 0, 0, 0, 0, 0};   //Digital code of ADC channel 10 conversion, four samples are acquired
unsigned int ADC_value9[6] = {0, 0, 0, 0, 0, 0};    //Digital code of ADC channel 9 conversion, four samples are acquired
float ADC_volt13[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; //Analog voltages obtained in ADC channel 13
float ADC_volt12[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; //Analog voltages obtained in ADC channel 12
float ADC_volt11[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; //Analog voltages obtained in ADC channel 11
float ADC_volt10[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; //Analog voltages obtained in ADC channel 10
float ADC_volt9[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};  //Analog voltages obtained in ADC channel 9
float dragf_zero = 0.0; //Zero weight drag front conditioned voltage
float dragb_zero = 0.0; //Zero weight drag back conditioned voltage
float right_zero = 0.0; //Zero weight right side conditioned voltage
float left_zero = 0.0;  //Zero weight left side conditioned voltage
float lift_zero = 0.0;  //Zero weight lift conditioned voltage
float ADC_volt_dragf_sum = 0.0; //Analog voltage accumulator used to average drag front conditioned voltage
float ADC_volt_dragf_ave = 0.0; //Average voltage obtained in drag front load cell
float ADC_volt_dragb_sum = 0.0; //Analog voltage accumulator used to average drag back conditioned voltage
float ADC_volt_dragb_ave = 0.0; //Average voltage obtained in drag back load cell
float ADC_volt_right_sum = 0.0; //Analog voltage accumulator used to average right side conditioned voltage
float ADC_volt_right_ave = 0.0; //Average voltage obtained in right side load cell
float ADC_volt_left_sum = 0.0;  //Analog voltage accumulator used to average left side conditioned voltage
float ADC_volt_left_ave = 0.0;  //Average voltage obtained in left side load cell
float ADC_volt_lift_sum = 0.0;  //Analog voltage accumulator used to average lift conditioned voltage
float ADC_volt_lift_ave = 0.0;  //Average voltage obtained in lift load cell
float dragf_error_factor = 0.0; //Error factor in drag front load cell
float dragb_error_factor = 0.0; //Error factor in drag back load cell
float right_error_factor = 0.0; //Error factor in right side load cell
float left_error_factor = 0.0;  //Error factor in left side load cell
float lift_error_factor = 0.0;  //Error factor in lift load cell
float drag_front_weight_lb = 0.0;   //Drag front weight in lb
float drag_front_force_lb = 0.0;    //Drag front force in lb
float drag_front_weight_N = 0.0;    //Drag front weight in N
float drag_front_force_N = 0.0; //Drag front force in N
float drag_back_weight_lb = 0.0;    //Drag back weight in lb
float drag_back_force_lb = 0.0; //Drag back force in lb
float drag_back_weight_N = 0.0; //Drag back weight in N
float drag_back_force_N = 0.0;  //Drag back force in N
float right_side_weight_lb = 0.0;   //Right side weight in lb
float right_side_weight_N = 0.0;    //Right side weight in N
float left_side_weight_lb = 0.0;    //Left side weight in lb
float left_side_weight_N = 0.0; //Left side weight in N
float lift_weight_lb = 0.0; //Lift weight in lb
float lift_force_lb = 0.0;  //Lift force in lb
float lift_weight_N = 0.0;  //Lift weight in N
float lift_force_N = 0.0;   //Lift force in N
float L1 = 14.605; //Distance between balance support point and drag load cell in cm
float L2 = 16.748; //Distance between balance support point and rod base in cm
float L3_base_rod = 19.288; //Height of rod base in cm
float L4 = 18.971; //Distance between balance support and lift load cell in cm
char (results[5])[5];  //Stores the forces results
char force[1500];   //Stores the force results in string format to send to the Master Module
int forceCounter = 25;  //Index for force[1500] variable
int i = 0; //Index used to accumulate analog voltages
int n_dragf = 0;    //Rate of change between difference of drag front conditioned output voltage and 2.64V, and 0.01 used for error correction
int n_dragb = 0;    //Rate of change between difference of drag back conditioned output voltage and 2.64V, and 0.01 used for error correction
int n_right = 0;    //Rate of change between difference of right side conditioned output voltage and 2.64V, and 0.01 used for error correction
int n_left = 0; //Rate of change between difference of left side conditioned output voltage and 2.64V, and 0.01 used for error correction
int n_lift = 0; //Rate of change between difference of lift conditioned output voltage and 2.64V, and 0.01 used for error correction
int durationTime;   //Duration time of experiment specified by the user in the Master Module
const int maxBytes = 512;   //Maximum number of bytes to send commands
uint8_t inBytes[maxBytes];
uint8_t index = 0;
int sampleNum, sampleCounter = 0;   //Number of samples
int rod_length = 0; //Rod length specified by the user in the Master Module


// Delay function
void delay(int del){
    int i = 0;
    for(; i < del; i++);
}
//![Simple UART Config]
/* UART Configuration Parameter. These are the configuration parameters to
 * make the eUSCI A UART module to operate with a 9600 baud rate. These
 * values were calculated using the online calculator that TI provides
 * at:
 *http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 */
const eUSCI_UART_Config uartConfig =
{
 EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
 78,                                     // BRDIV = 78
 2,                                       // UCxBRF = 2
 0,                                       // UCxBRS = 0
 EUSCI_A_UART_NO_PARITY,                  // No Parity
 EUSCI_A_UART_LSB_FIRST,                  // LSB First
 EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
 EUSCI_A_UART_MODE,                       // UART mode
 EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  // Oversampling
};
//![Simple UART Config]

int main(void)
{
    /* Halting WDT  */
    MAP_WDT_A_holdTimer();

    /* Selecting P1.2 and P1.3 in UART mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
                                                   GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Setting DCO to 12MHz */
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);

    //![Simple UART Example]
    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);

    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A0_BASE);

    /* Enabling interrupts */
    MAP_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_Interrupt_enableInterrupt(INT_EUSCIA0);
    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_Interrupt_enableMaster();   


    //ERROR FAULT LED
    P1->DIR |= BIT7;
    P1->OUT &= ~BIT7;
    /* BALANCE CODE */
    //PORT 4 Setup
    P4->SEL0 |= BIT0;   //Set pin 4.0 as Analog input A13
    P4->SEL1 |= BIT0;

    P4->SEL0 |= BIT1;   //Set pin 4.1 as Analog input A12
    P4->SEL1 |= BIT1;

    P4->SEL0 |= BIT2;   //Set pin 4.2 as Analog input A11
    P4->SEL1 |= BIT2;

    P4->SEL0 |= BIT3;   //Set pin 4.3 as Analog input A10
    P4->SEL1 |= BIT3;

    P4->SEL0 |= BIT4;   //Set pin 4.4 as Analog input A9
    P4->SEL1 |= BIT4;


    //PORT 5 Setup
    P5->SEL0 |= BIT6;   //Set pin 5.6 as external VeRef+
    P5->SEL1 |= BIT6;

    P5->SEL0 |= BIT7;   //Set pin 5.7 as external VeRef-
    P5->SEL1 |= BIT7;

    //ADC Configuration Setup
    ADC14->CTL0 &= ~ADC14_CTL0_ENC; //Disable Conversion before ADC setup
    ADC14->CTL0 = ADC14_CTL0_CONSEQ_3;  //Repeat sequence of channels mode
    ADC14->CTL0 |= ADC14_CTL0_SHT0__128 | ADC14_CTL0_SHT1__128 | ADC14_CTL0_SSEL__MODCLK | ADC14_CTL0_SHP | ADC14_CTL0_MSC | ADC14_CTL0_ON;    //Sample and Hold time setup, SAMPCON sourced
    //from sampling timer, Multiple sample conversions and ADC ON
    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_13 | ADC14_MCTLN_VRSEL_14; //Channel 13 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[1] |= ADC14_MCTLN_INCH_13 | ADC14_MCTLN_VRSEL_14; //Channel 13 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[2] |= ADC14_MCTLN_INCH_13 | ADC14_MCTLN_VRSEL_14;    //Channel 13 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[3] |= ADC14_MCTLN_INCH_13 | ADC14_MCTLN_VRSEL_14;    //Channel 13 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[4] |= ADC14_MCTLN_INCH_13 | ADC14_MCTLN_VRSEL_14;    //Channel 13 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[5] |= ADC14_MCTLN_INCH_13 | ADC14_MCTLN_VRSEL_14;    //Channel 13 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[6] |= ADC14_MCTLN_INCH_12 | ADC14_MCTLN_VRSEL_14;    //Channel 12 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[7] |= ADC14_MCTLN_INCH_12 | ADC14_MCTLN_VRSEL_14;    //Channel 12 selected  and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[8] |= ADC14_MCTLN_INCH_12 | ADC14_MCTLN_VRSEL_14;    //Channel 12 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[9] |= ADC14_MCTLN_INCH_12 | ADC14_MCTLN_VRSEL_14;    //Channel 12 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[10] |= ADC14_MCTLN_INCH_12 | ADC14_MCTLN_VRSEL_14;    //Channel 12 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[11] |= ADC14_MCTLN_INCH_12 | ADC14_MCTLN_VRSEL_14;    //Channel 12 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[12] |= ADC14_MCTLN_INCH_11 | ADC14_MCTLN_VRSEL_14;    //Channel 11 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[13] |= ADC14_MCTLN_INCH_11 | ADC14_MCTLN_VRSEL_14;    //Channel 11 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[14] |= ADC14_MCTLN_INCH_11 | ADC14_MCTLN_VRSEL_14;  //Channel 11 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[15] |= ADC14_MCTLN_INCH_11 | ADC14_MCTLN_VRSEL_14;    //Channel 11 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[16] |= ADC14_MCTLN_INCH_11 | ADC14_MCTLN_VRSEL_14;    //Channel 11 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[17] |= ADC14_MCTLN_INCH_11 | ADC14_MCTLN_VRSEL_14;    //Channel 11 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[18] |= ADC14_MCTLN_INCH_10 | ADC14_MCTLN_VRSEL_14;    //Channel 10 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[19] |= ADC14_MCTLN_INCH_10 | ADC14_MCTLN_VRSEL_14;    //Channel 10 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[20] |= ADC14_MCTLN_INCH_10 | ADC14_MCTLN_VRSEL_14;    //Channel 10 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[21] |= ADC14_MCTLN_INCH_10 | ADC14_MCTLN_VRSEL_14;    //Channel 10 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[22] |= ADC14_MCTLN_INCH_10 | ADC14_MCTLN_VRSEL_14;    //Channel 10 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[23] |= ADC14_MCTLN_INCH_10 | ADC14_MCTLN_VRSEL_14;    //Channel 10 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[24] |= ADC14_MCTLN_INCH_9 | ADC14_MCTLN_VRSEL_14;    //Channel 9 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[25] |= ADC14_MCTLN_INCH_9 | ADC14_MCTLN_VRSEL_14;    //Channel 9 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[26] |= ADC14_MCTLN_INCH_9 | ADC14_MCTLN_VRSEL_14;    //Channel 9 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[27] |= ADC14_MCTLN_INCH_9 | ADC14_MCTLN_VRSEL_14;    //Channel 9 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[28] |= ADC14_MCTLN_INCH_9 | ADC14_MCTLN_VRSEL_14;    //Channel 9 selected and Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->MCTL[29] |= ADC14_MCTLN_INCH_9 | ADC14_MCTLN_VRSEL_14 | ADC14_MCTLN_EOS;    //Channel 9 selected, end of sequence and
    //Vref+ = VeRef+ = 3.3V, Vref- = VeRef- = GND
    ADC14->CTL1 |= ADC14_CTL1_RES_3; //Resolution of ADC to 14 bits
    ADC14->IER0 &= ~ADC14_IER0_IE0; //Disable ADC interrupt flag
    /////////////////////////////
}

/* EUSCI A0 UART ISR - Echoes data back to PC host */
void EUSCIA0_IRQHandler(void)
{
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);

    MAP_UART_clearInterruptFlag(EUSCI_A0_BASE, status);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        char inByte = MAP_UART_receiveData(EUSCI_A0_BASE);
        if (inByte == '\n') {

            if(inBytes[0] =='S' && inBytes[1] == 'N'){
                sampleNum = (inBytes[2] - '0')*10 + (inBytes[3] - '0'); //Receives number of samples by Master Module
                rod_length = (inBytes[4] - '0')*100 + (inBytes[5] - '0')*10 + (inBytes[6] - '0');   //Receives rod length by Master Module

                while(sampleNum)
                {
                    P1->OUT |= BIT7;
                    acquireForces();    //Acquire forces
                    sampleNum--;
                }
                transmitMessage(force); //Transmit force results to Master Module
                P1->OUT &= ~BIT7;
                inBytes[0] = '0';
                index = 0;
                delay(5000);
                SYSCTL->REBOOT_CTL = 0x6901;
            }
            else if(inBytes[0] == 'W' && inBytes[1] == 'Y'){
                acquireForces();    //Acquires load cells conditioned voltages without wind
                index = 0;
                inBytes[0] = '0';
            }

        }
        else if (index >= maxBytes - 1) {
            index = 0;
        }
        else {
            inBytes[index++] = inByte;
        }
    }
}


//Function to transmit a message using UART protocol
void transmitMessage(char msg[])
{
    uint8_t *ptr = msg;
    while (*ptr) {
        MAP_UART_transmitData(EUSCI_A0_BASE, *ptr++);
    }
}


void acquireForces(void){
    ADC14->CTL0 |= ADC14_CTL0_ENC | //Enable and Start Conversion
            ADC14_CTL0_SC;
    delay(1500);    //Delay to give time for ADC conversions
    ADC14->CTL0 &= ~ADC14_CTL0_ENC; //Disable Conversion

    while(ADC14->IFGR0 != 0x00000000)
    {
        if(i < 6)
        {

            ADC_value13[i] = ADC14->MEM[i]; //Stores drag front load cell conditioned output voltages digital codes
            ADC_volt13[i] = ((3.3*ADC_value13[i])/(16384.0));   //Stores drag front load cell conditioned output voltages
            ADC_value12[i] = ADC14->MEM[i+6];   //Stores drag back load cell conditioned output voltages digital codes
            ADC_volt12[i] = ((3.3*ADC_value12[i])/(16384.0));   //Stores drag back load cell conditioned output voltages
            ADC_value11[i] = ADC14->MEM[i+12];   //Stores right side load cell conditioned output voltages digital codes
            ADC_volt11[i] = ((3.3*ADC_value11[i])/(16384.0));   //Stores right side load cell conditioned output voltages
            ADC_value10[i] = ADC14->MEM[i+18];  //Stores left side load cell conditioned output voltages digital codes
            ADC_volt10[i] = ((3.3*ADC_value10[i])/(16384.0));   //Stores left side load cell conditioned output voltages
            ADC_value9[i] = ADC14->MEM[i+24];   //Stores lift load cell conditioned output voltages digital codes
            ADC_volt9[i] = ((3.3*ADC_value9[i])/(16384.0)); //Stores lift load cell conditioned output voltages
            ADC_volt_dragf_sum = ADC_volt_dragf_sum + ADC_volt13[i];  //Accumulates drag front load cell conditioned output voltages
            ADC_volt_dragb_sum = ADC_volt_dragb_sum + ADC_volt12[i];  //Accumulates drag back load cell conditioned output voltages
            ADC_volt_right_sum = ADC_volt_right_sum + ADC_volt11[i];  //Accumulates right side load cell conditioned output voltages
            ADC_volt_left_sum = ADC_volt_left_sum + ADC_volt10[i];  //Accumulates left side load cell conditioned output voltages
            ADC_volt_lift_sum = ADC_volt_lift_sum + ADC_volt9[i];   //Accumulates lift load cell conditioned output voltages
            i++;
        }
        if(i == 6)
        {
            ADC_volt_dragf_ave = ADC_volt_dragf_sum/6;    //Calculates the average of drag front load cell conditioned output
            ADC_volt_dragf_sum = 0;
            ADC_volt_dragb_ave = ADC_volt_dragb_sum/6;  //Calculates the average of drag back load cell conditioned output
            ADC_volt_dragb_sum = 0;
            ADC_volt_right_ave = ADC_volt_right_sum/6;  //Calculates the average of right side load cell conditioned output
            ADC_volt_right_sum = 0;
            ADC_volt_left_ave = ADC_volt_left_sum/6;    //Calculates the average of left side load cell conditioned output
            ADC_volt_left_sum = 0;
            ADC_volt_lift_ave = ADC_volt_lift_sum/6;    //Calculates the average of lift load cell conditioned output
            ADC_volt_lift_sum = 0;
            i = 0;

            if(dragf_zero != 0.0)
            {
                //Analog voltage to drag front force conversion
                if(ADC_volt_dragf_ave >= (dragf_zero - 0.005) && ADC_volt_dragf_ave < (dragf_zero + 0.015))    //No weight in load cell
                {
                    drag_front_weight_lb = 0.00;
                }
                else if(ADC_volt_dragf_ave >= (dragf_zero + 0.025) && ADC_volt_dragf_ave <= 3.3)   //Weight in load cell
                {
                    if(ADC_volt_dragf_ave < (dragf_zero + 0.04))    //Used when weight in load cell is between 1lb and 5lb
                    {
                        drag_front_weight_lb = ((ADC_volt_dragf_ave-dragf_zero)/(1+100/1.4))*(45/0.01);
                    }
                    else
                    {
                        //Error correction factor when weight is greater or equal to 5lb
                        n_dragf = (ADC_volt_dragf_ave - (dragf_zero + 0.03))/0.01;
                        dragf_error_factor = 0.0004 + (n_dragf-1)*0.00015;

                        //Drag front force conversion using INA333 gain, load cells Voltage (mV) vs Weight (lb) transfer characteristic and
                        //error correction factor
                        drag_front_weight_lb = (((ADC_volt_dragf_ave-dragf_zero)/(1+100/1.4))+dragf_error_factor)*(45/0.01);
                    }
                }
                else if(ADC_volt_dragf_ave > 3.3) //When maximum weight (45lb) is exceeded
                {
                    drag_front_weight_lb = 45.00;
                }
                else    //Default
                {
                    //Drag front force conversion using INA333 gain and load cells Voltage (mV) vs Weight (lb) transfer characteristic, no error
                    //correction factor is needed (error doesn't affect measurement accuracy)
                    drag_front_weight_lb = ((ADC_volt_dragf_ave-dragf_zero)/(1+100/1.4))*(45/0.01);
                }
                if(drag_front_weight_lb < 0)
                {
                    drag_front_weight_lb = drag_front_weight_lb*(-1);
                }
                else
                {
                    drag_front_weight_lb = drag_front_weight_lb;
                }
                drag_front_force_lb = drag_front_weight_lb*(L2/L1); //Calculates drag front force in lb
                sprintf(results[0],"%2.2f",drag_front_force_lb);
            }
            else
            {
                dragf_zero = ADC_volt_dragf_ave;    //Stores the drag front load cell conditioned voltage without wind
            }

            if(dragb_zero != 0.0)
            {
                //Analog voltage to drag back force conversion
                if(ADC_volt_dragb_ave >= (dragb_zero - 0.005) && ADC_volt_dragb_ave < (dragb_zero + 0.015))    //No weight in load cell
                {
                    drag_back_weight_lb = 0.00;
                }
                else if(ADC_volt_dragb_ave >= (dragb_zero + 0.025) && ADC_volt_dragb_ave <= 3.3)   //Weight in load cell
                {
                    if(ADC_volt_dragb_ave < (dragb_zero + 0.04))    //Used when weight in load cell is between 1lb and 5lb
                    {
                        drag_back_weight_lb = ((ADC_volt_dragb_ave-dragb_zero)/(1+100/1.4))*(45/0.01);
                    }
                    else
                    {
                        //Error correction factor when weight is greater or equal to 5lb
                        n_dragb = (ADC_volt_dragb_ave - (dragb_zero + 0.03))/0.01;
                        dragb_error_factor = 0.0004 + (n_dragb-1)*0.00015;

                        //Drag back force conversion using INA333 gain, load cells Voltage (mV) vs Weight (lb) transfer characteristic and
                        //error correction factor
                        drag_back_weight_lb = (((ADC_volt_dragb_ave-dragb_zero)/(1+100/1.4))+dragb_error_factor)*(45/0.01);
                    }
                }
                else if(ADC_volt_dragb_ave > 3.3) //When maximum weight (45lb) is exceeded
                {
                    drag_back_weight_lb = 45.00;
                }
                else    //Default
                {
                    //Drag back force conversion using INA333 gain and load cells Voltage (mV) vs Weight (lb) transfer characteristic, no error
                    //correction factor is needed (error doesn't affect measurement accuracy)
                    drag_back_weight_lb = ((ADC_volt_dragb_ave-dragb_zero)/(1+100/1.4))*(45/0.01);
                }
                if(drag_back_weight_lb < 0)
                {
                    drag_back_weight_lb = drag_back_weight_lb*(-1);
                }
                else
                {
                    drag_back_weight_lb = drag_back_weight_lb;
                }
                drag_back_force_lb = drag_back_weight_lb*(L2/L1);   //Calculates drag back force in lb
                sprintf(results[1],"%2.2f",drag_back_force_lb);

            }
            else
            {
                dragb_zero = ADC_volt_dragb_ave;    //Stores the drag back load cell conditioned voltage without wind
            }

            if(right_zero != 0.0)
            {
                //Analog voltage to right side force conversion
                if(ADC_volt_right_ave >= (right_zero - 0.005) && ADC_volt_right_ave < (right_zero + 0.015))    //No weight in load cell
                {
                    right_side_weight_lb = 0.00;
                }
                else if(ADC_volt_right_ave >= (right_zero + 0.025) && ADC_volt_right_ave <= 3.3)   //Weight in load cell
                {
                    if(ADC_volt_right_ave < (right_zero + 0.04))    //Used when weight in load cell is between 1lb and 5lb
                    {
                        right_side_weight_lb = ((ADC_volt_right_ave-right_zero)/(1+100/1.4))*(45/0.01);
                    }
                    else
                    {
                        //Error correction factor when weight is greater or equal to 5lb
                        n_right = (ADC_volt_right_ave - (right_zero + 0.03))/0.01;
                        right_error_factor = 0.0004 + (n_right-1)*0.00015;

                        //Right side force conversion using INA333 gain, load cells Voltage (mV) vs Weight (lb) transfer characteristic and
                        //error correction factor
                        right_side_weight_lb = (((ADC_volt_right_ave-right_zero)/(1+100/1.4))+right_error_factor)*(45/0.01);
                    }
                }
                else if(ADC_volt_right_ave > 3.3) //When maximum weight (45lb) is exceeded
                {
                    right_side_weight_lb = 45.00;
                }
                else    //Default
                {
                    //Right side force conversion using INA333 gain and load cells Voltage (mV) vs Weight (lb) transfer characteristic, no error
                    //correction factor is needed (error doesn't affect measurement accuracy)
                    right_side_weight_lb = ((ADC_volt_right_ave-right_zero)/(1+100/1.4))*(45/0.01);
                }
                if(right_side_weight_lb < 0)
                {
                    right_side_weight_lb = right_side_weight_lb*(-1);
                }
                else
                {
                    right_side_weight_lb = right_side_weight_lb;
                }
                sprintf(results[2],"%2.2f",right_side_weight_lb);
            }
            else
            {
                right_zero = ADC_volt_right_ave;    //Stores the right side load cell conditioned voltage without wind
            }

            if(left_zero != 0.0)
            {
                //Analog voltage to left side force conversion
                if(ADC_volt_left_ave >= (left_zero - 0.005) && ADC_volt_left_ave < (left_zero + 0.015))    //No weight in load cell
                {
                    left_side_weight_lb = 0.00;
                }
                else if(ADC_volt_left_ave >= (left_zero + 0.025) && ADC_volt_left_ave <= 3.3)   //Weight in load cell
                {
                    if(ADC_volt_left_ave < (left_zero + 0.04))    //Used when weight in load cell is between 1lb and 5lb
                    {
                        left_side_weight_lb = ((ADC_volt_left_ave-left_zero)/(1+100/1.4))*(45/0.01);
                    }
                    else
                    {
                        //Error correction factor when weight is greater or equal to 5lb
                        n_left = (ADC_volt_left_ave - (left_zero + 0.03))/0.01;
                        left_error_factor = 0.0004 + (n_left-1)*0.00015;

                        //Left side force conversion using INA333 gain, load cells Voltage (mV) vs Weight (lb) transfer characteristic and
                        //error correction factor
                        left_side_weight_lb = (((ADC_volt_left_ave-left_zero)/(1+100/1.4))+left_error_factor)*(45/0.01);
                    }
                }
                else if(ADC_volt_left_ave > 3.3) //When maximum weight (45lb) is exceeded
                {
                    left_side_weight_lb = 45.00;
                }
                else    //Default
                {
                    //Left side force conversion using INA333 gain and load cells Voltage (mV) vs Weight (lb) transfer characteristic, no error
                    //correction factor is needed (error doesn't affect measurement accuracy)
                    left_side_weight_lb = ((ADC_volt_left_ave-left_zero)/(1+100/1.4))*(45/0.01);
                }
                if(left_side_weight_lb < 0)
                {
                    left_side_weight_lb = left_side_weight_lb*(-1);
                }
                else
                {
                    left_side_weight_lb = left_side_weight_lb;
                }
                sprintf(results[3],"%2.2f",left_side_weight_lb);

            }
            else
            {
                left_zero = ADC_volt_left_ave;  //Stores the left side load cell conditioned voltage without wind
            }

            if(lift_zero != 0)
            {
                //Analog voltage to lift force conversion
                if(ADC_volt_lift_ave >= (lift_zero - 0.005) && ADC_volt_lift_ave < (lift_zero + 0.015))    //No weight in load cell
                {
                    lift_weight_lb = 0.00;
                }
                else if(ADC_volt_lift_ave >= (lift_zero + 0.025) && ADC_volt_lift_ave <= 3.3)   //Weight in load cell
                {
                    if(ADC_volt_lift_ave < (lift_zero + 0.04))    //Used when weight in load cell is between 1lb and 5lb
                    {
                        lift_weight_lb = ((ADC_volt_lift_ave-lift_zero)/(1+100/1.4))*(45/0.01);
                    }
                    else
                    {
                        //Error correction factor when weight is greater or equal to 5lb
                        n_lift = (ADC_volt_lift_ave - (lift_zero + 0.03))/0.01;
                        lift_error_factor = 0.0004 + (n_lift-1)*0.00015;

                        //Lift weight conversion using INA333 gain, load cells Voltage (mV) vs Weight (lb) transfer characteristic and
                        //error correction factor
                        lift_weight_lb = (((ADC_volt_lift_ave-lift_zero)/(1+100/1.4))+lift_error_factor)*(45/0.01);
                    }
                }
                else if(ADC_volt_lift_ave > 3.3) //When maximum weight (45lb) is exceeded
                {
                    lift_weight_lb = 45.00;
                }
                else    //Default
                {
                    //Lift weight conversion using INA333 gain and load cells Voltage (mV) vs Weight (lb) transfer characteristic, no error
                    //correction factor is needed (error doesn't affect measurement accuracy)
                    lift_weight_lb = ((ADC_volt_lift_ave-lift_zero)/(1+100/1.4))*(45/0.01);
                }
                if(lift_weight_lb < 0)
                {
                    lift_weight_lb = lift_weight_lb*(-1);
                }
                else
                {
                    lift_weight_lb = lift_weight_lb;
                }
                lift_force_lb = lift_weight_lb*(L4/L3_base_rod + rod_length); //Calculates lift force in lb
                sprintf(results[4],"%2.2f",lift_force_lb);

                //Store force results to convert to character before sending to Master Module
                for(i = 0; i< 5; i++){
                    if(results[i][3] == 0){
                        results[i][3] = '0';
                    }
                    if(results[i][4] == 0){
                        results[i][4] = '0';
                    }
                }
                strcat(force, results[0]);
                force[forceCounter] = '\n';
                force[forceCounter+1] = 0;
                forceCounter +=25;
                i = 0;
            }
            else
            {
                lift_zero = ADC_volt_lift_ave;  //Stores the lift load cell conditioned voltage without wind
            }
        }
    }
}
