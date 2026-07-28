#include "Arduino.h"
// #include <EEPROM.h>
#include "components.h"

void logSerial(String s)
{
  Serial.println(s);
}

//------------------------------Timer----------------------------------//
void timer_init() {
  // Timer/Counter 2 initialization
  ASSR = (0 << EXCLK) | (0 << AS2);
  TCCR2A = (0 << COM2A1) | (0 << COM2A0) | (0 << COM2B1) | (0 << COM2B0) | (0 << WGM21) | (0 << WGM20);
  TCCR2B = (0 << WGM22) | (1 << CS22) | (1 << CS21) | (1 << CS20);
  TCNT2 = 0xB2;
  OCR2A = 0x00;
  OCR2B = 0x00;
  // Timer/Counter 2 Interrupt(s) initialization
  TIMSK2 = (0 << OCIE2B) | (0 << OCIE2A) | (1 << TOIE2);
}
ISR(TIMER2_OVF_vect) {
  TCNT2 = 0xB2;
  // read_sensor();  // PID & sensor read
  cnt++;
}
//------------------------------indicator LED---------------------------//
void RGB_loading(){
  switch (millis() / 100 % 3) {
    case 0:
      RGB(255, 0, 0);
      break;
    case 1:
      RGB(0, 1, 0);
      break;
    case 2:
      RGB(0, 0, 1);
      break;
    default: break;
  }
}
void RGB(short r, bool g, bool b){
  analogWrite(RED, 255 - r);
  digitalWrite(GREEN, !g);
  digitalWrite(BLUE, !b);
}
//---------------------------------------------------------------------//
void BUZZ(short type){
  switch (type) {
    case 0: // start up
      tone(BUZZER, 1047, 80);  delay(80);  // Quick C6
      tone(BUZZER, 1319, 80);  delay(80);  // Quick E6
      tone(BUZZER, 1568, 150); delay(150); // Sustained G6
      noTone(BUZZER);
      break;
    case 1: // selecting
      tone(BUZZER, 2500, 50); delay(50);
      noTone(BUZZER);
      break; 
    case 2: // run ready
      tone(BUZZER, 1047, 60);  delay(60);  // C6
      tone(BUZZER, 1568, 60);  delay(60);  // G6
      tone(BUZZER, 2093, 200); delay(200); // Triumphant C7
      noTone(BUZZER);
      break;
    default: break; 
  }
}
//------------------------------Sensor----------------------------------//
// void readEeprom() {
//   for (int i = 0; i < 8; i++) {
//     compare_value[i] = EEPROM.read(i) * 4;
//   }
// }
// EEPROM.put(addr, compare_value); 
// EEPROM.get(addr, compare_value); 

void calibIR(){ // tính toán khoảng giá trị max, min, ngưỡng sáng tối
  for(int i = 0; i< 2; i++){
    if (IRS_blackVal[i] == 0) IRS_blackVal[i] = 1100;
    if(IR_values[i] < IRS_blackVal[i]) IRS_blackVal[i] = IR_values[i];
    if(IR_values[i] > IRS_whiteVal[i])  IRS_whiteVal[i] = IR_values[i]; 
    IRS_threshold[i] = (IRS_whiteVal[i] + IRS_blackVal[i])/2;
  }
  // BUZZ(2);
}
/*
void updateLine() {
  //Serial.println(sensor, BIN);
  for (int i = 0; i < 8; i++) {
    if (black_value[i] == 0) black_value[i] = 1100;
    if (sensorValue[i] < black_value[i]) black_value[i] = sensorValue[i];
    if (sensorValue[i] > white_value[i]) white_value[i] = sensorValue[i];
    compare_value[i] = (black_value[i] + white_value[i]) / 2;
  }

  if (digitalRead(BUTTON1) == 0) {
    for (int i = 0; i < 8; i++) {
      // compare_value[i] = (black_value[i] + white_value[i]) / 2;
      EEPROM.write(addr + i, compare_value[i] / 4); // devided by 4 cause EEPROM is 8 bit but the ADC on Ard is 10 bit
    }
    digitalWrite(BUZZER, 1);
    delay(100);
    digitalWrite(BUZZER, 0);
  }
  switch (millis() / 100 % 3) {
    case 0:
      digitalWrite(RED, 1);
      digitalWrite(GREEN, 1);
      digitalWrite(BLUE, 0);
      break;
    case 1:
      digitalWrite(RED, 0);
      digitalWrite(GREEN, 1);
      digitalWrite(BLUE, 1);
      break;
    case 2:
      digitalWrite(RED, 1);
      digitalWrite(GREEN, 0);
      digitalWrite(BLUE, 1);
      break;
    default: break;
  }
}
void read_sensor()  // hàm đọc cảm biến
{
  unsigned char temp = 0;
  unsigned int sum = 0;
  unsigned long avg = 0;
  int i, iP, iD;
  int kp;
  int kd;
  int iRet;
  sensorValue[0] = analogRead(A0);
  sensorValue[1] = analogRead(A1);
  sensorValue[2] = analogRead(A2);
  sensorValue[3] = analogRead(A3);
  sensorValue[4] = analogRead(A4);
  sensorValue[5] = analogRead(A5);
  sensorValue[6] = analogRead(A6);
  sensorValue[7] = analogRead(A7);
  for (int j = 0; j < 8; j++) {
    if (isCalib == 0) {
      if (sensorValue[j] < black_value[j]) sensorValue[j] = black_value[j];
      if (sensorValue[j] > white_value[j]) sensorValue[j] = white_value[j];
      sensorPID[j] = map(sensorValue[j], black_value[j], white_value[j], 0, 1000);
    }
    temp = temp << 1;
    if (sensorValue[j] > compare_value[j]) {
      temp |= 0x01;
    } else {
      temp &= 0xfe;
    }
    sensor = temp;
  }
  for (int j = 0; j < 8; j++) {
    avg += (long)(sensorPID[j]) * ((j)*1000);
    sum += sensorPID[j];
  }
  i = (int)((avg / sum) - 3500);
  kp = 1;
  kd = 1;
  iP = kp * i;
  iD = kd * (lastPos - i);
  iRet = (iP - iD);
  if ((iRet < -4500)) {
    iRet = 0;
  }
  servoPwm = iRet / 20;
  lastPos = i;
}
*/
void tcaselect(uint8_t channel) 
{
  if (channel > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();  
}
bool initI2Csensors() 
{
  Wire.begin(); 
  // Wire.setWireTimeout(3000, true); 

  // 1. Initialize ToF Sensors (Channels 0 to 2)
  for (uint8_t i = 0; i < NUM_SENSORS; i++) {
    tcaselect(i);
    // lox[i].setTimeout(500);
    if (!lox[i].init()) {
      Serial.print("ToF Sensor "); 
      Serial.print(i); 
      Serial.println(" failed to initialize!");
      return false; 
    }
    lox[i].startContinuous();
  }

  // 2. Initialize BNO055 IMU (Channel 3)
  tcaselect(IMU_CH);
  bno.reset();
  delay(50); // Short pause for IMU reset cycle to clear

  if (bno.begin() != DFRobot_BNO055_IIC::eStatusOK) {
    Serial.println("FAILED!");
    return false;
  }
  Serial.println("SUCCESS!");
  return true; 
}
void readTOF(uint8_t distances[]) 
{
  for (uint8_t i = 0; i < NUM_SENSORS; i++) {
    tcaselect(i);
    
    if (lox[i].timeoutOccurred()) {
      distances[i] = 0; // Error fallback value
    } else {
      distances[i] = lox[i].readRangeContinuousMillimeters();
    }
  }
}
void readIMU(float IMU_values[]) 
{
  tcaselect(IMU_CH);
  DFRobot_BNO055_IIC::sEulAnalog_t sEul = bno.getEul();
  
  IMU_values[0] = sEul.roll;
  IMU_values[1] = sEul.pitch;
  IMU_values[2] = sEul.head; // '.head' represents the Yaw/Heading angle in the DFRobot library
}
void readIR(uint8_t IR_values[]){

  IR_values[0] = analogRead(A0);
  IR_values[1] = analogRead(A1);
  IR_values[2] = analogRead(A3);
}
//-----------------------------Strategy--------------------------------//
bool isEnemyDetected(uint8_t TOF_distance[]){
  for (short i =0; i<NUM_SENSORS; i++){
    if(TOF_distance[i]<250) return 1; 
  }
  return 0;
}
bool isEdgeDetected(uint8_t IR_values[]){
  if(IR_values[0] < 100 || IR_values[1] < 100 || IR_values[2] < 100) return 1;
  return 0;
}
//------------------------------Motor----------------------------------//
void handleAndSpeed(int speed1, int angle) 
{
  speed_run(speed1 + angle, speed1 - angle);
}


void speed_run(int speedDC_left, int speedDC_right)  // hàm truyền vào tốc độ động cơ trái + phải
{
  if (speedDC_left < 0) {
    analogWrite(LPWM, 255 + speedDC_left);
    digitalWrite(LDIR, HIGH);
  } else if (speedDC_left >= 0) {
    speedDC_left = speedDC_left;
    analogWrite(LPWM, speedDC_left);
    digitalWrite(LDIR, LOW);
  }
  if (speedDC_right < 0) {
    analogWrite(RPWM, 255 + speedDC_right);
    digitalWrite(RDIR, HIGH);
  } else if (speedDC_right >= 0) {
    speedDC_right = speedDC_right;
    analogWrite(RPWM, speedDC_right);
    digitalWrite(RDIR, LOW);
  }
}
