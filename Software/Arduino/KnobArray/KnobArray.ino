#include "Serial_parser.h"

Serial_parser serial_port;
int values[6];
unsigned long last_time = 0;
unsigned long data_rate = 50;

bool printData_ON = false;

void setup(){
    Serial.begin(250000);
    serial_port.serialPort = &Serial;
    serial_port.serialPort_debug = &Serial;
}

void loop(){
    int inComand = serial_port.parseInputData();
    if (inComand != 0){
        if (inComand == 1){
            printData_ON = (serial_port.dataStream == 1)? true : false;
        }
    }

    values[0] = analogRead(A0);
    values[1] = analogRead(A1);
    values[2] = analogRead(A2);
    values[3] = analogRead(A3);
    values[4] = analogRead(A4);
    values[5] = analogRead(A5);
    
    if (millis() - last_time >= data_rate && printData_ON){
        last_time = millis();
        printData();
    }
}

void printData(){
    Serial.print('#');
    for (int i = 0; i < 6; i++){
        Serial.print(values[i]);
        if (i != 5){
            Serial.print(",");
        }
    }
    Serial.println(';');

}