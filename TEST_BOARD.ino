///////////////////////////////////////////////////////////////////////////
//                     CODE XE DÒ LINE 2 BÁNH PID                        //
//          HÃY THAM GIA GROUP XE ĐUA TỰ HÀNH MCR TRÊN FACEBOOK          //
//                 HÃY CHIA SẼ VÀ GHI RÕ NGUỒN KHI THAM KHẢO             //
//                              CẢM ƠN BẠN                               //
//            https://www.facebook.com/groups/xedualaptrinhMCR           //
///////////////////////////////////////////////////////////////////////////

#include "components.h"

SumoState currentState = STATE_IDLE;


void setup() {
  pinMode(LDIR, OUTPUT);
  pinMode(RDIR, OUTPUT);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(RDIR, LOW);
  digitalWrite(LDIR, LOW);
  RGB(0,0,0);
  //-------------------------------------------------//
  //CÀI ĐẶT CÁC THÔNG SỐ BAN ĐẦU
  state = 0;// set trạng thái chạy
  // readEeprom();
  timer_init();
  speed_run(0, 0);// dừng 2 bánh
  Serial.begin(9600);
  initI2Csensors();
//   BUZZ(1);
}

void loop()// vòng lặp, không dùng delay() trong này
{
  //  readTOF(TOF_distance);
// readTOF(TOF_distance);
//     Serial.print(TOF_distance[0]);
//     Serial.print("\t");
//     Serial.print(TOF_distance[1]);
//     Serial.print("\t");
//     Serial.println(TOF_distance[2]);
// delay(10);
   // readIR(IR_values);
//     Serial.print(IR_values[0]);
//     Serial.print("\t");
//     Serial.print(IR_values[1]);
//     Serial.print("\t");
//     Serial.println(IR_values[2]);
// delay(100);
  readIMU(IMU_values, IMU_AccValues);
    Serial.print(IMU_values[0]);
    Serial.print("\t");
    Serial.print(IMU_values[1]);
    Serial.print("\t");
    Serial.print(IMU_values[2]);
    Serial.print("\t");
    Serial.print(IMU_AccValues[0]);
    Serial.print("\t");
    Serial.print(IMU_AccValues[1]);
    Serial.print("\t");
    Serial.println(IMU_AccValues[2]);
 delay(10);
/*
    switch (currentState) {
        case STATE_IDLE:
            // if () // kiểm tra hướng xoay của robot 
            if (!digitalRead(BUTTON1))
                changeState(STATE_CALIB);
            else if (!digitalRead(BUTTON2))
            {
                stateTimer = millis();
                while (millis() - stateTimer <= 5000)
                    {
                        RGB_loading();
                        logSerial("Waiting...");
                        delay(1000);
                        speed_run(0, 0); 
                    }
                changeState(STATE_ATTACK);
            }
            break;

        case STATE_CALIB:
            calibIR();
            simpleDrive(0, 0);
            break;


        case STATE_SEARCH:
            if(isEnemyDetected(TOF_distance)) changeState(STATE_ATTACK);
            if (isEdgeDetected(IR_values)) changeState(STATE_EDGE_AVOID);
            if(millis() - stateTimer > 5000) simpleDrive(0, 0);
            
            logSerial("Searching...");
            simpleDrive(0, 180);

            break;

        case STATE_ATTACK:
            if(!isEnemyDetected(TOF_distance)) changeState(STATE_SEARCH);
            // if(isEdgeDetected(IR_values)) changeState(STATE_EDGE_AVOID);
            
            logSerial("Attacking...");
            vectorDrive(255, turnFactor(TOF_distance));
            break;

        case STATE_EDGE_AVOID:
        /*
            if (avoidPhase == 0) {
                simpleDrive(175, 180);
                if (millis() - stateTimer > TIME_ESCAPE_REV) { 
                    avoidPhase = 1;
                    stateTimer = millis(); 
                }
            } else if (avoidPhase == 1) {
                simpleDrive(255, 0);
                if (millis() - stateTimer > TIME_ESCAPE_SPIN) { 
                    changeState(STATE_SEARCH);
                }
            }
            break; *
            if (!isEdgeDetected(IR_values)) changeState(STATE_ATTACK);

            Serial.print(IR_values[0]);
            Serial.print("\t");
            Serial.print(IR_values[1]);
            Serial.print("\t");
            Serial.println(IR_values[2]);

            logSerial("Avoiding edge...");
            if (IR_values[0] < 100 && IR_values[1] < 100)
                simpleDrive(-255, 0);
            else if (IR_values[0] < 100)
                speed_run(-255, 0);
            else if (IR_values[1] < 100) 
                speed_run(0, -255);
            else if (IR_values[2] < 100)
                simpleDrive(255, 0);
            // xử lý trường hợp 0 && 2 hoặc 1 && 2 detect (hổng 1 bánh)
            break;
    }
    */
}
void changeState(SumoState newState) {
    currentState = newState;
    stateTimer = millis();
}