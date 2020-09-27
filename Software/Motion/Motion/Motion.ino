
#ifndef ARDUINO
#ifdef ESP8266
 #include <ESP8266WiFi.h>
#else //ESP32
 #include <WiFi.h>
#endif
#include <ModbusIP_ESP8266.h>
#include "SoftwareSerial.h"
#endif
#include <Servo.h>
#include <Stepper.h>


/*Nema 17 step per revolution*/
#define _STEPS 200

#define MINB 130
#define MAXB 170
#define MINM 75
#define MAXM 155

/*ESP8266 PINOUT*/
/*ULN2003A PINS*/
#ifdef ESP8266
/*Stepper - Rotation Ax*/
#define _PHASE_A 5
#define _PHASE_B 0
#define _PHASE_C 15
#define _PHASE_D 13

/*Solenoid*/
#define _SOLENOID_CMD 14
/*Servos pins*/
#define _SERVO_BATTENTE_PIN  4
#define _SERVO_PRECARICO_PIN 12
#endif
/*ARDUINO PINOUT*/
#ifdef ARDUINO
/*L293B*/
#define _PHASE_A 10
#define _PHASE_B 11
#define _PHASE_C 13
#define _PHASE_D 12
/*Solenoid*/
#define _SOLENOID_CMD 14
/*Servos pins*/
#define _SERVO_BATTENTE_PIN  5
#define _SERVO_PRECARICO_PIN 6
#endif

/*Serial commands*/
/*MODBUSTCP*/
/*I Registers*/
#define _INPUT_REGISTERS 4
#define _H_REG_OFFSET    4
#define _I_REG_POS_BATTENTE   0x1
#define _I_REG_POS_PRECARICO  0x2
#define _I_REG_POS_ROTATION   0x3
#define _I_REG_POS_AVAILABLE  0x4
/*Coils*/
#define _COILS_NUMBERS 10
/************************************/
#define _I_COIL_CHARGE              1
#define _I_COIL_SHOOT               2
#define _I_COIL_WAITING             3 
/***********DEBUG COILS*************/
#define _I_COIL_MOVECLOCKWISE       4
#define _I_COIL_MOVEANTICLOCKWISE   5
#define _I_COIL_DEBUG               6
/***********************************/
/***********************************/
#ifdef ESP8266
ModbusIP mb;
#endif
/************************************/
/************************************/

/*WIFI CONNECTION*/
#define _SSID "iPhone di pietr"
#define _PWD  "pietro1234"
/*Servos*/
Servo servobattente;
Servo servomolla; 
/*Stepper*/
Stepper stepper(_STEPS,_PHASE_A,_PHASE_C,_PHASE_B,_PHASE_D); 

int posB = MINB;    // variable to store the servo Battente position
int posM = MINM;   // variable to store the servo Molla position
char cmdBuffer;

#define _POSITIVE_LIMIT  100
#define _NEGATIVE_LIMIT -100
int stepperPos = 0;

void setup() 
{
  /*Serial*/
  Serial.begin(115200);
  #ifdef ESP8266
  /*WI-FI*/
  WiFi.begin(_SSID, _PWD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  /*Modbus server*/
  mb.onConnect(cbConn); /*Call back for connection event*/
  mb.server();
  /*Coils*/
  for(int index = 0; index < _COILS_NUMBERS; index++)
  {
    mb.addCoil(index);
  }
  /*Holding registers*/
  mb.addHreg(_H_REG_OFFSET, _I_REG_POS_BATTENTE);
  mb.addHreg(_H_REG_OFFSET, _I_REG_POS_PRECARICO);
  mb.addHreg(_H_REG_OFFSET, _I_REG_POS_BATTENTE);
  mb.addHreg(_H_REG_OFFSET, _I_REG_POS_ROTATION);
  mb.addHreg(_H_REG_OFFSET, _I_REG_POS_AVAILABLE);
  #endif
  /*************************************/
  /*************************************/
  pinMode(_SOLENOID_CMD, OUTPUT);
  servobattente.attach(_SERVO_BATTENTE_PIN);
  servomolla.attach(_SERVO_PRECARICO_PIN);
  /*************************************/
  /*Stepper speed*/
  stepper.setSpeed(20);
  /*************************************/
}

void loop() 
{

  #ifdef ESP8266
  /*Modbus task*/
  mb.task();

  if(mb.Coil(_I_COIL_CHARGE))
  {
      mb.Coil(_I_COIL_CHARGE,0);
      Serial.println("Charge");
      carica();
      Serial.println("Done");
  }

  if(mb.Coil(_I_COIL_SHOOT))
  {
      mb.Coil(_I_COIL_SHOOT,0);
      Serial.println("Fire");
      fuoco();
      Serial.println("Done");
  }

  if(mb.Coil(_I_COIL_WAITING))
  {
      mb.Coil(_I_COIL_WAITING,0);
      Serial.println("Waiting position");
      riposa();
      Serial.println("Done");
  }

  if(mb.Coil(_I_COIL_MOVECLOCKWISE))
  {
      mb.Coil(_I_COIL_MOVECLOCKWISE,0);
      Serial.println("Debug function");
      stepper.moveTo (true, 2048);
      Serial.println("Done");
  }

  if(mb.Coil(_I_COIL_MOVEANTICLOCKWISE))
  {
      mb.Coil(_I_COIL_MOVEANTICLOCKWISE,0);
      Serial.println("Debug function");
      stepper.moveTo (true, 0);
      Serial.println("Done");
  }

  if(mb.Coil(_I_COIL_DEBUG))
  {
      mb.Coil(_I_COIL_DEBUG,0);
      Serial.println("Debug function");
      test();
      Serial.println("Done");
  }
  #else

   if(Serial.available())
  {
    cmdBuffer = Serial.read();

    switch(cmdBuffer)
    {
      case '1':
        Serial.println("Charge");
        carica();
        delay(1000);
        Serial.println("Done");
        break;

      case '2':
        Serial.println("Fire");
        fuoco();
        delay(1000);
        Serial.println("Done");
        break;

      case '3':
        Serial.println("Waiting position");
        riposa();
        delay(1000);
        Serial.println("Done");
        break;

      case '4':
        Serial.println("MOVE 50 STEP LEFT");
        if(stepperPos > _NEGATIVE_LIMIT)
        {
          stepperPos = stepperPos - 50;
          stepper.step(-50);
        }
        Serial.println("Done");
        break;

      case '5':
        Serial.println("MOVE 50 STEP RIGHT");
        if(stepperPos < _POSITIVE_LIMIT)
        {
          stepperPos = stepperPos + 50;
          stepper.step(50);
        }
        Serial.println("Done");
        break;
      
    }
      
  }

  #endif
  
  delay(10);
}

/*Catapult functions*/
void riposa() {//3
  for (posM ; posM <= MAXM; posM += 1) { 
    servomolla.write(posM);              
    delay(15);
  }
//  delay(200);

  for (posB; posB <= MAXB; posB += 1) { 
    servobattente.write(posB);              
    delay(15);
  }
//  delay(200);
  //digitalWrite(3, LOW); 
}
void carica() { //1
  for (posM; posM >= MINM; posM -= 1) { 
    servomolla.write(posM);              
    delay(15);                       
  }
  for (posB; posB >= MINB; posB -= 1) { 
    servobattente.write(posB);              
    delay(15);
  }
 // delay(200);
}
void fuoco() { //2
      digitalWrite(_SOLENOID_CMD, HIGH); 
//      delay(1000);
      digitalWrite(_SOLENOID_CMD, LOW);
}

#ifdef ESP8266
/*MODBUS Callbacks*/
bool cbConn(IPAddress ip) {
  Serial.println("Incoming MODBUS connession from : ");
  Serial.println(ip);
  return true;
}
#endif

void test()
{  
  for(int i = 0; i < 1000; i++)
  {
    digitalWrite(_PHASE_A, HIGH);
    digitalWrite(_PHASE_B, LOW);
    digitalWrite(_PHASE_C, LOW);
    digitalWrite(_PHASE_D, LOW);
    delay(4);
    digitalWrite(_PHASE_A, LOW);
    digitalWrite(_PHASE_B, HIGH);
    digitalWrite(_PHASE_C, LOW);
    digitalWrite(_PHASE_D, LOW);
    delay(4);
    digitalWrite(_PHASE_A, LOW);
    digitalWrite(_PHASE_B, LOW);
    digitalWrite(_PHASE_C, HIGH);
    digitalWrite(_PHASE_D, LOW);
    delay(4);
    digitalWrite(_PHASE_A, LOW);
    digitalWrite(_PHASE_B, LOW);
    digitalWrite(_PHASE_C, LOW);
    digitalWrite(_PHASE_D, HIGH);
    delay(4);
  }
 }
 
