/*
                        Pressure Module Master Code
 Pressure Module Master Explanation
 This code will belong to the Pressure Module directly connected to the 
 Master Module of the system. The Pressure Module Master will have an I2C 
 bus for other Pressure Modules that can be added in order to increase the 
 number of barometric pressure sensors that can be connected.

 Pressure Module Master Design Constraints
 The Pressure Module Master can support up to 16 barometric pressure sensors
 using the current protoboard design. In order to increase the 
 number of sensors, another microcontroller must be added to this module 
 via an I2C connection. Several variables will need to be modified in order for
 the addition of other Pressure Modules be valid and acceptable. Please remember
 to use the Pressure Module Slave code for the Pressure Modules to be added to the
 I2C bus and upload the code to their microcontroller.

 Pressure Module Master Wiring
 When connecting the barometric pressure sensors to the protoboard, please check that
 the jumper connections are correct beforehand to prevent any possible internal
 damage to the pressure sensor device, as well as to prevent data adquisition
 error, since it won't acquire the proper pressure measurement if not connected 
 correctly. Make sure that each jumper are connected between the barometric
 pressure sensor and the protoboard with the correct label names (e.g. VIN to VIN, 
 SCK to SCLK, etc.).
 
 Adafruit Industries Supportability
 These barometric pressure sensors are supported by Adafruit Industries, which means that the
 configuration of these pressure sensors are explained in their datasheet and can be done through
 the libraries provided by the industry. 
 */

/* Libraries used for the source code. */
#include <Wire.h>
#include <SPI.h>
#include "Adafruit_BMP280.h"
#include "Adafruit_Sensor.h"

/* Data acquisition variables */
int expTime;
int pressureSamples;
boolean enableSensor[16];

/* UART constants */
boolean newData = false;
const int maxBytes = 128;
char inBytes[maxBytes];

/* Results parameters */
typedef struct {
  float pressures[16];
  unsigned long pressure_ts[16];
} Measurement;

/* Labels for the pins to use and their respective ports. */
#define BMP_SCK  (13)       //SPI Clock source
#define BMP_MISO (12)       //SPI Master In Slave Out (SDO)
#define BMP_MOSI (11)       //SPI Master Out Slave In (SDI)

#define BMP_CS1  (2)       //Chip Select for BMP1
#define BMP_CS2  (3)       //Chip Select for BMP2
#define BMP_CS3  (4)       //Chip Select for BMP3
#define BMP_CS4  (5)       //Chip Select for BMP4
#define BMP_CS5  (6)       //Chip Select for BMP5
#define BMP_CS6  (7)       //Chip Select for BMP6
#define BMP_CS7  (8)       //Chip Select for BMP7
#define BMP_CS8  (9)       //Chip Select for BMP8
#define BMP_CS9  (10)      //Chip Select for BMP9
#define BMP_CS10 (14)      //Chip Select for BMP10 
#define BMP_CS11 (15)      //Chip Select for BMP11 
#define BMP_CS12 (16)      //Chip Select for BMP12 
#define BMP_CS13 (17)      //Chip Select for BMP13 
#define BMP_CS14 (22)      //Chip Select for BMP14
#define BMP_CS15 (23)      //Chip Select for BMP15
#define BMP_CS16 (24)      //Chip Select for BMP16 

const int ledPin = 52;

/* Variable definitions and BMP280 pressure sensors initialized. */
const int num_pressure_module_slaves = 0;                              //Specify the number of pressure module slaves in the I2C bus.
const char* pressure_slaves_addresses = {};                            //Specify the slave addresses

const int results_capacity = num_pressure_module_slaves*16 + 16;       //Indicates the maximum total number of results of barometric
                                                                       //pressure measurements. If no Pressure Module Slaves are 
                                                                       //connected, the maximum total number of results will be 16.
float results[results_capacity];                                       //Array where the results will be stored in type float.
const int results_for_transmission_capacity = results_capacity*9;      //Indicates the capacity for the bytes to be transmitted to the Master
                                                                       //Module. If no Pressure Module Slaves are connected, the total
                                                                       //capacity for transmission will be 16*9 = 144.
char results_for_transmission[results_for_transmission_capacity];      //Results in type char for transmission to Master Module.
int results_for_transmission_index = 0;                                //Index used to store the char results.
char* sensorsArray = {};                                               //Sensors to be measured among the pressure modules.
int begin_bool = 0;                                                    //For data acquisition purposes

Adafruit_BMP280 bmp1(BMP_CS1, BMP_MOSI, BMP_MISO,  BMP_SCK);           //BMP280 sensor 1
Adafruit_BMP280 bmp2(BMP_CS2, BMP_MOSI, BMP_MISO,  BMP_SCK);           //BMP280 sensor 2
Adafruit_BMP280 bmp3(BMP_CS3, BMP_MOSI, BMP_MISO,  BMP_SCK);           //BMP280 sensor 3
Adafruit_BMP280 bmp4(BMP_CS4, BMP_MOSI, BMP_MISO,  BMP_SCK);           //BMP280 sensor 4
Adafruit_BMP280 bmp5(BMP_CS5, BMP_MOSI, BMP_MISO,  BMP_SCK);           //BMP280 sensor 5
Adafruit_BMP280 bmp6(BMP_CS6, BMP_MOSI, BMP_MISO,  BMP_SCK);           //BMP280 sensor 6
Adafruit_BMP280 bmp7(BMP_CS7, BMP_MOSI, BMP_MISO,  BMP_SCK);           //BMP280 sensor 7
Adafruit_BMP280 bmp8(BMP_CS8, BMP_MOSI, BMP_MISO,  BMP_SCK);           //BMP280 sensor 8
Adafruit_BMP280 bmp9(BMP_CS9, BMP_MOSI, BMP_MISO,  BMP_SCK);           //BMP280 sensor 9
Adafruit_BMP280 bmp10(BMP_CS10, BMP_MOSI, BMP_MISO,  BMP_SCK);         //BMP280 sensor 10
Adafruit_BMP280 bmp11(BMP_CS11, BMP_MOSI, BMP_MISO,  BMP_SCK);         //BMP280 sensor 11
Adafruit_BMP280 bmp12(BMP_CS12, BMP_MOSI, BMP_MISO,  BMP_SCK);         //BMP280 sensor 12
Adafruit_BMP280 bmp13(BMP_CS13, BMP_MOSI, BMP_MISO,  BMP_SCK);         //BMP280 sensor 13
Adafruit_BMP280 bmp14(BMP_CS14, BMP_MOSI, BMP_MISO,  BMP_SCK);         //BMP280 sensor 14
Adafruit_BMP280 bmp15(BMP_CS15, BMP_MOSI, BMP_MISO,  BMP_SCK);         //BMP280 sensor 15
Adafruit_BMP280 bmp16(BMP_CS16, BMP_MOSI, BMP_MISO,  BMP_SCK);         //BMP280 sensor 16

/* 
 The setup function is a default function in Energia IDE that is the first function to execute. 
*/
void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  pinMode(ledPin, OUTPUT);
}

/* setSampling will configure the sensors with the desired settings and data measurement accuracy. */
void setSampling(Adafruit_BMP280 bmp){
    /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,       /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X1,       /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,      /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X2,        /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_125);   /* Standby time. */
}

/* This function will initialize the SPI communication with the pressure sensors, configure each sensor, as well as obtain their data and send
it to the Master Module */
void acquireDataAndTransmitToMaster(){
  unsigned period;
  int sample_index = 0;

  period = (expTime / pressureSamples) * 1000;
  
  unsigned long next = 0;

  digitalWrite(ledPin, HIGH);

  Measurement results[pressureSamples];
  long startTime = millis();
  long currentTime = startTime;

  while (sample_index < pressureSamples) {
    currentTime = millis() - startTime;
    if (currentTime >= next) {
      Serial.println("Taking sample #" + String(sample_index));
      delay(200);
      int j;
      for (j = 1; j <= 16; j++) {
        switch (j) {
          case 1: {
            if (enableSensor[j - 1] == true) {
              bmp1.begin(0x77);
              setSampling(bmp1);
              results[sample_index].pressures[j - 1] =  bmp1.readPressure();
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            else {
              results[sample_index].pressures[j - 1] = 0;
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000; 
            }
            break;
          }
          case 2: {
            if (enableSensor[j - 1] == true) {
              bmp2.begin(0x77);
              setSampling(bmp2);
              results[sample_index].pressures[j - 1] =  bmp2.readPressure();
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            else {
              results[sample_index].pressures[j - 1] =  0;
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            break;
          }
          case 3: {
            if (enableSensor[j - 1] == true) {
              bmp3.begin(0x77);
              setSampling(bmp3);
              results[sample_index].pressures[j - 1] =  bmp3.readPressure();
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            else {
              results[sample_index].pressures[j - 1] =  0;
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            break;
          }
          case 4: {
            if (enableSensor[j - 1] == true) {
              bmp4.begin(0x77);
              setSampling(bmp4);
              results[sample_index].pressures[j - 1] =  bmp4.readPressure();
              Serial.println(results[sample_index].pressures[j - 1]);
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            else {
              results[sample_index].pressures[j - 1] =  0;
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            break;
          }
          case 5: {
            if (enableSensor[j - 1] == true) {
              bmp5.begin(0x77);
              setSampling(bmp5);
              results[sample_index].pressures[j - 1] =  bmp5.readPressure();
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            else {
              results[sample_index].pressures[j - 1] =  0;
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            break;
          }
          case 6: {
            if (enableSensor[j - 1] == true) {
              bmp6.begin(0x77);
              setSampling(bmp6);
              results[sample_index].pressures[j - 1] =  bmp6.readPressure();
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            else {
              results[sample_index].pressures[j - 1] =  0;
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            break;
          }
          case 7: {
            if (enableSensor[j - 1] == true) {
              bmp7.begin(0x77);
              setSampling(bmp7);
              results[sample_index].pressures[j - 1] =  bmp7.readPressure();
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            else {
              results[sample_index].pressures[j - 1] =  0;
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            break;
          }
          case 8: {
            if (enableSensor[j - 1] == true) {
              bmp8.begin(0x77);
              setSampling(bmp8);
              results[sample_index].pressures[j - 1] =  bmp8.readPressure();
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            else {
              results[sample_index].pressures[j - 1] =  0;
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            break;
          }
          case 9: {
            if (enableSensor[j - 1] == true) {
              bmp9.begin(0x77);
              setSampling(bmp9);
              results[sample_index].pressures[j - 1] =  bmp9.readPressure();
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            else {
              results[sample_index].pressures[j - 1] =  0;
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            break;
          }
          case 10: {
            if (enableSensor[j - 1] == true) {
              bmp10.begin(0x77);
              setSampling(bmp10);
              results[sample_index].pressures[j - 1] =  bmp10.readPressure();
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            else {
              results[sample_index].pressures[j - 1] =  0;
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            break;
          }
          case 11: {
            if (enableSensor[j - 1] == true) {
              bmp11.begin(0x77);
              setSampling(bmp11);
              results[sample_index].pressures[j - 1] =  bmp11.readPressure();
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            else {
              results[sample_index].pressures[j - 1] =  0;
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            break;
          }
          case 12: {
            if (enableSensor[j - 1] == true) {
              bmp12.begin(0x77);
              setSampling(bmp12);
              results[sample_index].pressures[j - 1] =  bmp12.readPressure();
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            else {
              results[sample_index].pressures[j - 1] =  0;
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            break;
          }
          case 13: {
            if (enableSensor[j - 1] == true) {
              bmp13.begin(0x77);
              setSampling(bmp13);
              results[sample_index].pressures[j - 1] =  bmp13.readPressure();
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            else {
              results[sample_index].pressures[j - 1] =  0;
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            break;
          }
          case 14: {
            if (enableSensor[j - 1] == true) {
              bmp14.begin(0x77);
              setSampling(bmp14);
              results[sample_index].pressures[j - 1] =  bmp14.readPressure();
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            else {
              results[sample_index].pressures[j - 1] =  0;
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            break;
          }
          case 15: {
            if (enableSensor[j - 1] == true) {
              bmp15.begin(0x77);
              setSampling(bmp15);
              results[sample_index].pressures[j - 1] =  bmp15.readPressure();
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            else {
              results[sample_index].pressures[j - 1] =  0;
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            break;
          }
          case 16: {
            if (enableSensor[j - 1] == true) {
              bmp16.begin(0x77);
              setSampling(bmp16);
              results[sample_index].pressures[j - 1] =  bmp16.readPressure();
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            else {
              results[sample_index].pressures[j - 1] =  0;
              results[sample_index].pressure_ts[j - 1] = currentTime / 1000;
            }
            break;
          }
          default: {
            // TODO
          }
        }
        Serial.print(results[sample_index].pressures[j - 1]);
        Serial.print(" @ ");
        Serial.println(results[sample_index].pressure_ts[j - 1]);
      }
      sample_index++;
      next = currentTime + period;
    }
  }

  // Transmit to master
  int i;
  for (i = 0; i < pressureSamples; i++) {
    delay(1000);
    Serial2.print(String(results[i].pressures[0]) + "," + String(results[i].pressure_ts[0]) + "," + String(results[i].pressures[1]) + "," + String(results[i].pressure_ts[1]));
    Serial2.print("," + String(results[i].pressures[2]) + "," + String(results[i].pressure_ts[2]) + "," + String(results[i].pressures[3]) + "," + String(results[i].pressure_ts[3]));
    Serial2.print("," + String(results[i].pressures[4]) + "," + String(results[i].pressure_ts[4]) + "," + String(results[i].pressures[5]) + "," + String(results[i].pressure_ts[5]));
    Serial2.print("," + String(results[i].pressures[6]) + "," + String(results[i].pressure_ts[6]) + "," + String(results[i].pressures[7]) + "," + String(results[i].pressure_ts[7]));
    Serial2.print("," + String(results[i].pressures[8]) + "," + String(results[i].pressure_ts[8]) + "," + String(results[i].pressures[9]) + "," + String(results[i].pressure_ts[9]));
    Serial2.print("," + String(results[i].pressures[10]) + "," + String(results[i].pressure_ts[10]) + "," + String(results[i].pressures[11]) + "," + String(results[i].pressure_ts[11]));
    Serial2.print("," + String(results[i].pressures[12]) + "," + String(results[i].pressure_ts[12]) + "," + String(results[i].pressures[13]) + "," + String(results[i].pressure_ts[13]));
    Serial2.print("," + String(results[i].pressures[14]) + "," + String(results[i].pressure_ts[14]) + "," + String(results[i].pressures[15]) + "," + String(results[i].pressure_ts[15]));
    Serial2.print('\n');
    
    Serial.print(String(results[i].pressures[0]) + "," + String(results[i].pressure_ts[0]) + "," + String(results[i].pressures[1]) + "," + String(results[i].pressure_ts[1]));
    Serial.print("," + String(results[i].pressures[2]) + "," + String(results[i].pressure_ts[2]) + "," + String(results[i].pressures[3]) + "," + String(results[i].pressure_ts[3]));
    Serial.print("," + String(results[i].pressures[4]) + "," + String(results[i].pressure_ts[4]) + "," + String(results[i].pressures[5]) + "," + String(results[i].pressure_ts[5]));
    Serial.print("," + String(results[i].pressures[6]) + "," + String(results[i].pressure_ts[6]) + "," + String(results[i].pressures[7]) + "," + String(results[i].pressure_ts[7]));
    Serial.print("," + String(results[i].pressures[8]) + "," + String(results[i].pressure_ts[8]) + "," + String(results[i].pressures[9]) + "," + String(results[i].pressure_ts[9]));
    Serial.print("," + String(results[i].pressures[10]) + "," + String(results[i].pressure_ts[10]) + "," + String(results[i].pressures[11]) + "," + String(results[i].pressure_ts[11]));
    Serial.print("," + String(results[i].pressures[12]) + "," + String(results[i].pressure_ts[12]) + "," + String(results[i].pressures[13]) + "," + String(results[i].pressure_ts[13]));
    Serial.println("," + String(results[i].pressures[14]) + "," + String(results[i].pressure_ts[14]) + "," + String(results[i].pressures[15]) + "," + String(results[i].pressure_ts[15]));
  }
  digitalWrite(ledPin, LOW);
}

/* Function loop is a default function for the Energia IDE, where as soon as the function setup execute, loop will execute afterwards
automatically. */
void loop() {
  delay(1000);

  // Listen to master
  Serial.println("Waiting for Master...");
  receiveMessage();
  int command = processCommand();
  switch (command) {
    case 0: {
      // Acquire data mode
      boolean instructionReceived = false;
      Serial.println("Waiting for instructions...");
      while (!instructionReceived) {
        Serial.println("Waiting for instructions...");
        receiveMessage();
        command = processInstructions();
        if (command == 0) {
          instructionReceived = true;
        }
        delay(1000);
      }
      Serial.println("Acquiring data");
      acquireDataAndTransmitToMaster();
      break;
    }
    default: {
      // TODO
    }
  }
}

void receiveMessage() {
  static int ndx = 0;
  char endMarker = '\n';
  char inByte;

  if (Serial2.available() > 0) {
    Serial.println("Received bytes");
    while (Serial2.available() > 0 && newData == false) {
      inByte = Serial2.read();
      if (inByte != endMarker) {
        inBytes[ndx] = inByte;
        ndx++;
        if (ndx >= maxBytes) {
          ndx = maxBytes - 1;
        }
      }
      else {
        inBytes[ndx] = '\0';
        ndx = 0;
        newData = true;
      }
    }
  }
}

int processCommand() {
  if (newData == true) {
    newData = false;
    String command = inBytes;
    // Determine command
    if (command == "acquireData") {
      Serial.println("processCommand(): acquireData");
      return 0;
    }
    else {
      Serial.println("processCommand(): invalid command");
      return -1;
    }
  }
  return -1;
}

int processInstructions() {
  if (newData == true) {
    newData = false;
    // Parse instructions
    return parseInstructions(inBytes);
  }
  return -1;
}

int parseInstructions(char instructions[]) {
  Serial.println("Instruction: " + String(instructions));
  char * strtokIndex;

  Serial.print("Parsing experiment duration: ");
  strtokIndex = strtok(instructions, ",");
  if (strtokIndex == NULL) {
    return -1;
  }
  expTime = atoi(strtokIndex);
  Serial.println(expTime);

  Serial.print("Parsing number of samples: ");
  strtokIndex = strtok(NULL, ",");
  if (strtokIndex == NULL) {
    return -1;
  }
  pressureSamples = atoi(strtokIndex);
  Serial.println(pressureSamples);

  Serial.print("Parsing booleans: ");
  for (int i = 0; i < 8; i++) {
    strtokIndex = strtok(NULL, ",");
    if (strtokIndex == NULL) {
      return -1;
    }

    if (atoi(strtokIndex) == 1) {
      Serial.print("True ");
      enableSensor[i] = true;
    }
    else {
      Serial.print("False ");
      enableSensor[i] = false;
    }
  }
  Serial.println("");
  return 0;
}

void loop_testing() {
  bmp1.begin(0x77);
  setSampling(bmp1);
  bmp2.begin(0x77);
  setSampling(bmp2);
  while (true) {
    Serial.println("bmp1: " + String(bmp1.readPressure()));
    Serial.println("bmp2: " + String(bmp2.readPressure()));
    delay(1000);
  }
}
