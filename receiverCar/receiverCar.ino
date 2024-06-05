#include <RF24.h>
#include <RF24_config.h>
#include <nRF24L01.h>
#include <printf.h>

// system data
#define MOTOR_SPEED 100
#define ACCESS_DATA_TIMING 100
#define PINS_NUMBER 6

// 
RF24 radio(2, 9);
const byte address[6] = "00001";

// creo un delay per contrastare quello generato dal module nRF24L01
unsigned long t1, dt;

int motor1[6] = {3, 4, 5, 6, 7, 8}; // (motor 1) enA, in1, in2, in3, in4, enB
int motor2[6] = {14, 15, 16, 17, 18, 19}; // (motor 2) enA, in1, in2, in3, in4, enB

struct MOTOR_STATUS{
  int forward[8] = {HIGH, LOW, LOW, HIGH, LOW, HIGH, HIGH, LOW}; // 1, 2,3 ,4 (2 a 2)
  int behind[8] = {LOW, HIGH, HIGH, LOW, HIGH, LOW, LOW, HIGH};
  int right[8] = {HIGH, LOW, HIGH, LOW, LOW, HIGH, LOW, HIGH};
  int left[8] = {LOW, HIGH, LOW, HIGH, HIGH, LOW, HIGH, LOW};
  int forward_right[8] = {LOW, LOW, HIGH, LOW, HIGH, LOW, LOW, LOW};
  int forward_left[8] = {HIGH, LOW, LOW, LOW, LOW, LOW, LOW, HIGH};
  int behind_right[8] = {LOW, HIGH, LOW, LOW, LOW, LOW, LOW, HIGH};
  int behind_left[8] = {LOW, LOW, HIGH, LOW, HIGH, LOW, LOW, LOW};

  int stop[8] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
};


// scelgo quali motori azionare
void get_coordinateData(){
  float margin_pos = 15.0;
  float margin_neg = -15.0;
  int coordinates[2]; // x, y
  MOTOR_STATUS status_motor;
  
  radio.read(&coordinates, sizeof(coordinates));

  Serial.print("X: ");
  Serial.println(coordinates[0]);
  Serial.print("Y: ");
  Serial.println(coordinates[1]);
  Serial.println("-------------------");

  // FORWARD
  if(coordinates[0] < margin_neg){
    movements(status_motor.forward);
  }
  // BEHIND
  else if(coordinates[0] > margin_pos){
    movements(status_motor.behind);
  }
  // RIGHT
  else if(coordinates[1] > margin_pos){
    movements(status_motor.right);
  }
  // LEFT
  else if(coordinates[1] < margin_neg){
    movements(status_motor.left);
  }
  // FORWARD_RIGHT
  else if(coordinates[0] < margin_neg && coordinates[1] > margin_pos){
    movements(status_motor.forward_right);
  }
  // FORWARD_LEFT
  else if(coordinates[0] < margin_neg && coordinates[1] < margin_neg){
    movements(status_motor.forward_left);
  }
  // BEHIND_RIGHT
  else if(coordinates[0] > margin_pos && coordinates[1] > margin_pos){
    movements(status_motor.behind_right);
  }
  // BEHIND_LEFT
  else if(coordinates[0] > margin_pos && coordinates[1] < margin_neg){
    movements(status_motor.behind_left);
  }
  // STOP
  else{
    movements(status_motor.stop);
  }
}

void movements(int status_motors[]){
  digitalWrite(motor1[0], status_motors[0]);
  digitalWrite(motor1[1], status_motors[1]);
  digitalWrite(motor1[2], status_motors[2]);
  digitalWrite(motor1[3], status_motors[3]);
  digitalWrite(motor1[4], MOTOR_SPEED);
  digitalWrite(motor1[5], MOTOR_SPEED);

  digitalWrite(motor2[0], status_motors[4]);
  digitalWrite(motor2[1], status_motors[5]);
  digitalWrite(motor2[2], status_motors[6]);
  digitalWrite(motor2[3], status_motors[7]);
  digitalWrite(motor2[4], MOTOR_SPEED);
  digitalWrite(motor2[5], MOTOR_SPEED);
}

void setPinMode(){
  for(int i = 0; i < PINS_NUMBER; i++){
    pinMode(motor1[i], OUTPUT);
    pinMode(motor2[i], OUTPUT);
  }
}


void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  setPinMode();
}

void loop() {
  if(radio.available()){

    dt = millis() - t1;
    if(dt > ACCESS_DATA_TIMING){
      t1 = millis();
      get_coordinateData();
    }
  }

}





/*

AGGIUNTA:
usare solo la libreria <Wire.h>
usare funzione map() per dimenionare l'accelerazione a seconda del piegamento della mano

*/




