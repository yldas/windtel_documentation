#include "DHTesp.h"
#include <ArduinoJson.h>

/* Sensor pins */
const int anemPin = 32;
const int dhtPin = 14;
const int ledPin = 18;

DynamicJsonDocument testDoc(2048);

/* Speed control variables */
volatile int pulseCount = 0;
const int speedBufferSize = 5;
float speedBuffer[speedBufferSize];

/* Data acquisition variables */
int expTime;
int tempSamples;
int humSamples;
int speedSamples;

int maxSamples;

/* Results parameters */
typedef struct {
  float temp;
  unsigned long tempTime;
  float hum;
  unsigned long humTime;
  float speed;
  unsigned long speedTime;
} Measurement;

/* DHT variables */
DHTesp dht;

/* UART variables */
const int maxBytes = 512;
char inBytes[maxBytes];
boolean newData = false;

/* Number of results from Master */
int num_dynamic_results = 0;
int num_pressure_results = 0;
int num_balance_results = 0;

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  
  pinMode(anemPin, INPUT);
  pinMode(dhtPin, INPUT);
  pinMode(ledPin, OUTPUT);

  dht.setup(dhtPin);
  attachInterrupt(anemPin, incrementPulseCount, FALLING);
  noInterrupts();
}

void loop() {
  delay(1000);

  // Listen to master
  Serial.println("Listening to Master...");
  receiveMessage();
  int command = processCommand();
  switch (command) {
    case 0: {
      // Speed control mode
      boolean stopReceived = false;
      while (!stopReceived) {
        speedControl();
        receiveMessage();
        Serial.println("Waiting for stop command...");
        command = processCommand();
        if (command == 1) {
          stopReceived = true;
        }
      }
      break;
    }
    case 2: {
      // Acquire data mode
      boolean instructionReceived = false;
      while (!instructionReceived) {
        receiveMessage();
        Serial.println("Waiting for instructions...");
        command = processInstructions();
        if (command == 0) {
          instructionReceived = true;
        }
        delay(1000);
      }
      Serial.println("expTime: " + String(expTime));
      Serial.println("tempSamples: " + String(tempSamples));
      Serial.println("humSamples: " + String(humSamples));
      Serial.println("speedSamples: " + String(speedSamples));

      Serial.println("Acquiring data");
      acquireDataAndTransmitToMaster();
      break;
    }
    case 3: {
      // TODO
      break;
    }
    default: {
      // TODO
    }
  }
}

void speedControl() {
  interrupts();
  /* Count pulses within 1 second */
  delay(1000);
  noInterrupts();
  float frequency = pulseCount / 2;
  float speed = frequency * 0.098 * 2.23694;
  
  Serial.println("Wind speed: " + String(speed) + " mph");
  Serial2.println(speed);
  pulseCount = 0;
}

float getSpeed() {
  interrupts();
  /* Count pulses within 1 second */
  delay(1000);
  noInterrupts();
  float frequency = pulseCount / 4;
  float speed = frequency * 0.098 * 2.23694;
  pulseCount = 0;
  return speed;
}

void incrementPulseCount() {
  pulseCount++;
}

void acquireDataAndTransmitToMaster() {  
  unsigned long tempPeriod;
  unsigned long humPeriod;
  unsigned long speedPeriod;
  
  int tempSamples_i = 0;
  int humSamples_i = 0;
  int speedSamples_i = 0;
  
  maxSamples = getMaxSamples();
  num_dynamic_results = maxSamples;

  if (tempSamples == 0) {
    tempPeriod = 0;
  }
  else {
    tempPeriod = (expTime / tempSamples) * 1000;
  }
  if (humSamples == 0) {
    humPeriod = 0;
  }
  else {
    humPeriod = (expTime / humSamples) * 1000;
  }
  if (speedSamples == 0) {
    speedPeriod = 0;
  }
  else {
    speedPeriod = (expTime / speedSamples) * 1000;
  }

  unsigned long tempNext = 0;
  unsigned long humNext = 0;
  unsigned long speedNext = 0;

  Serial.println("Executing instructions");
  digitalWrite(ledPin, HIGH);
  Measurement results[maxSamples];
  long startTime = millis();
  long currentTime = startTime;
  while (tempSamples_i < tempSamples || humSamples_i < humSamples || speedSamples_i < speedSamples) {
    currentTime = millis() - startTime;
    if (tempSamples_i < tempSamples && currentTime >= tempNext) {
      Serial.print("Taking temperature: ");
      delay(200);
      results[tempSamples_i].temp = dht.getTemperature();
      //results[tempSamples_i].temp = 25.65;
      results[tempSamples_i].tempTime = currentTime / 1000;
      Serial.print(results[tempSamples_i].temp);
      Serial.print(" @ ");
      Serial.print(results[tempSamples_i].tempTime);
      Serial.println("s");
      tempSamples_i++;
      tempNext = currentTime + tempPeriod;
    }

    currentTime = millis() - startTime;
    if (humSamples_i < humSamples && currentTime >= humNext) {
      Serial.print("Taking humidity: ");
      delay(200);
      results[humSamples_i].hum = dht.getHumidity();
      //results[humSamples_i].hum = 85.32;
      results[humSamples_i].humTime = currentTime / 1000;
      Serial.print(results[humSamples_i].hum);
      Serial.print(" @ ");
      Serial.print(results[humSamples_i].humTime);
      Serial.println("s");
      humSamples_i++;
      humNext = currentTime + humPeriod;
    }
    
    currentTime = millis() - startTime;
    if (speedSamples_i < speedSamples && currentTime >= speedNext) {
      Serial.print("Taking speed: ");
      results[speedSamples_i].speed = getSpeed();
      results[speedSamples_i].speedTime = currentTime / 1000;
      Serial.print(results[speedSamples_i].speed);
      Serial.print(" @ ");
      Serial.print(results[speedSamples_i].speedTime);
      Serial.println("s");
      speedSamples_i++;
      speedNext = currentTime + speedPeriod;
    }
  }
  
  // Transmit to master
  int i;
  for (i = 0; i < maxSamples; i++) {
    Serial.println("Transmitting results...");
    delay(500);
    int index = 0;
    if (results[i].speed > 100) {
      results[i].speed = 99;
    }
    Serial2.println(String(results[i].temp) + "," + String(results[i].tempTime) + "," + String(results[i].hum) + "," + String(results[i].humTime) + "," + String(results[i].speed) + "," + String(results[i].speedTime));
    Serial.println(String(results[i].temp) + "," + String(results[i].tempTime) + "," + String(results[i].hum) + "," + String(results[i].humTime) + "," + String(results[i].speed) + "," + String(results[i].speedTime));
  }
  digitalWrite(ledPin, LOW);
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
    if (command == "speedControl") {
      Serial.println("processCommand(): speedControl");
      return 0;
    }
    else if (command == "stop") {
      Serial.println("processCommand(): stop");
      return 1;
    }
    else if (command == "acquireData") {
      Serial.println("processCommand(): acquireData");
      return 2;
    }
    else if (command == "finish") {
      Serial.println("processCommand(): finish");
      return 3;
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

int processResults() {
  if (newData == true) {
    newData = false;
    // TODO
  }
}

int parseInstructions(char instructions[]) {
  Serial.println("Instruction: " + String(instructions));
  char * strtokIndex;
  
  strtokIndex = strtok(instructions, ",");
  if (strtokIndex == NULL) {
    return -1;
  }
  expTime = atoi(strtokIndex);

  strtokIndex = strtok(NULL, ",");
  if (strtokIndex == NULL) {
    return -1;
  }
  tempSamples = atoi(strtokIndex);

  strtokIndex = strtok(NULL, ",");
  if (strtokIndex == NULL) {
    return -1;
  }
  humSamples = atoi(strtokIndex);

  strtokIndex = strtok(NULL, ",");
  if (strtokIndex == NULL) {
    return -1;
  }
  speedSamples = atoi(strtokIndex);

  return 0;
}

void testing_sensors() {
  Serial.println("Temperature: " + String(dht.getTemperature()));
  Serial.println("Humidity: " + String(dht.getHumidity()));
  Serial.println("Speed: " + String(getSpeed()));
  delay(3000);
}

int getMaxSamples() {
  maxSamples = tempSamples;
  if (tempSamples > maxSamples) {
    maxSamples = tempSamples;
  }
  if (speedSamples > maxSamples) {
    maxSamples = speedSamples;
  }
  return maxSamples;
}

void parseCommandsAndSendToMaster(String message) {
  DynamicJsonDocument doc(2048);

  DeserializationError error = deserializeJson(doc, message);
  deserializeJson(testDoc, message);
  if (error) {
    Serial.print("deserialize() failed: ");
    Serial.println(error.c_str());
    return;
  }
  
  int rodLength = doc["rodLength"];
  int expDuration = doc["expDuration"];
  
  int pressureSamples = doc["pressureSamples"];
  boolean pressureSensors[] = {
    doc["pressureSensors"][0], doc["pressureSensors"][1], doc["pressureSensors"][2], doc["pressureSensors"][3],
    doc["pressureSensors"][4], doc["pressureSensors"][5], doc["pressureSensors"][6], doc["pressureSensors"][7],
    doc["pressureSensors"][8], doc["pressureSensors"][9], doc["pressureSensors"][10], doc["pressureSensors"][11],
    doc["pressureSensors"][12], doc["pressureSensors"][13], doc["pressureSensors"][14], doc["pressureSensors"][15]
    };
    
  int forcesSamples = doc["forcesSamples"];
  boolean forcesSensors[] = {
    doc["forcesSensors"][0], doc["forcesSensors"][1], doc["forcesSensors"][2], doc["forcesSensors"][3], doc["forcesSensors"][4]
    };
    
  int humidity_samples = doc["humSamples"];
  int temperature_samples = doc["tempSamples"];
  int windspeed_samples = doc["windSpeedSamples"];

  // Transmit commands to Master
  Serial.println("Sending app commands to Master");
  
  Serial.println("Transmitting Pressure commands");

  Serial2.print(String(expDuration) + "," + String(pressureSamples) + ",");
  for (int i = 0; i < 15; i++) {
    Serial2.print(String(pressureSensors[i]) + ",");
  }
  Serial2.println(String(pressureSensors[15]));

  Serial.println("Transmitting Dynamic commands");
  Serial2.println(String(expDuration) + "," + String(temperature_samples) + "," + String(humidity_samples) + "," + String(windspeed_samples));

  Serial.println("Transmitting Balance commands");
  Serial2.println(String(expDuration) + String(forcesSamples) + "," + String(rodLength) + "," + "1,1,1,1,1");
}
