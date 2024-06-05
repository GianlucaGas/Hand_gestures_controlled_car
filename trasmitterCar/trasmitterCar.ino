#include <RF24.h>
#include <RF24_config.h>
#include <nRF24L01.h>
#include <printf.h>

#include <MPU6050_tockn.h>
#include <Wire.h> // non sembra servire

#define ACCESS_DATA_TIMING 100


MPU6050 mpu6050(Wire);
RF24 radio(2, 9);  //definisco oggetto (ce, cns)
const byte address[6] = "00001";

unsigned long t1, dt;


void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

void loop() {
  float coordinates[2];

  mpu6050.update();

  dt = millis() - t1;
  if(dt > ACCESS_DATA_TIMING){
    t1 = millis();
    coordinates[0] = mpu6050.getAccAngleX(); // x
    coordinates[1] = mpu6050.getAccAngleY(); // y
    Serial.print("X: ");
    Serial.println(coordinates[0]);
    Serial.print("Y: ");
    Serial.println(coordinates[1]);
    Serial.println("----------------------");
    
    

    radio.write(&coordinates, sizeof(coordinates));
  }
}
