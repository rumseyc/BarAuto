#include "LedControl.h"
#include "StepMotorCntl.h"
#include <AccelStepper.h>

#define LEFT_PROX_PIN   12
#define RIGHT_PROX_PIN  13
#define HOME_PROX_PIN   11


AccelStepper stepper(1, 8, 9);

//                   0       1      2      3   4    5     6     7     8     
uint32_t bottlePos[] = {-12000, -9000, -6000, -3000, 0, 3000, 6000, 9000};

boolean machineBusy = false;

void setup()
{
  Serial.begin(115200);
  
  stepper.setMaxSpeed(3000);
  stepper.setSpeed(1500);
  stepper.setAcceleration(1500); 
  
  delay(2000); // power-up safety delay

  pinMode(MOTOR_DIR_PIN, OUTPUT);
  pinMode(MOTOR_CNTL_PIN, OUTPUT); 
  pinMode(MOTOR_ENB_PIN, OUTPUT);
  digitalWrite(MOTOR_ENB_PIN, HIGH);
  pinMode(LED_STRIP_CTRL_PIN, OUTPUT);
  pinMode(CUP_LED_CTRL_PIN, OUTPUT);

  pinMode(HOME_PROX_PIN, INPUT);
  pinMode(RIGHT_PROX_PIN, INPUT);
  pinMode(LEFT_PROX_PIN, INPUT);

  init_LedComponents();

  searchForHomePos();
  stepper.setMaxSpeed(3000);
  stepper.setSpeed(1500);
  stepper.setAcceleration(1500); 
  
}
 
void loop() 
{
  if (Serial.available() > 0) {
    String message="";
    while(Serial.available()){ 
      char partial = Serial.read();
      message += partial;
      delay(2);
    }
    parseString(message);
  }

//  if(!machineBusy)
//    idleLights();
}

/*
  COMMUNICATION METHODS
*/
void parseString(String message){
//  tell("Got a message");
  Serial.println(message);
  if(message.startsWith("!") && message.endsWith("\n")){
     if(message == "!GoHome\n"){
       Serial.println("Going Home");
//       goHome();
//       success();
     }else if(message.startsWith("!NewDrink")){
         Serial.println("NewDrink");
         machineBusy = true;
         int firstIndex = message.indexOf('|');
         int secondIndex = message.indexOf('|', firstIndex+1);
         int thirdIndex = message.indexOf('|',secondIndex+1);
         int starts =(message.substring(firstIndex+1,secondIndex)).toInt();
         int time=(message.substring(secondIndex+1,thirdIndex)).toInt();
         bool newLights= (message.substring(thirdIndex+1)).toInt() == 1;  
         Serial.println( starts);
         Serial.println( time);
         Serial.println( newLights);

//         if(newLights==false && hasLights == true){
//           ledSet(BLACK);
//           shutDownNeo();
//         }
//         hasLights=newLights;
      
//         goHome();
//         if(waitForActivation(starts,time)){
//            success(); 
//         }else{
//           timeout();
//         }
     } else {
         //tell("Directives");
         int index = message.indexOf('|');
         int botIdx = (message.substring(1,index)).toInt();
         int mliters = (message.substring(index+1)).toInt();
         Serial.println( botIdx);
         Serial.println( mliters);
         servoGoToPos(botIdx);
//         spill(times);
//         success();
//         if(pos==0 && times==0 && hasLights){
//           machineBusy = false;
//           rainbowParty(15);      
//         }
     }
  }  
}


//Add find home position with stepper motor
void searchForHomePos()
{
  //First step left
  stepper.moveTo(-100000);
  while ((digitalRead(LEFT_PROX_PIN) != HIGH) || (digitalRead(HOME_PROX_PIN) != HIGH)) 
    stepper.run();
  stepper.stop();
  
  if(digitalRead(HOME_PROX_PIN) != HIGH)
  {
    stepper.moveTo(100000); //Move right
    while ((digitalRead(RIGHT_PROX_PIN) != HIGH) || (digitalRead(HOME_PROX_PIN) != HIGH)) 
      stepper.run();
    stepper.stop();
  }

  if(digitalRead(HOME_PROX_PIN) == HIGH)
  {
    stepper.setCurrentPosition(0); //Set 0 position
  }
  else
    exit(-1);
    
}

void servoGoToPos(uint32_t index)
{
  stepper.moveTo(bottlePos[index]);
  while (stepper.currentPosition() != index) // Full speed up to 300
    stepper.run();
//  stepper.runToNewPosition(bottlePos[index]); // Cause an overshoot then back to 0

}

//void spill(int times){
//  myservo.attach(servo); 
//  for ( int i = 0; i<times ; i++){
//    servoup();
//    delay(timeToSpill);
//    servodown();
//    if(i!=times-1) delay (timeToRefill);
//  }  
//  delay(200);
//  myservo.detach(); 
//}
