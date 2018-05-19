#include <Ultrasonic.h>// by Erick Simões
#include <SoftwareSerial.h>
#include <Servo.h>

#define SERVO 9
#define TONE 5
#define ECHO 13
#define TRIGGER 12
#define TIMEOUT 2000
#define RX 10
#define TX 11

Servo servo;
Ultrasonic ultrasonic(TRIGGER, ECHO);//, TIMEOUT);
SoftwareSerial bluetooth(RX, TX); //RX, TX
int incomingByte;
int pos = 0;// Posição 0 o portão está aberto, Posição 125 o portão está aberto
float seno;
int alarmEnabled = 0;


void setup(){
  // put your setup code here, to run once:
  //pos = 45;
  Serial.begin(9600);
  bluetooth.begin(9600);
  bluetooth.println("Hello, world?");
  
  servo.attach(SERVO);
  servo.write(pos);
  //pinMode(11, OUTPUT);
  //digitalWrite(11, LOW);//GND mode
}

void loop(){  
  // put your main code here, to run repeatedly:
  //openGate();
  //closeGate();
  calcDistance();
  if(bluetooth.available()){
    bluetooth.println("received");
    incomingByte = bluetooth.read();
    Serial.println(incomingByte);
  }
  //ativa o alarme
  if (incomingByte == 'a'){
    alarmEnabled = 1;
    bluetooth.println("alarme");
    if(calcDistance() < 9.0){
      closeGate(true);
      alarm();
    } else{
      openGate(true);
    }
  } else if (incomingByte == 'o'){
    //abre a porta
    openGate(true);
  } else  if (incomingByte == 'c'){
    //fecha a porta
    closeGate();
  } else{
    alarmEnabled = 0;
    noTone(TONE);
  }
}

void openGate(){
  for(pos; pos >= 0; pos -= 1){
      servo.write(pos);
      delay(5);
    }
    pos = 0;
}

void openGate(bool turnOffAlarm){
  if (turnOffAlarm){
    noTone(TONE);
  }
  openGate();
}

void closeGate(){
  for(pos; pos <= 125; pos += 1){
    servo.write(pos);
    delay(25);
  }
  pos = 140;
}

void closeGate(bool playAlarm){  
  if (playAlarm){
        alarm(); 
      }
  for(pos; pos <= 125; pos += 1){      
      servo.write(pos);
      //delay(2);
    }
    pos = 140;
}

void alarm(){
  for(int x = 0; x <= 360; x += 1){
    seno = 50 * (sin(x*PI/180 + 5) + 10); //aumentar a amplitude para um alarme mais alto
    int freq = int(seno);
    tone(TONE, freq);
    delay(1);
  }
}

float calcDistance(){
  float distanceCM = ultrasonic.distanceRead(CM);
  Serial.print(distanceCM);
  Serial.println(" cm");
  return distanceCM;
}

