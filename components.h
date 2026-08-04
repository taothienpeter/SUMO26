#include <stdint.h>
#ifndef COMPONENTS_H
#define COMPONENTS_H

//---------------------------------------------------
#include <avr/interrupt.h>
// #include <EEPROM.h>
#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <DFRobot_BNO055.h>
//---------------------------------------------------
#define RDIR 11// chiều quay động cơ phải
#define LDIR 9// chiều quay động cơ trái
#define LPWM 5// tốc độ động cơ trái
#define RPWM 6// tốc độ động cơ phải
#define BUTTON1 7// nút nhấn 1
#define BUTTON2 8// nút nhấn 2
#define BUTTON3 12// nút nhấn 3
#define BUZZER 4// chân điều khiển loa
#define RED 3// Chân RED trên mạch
#define GREEN 2// Chân GREEN trên mạch
#define BLUE 13// Chân BLUE trên mạch

#define NUM_SENSORS 3
#define IMU_CH 3      // Multiplexer channel for the BNO055 IMU
//---------------------------------------------------//
#define TCAADDR 0x70
inline VL53L0X lox[NUM_SENSORS];
inline DFRobot_BNO055_IIC bno(&Wire, 0x28);

#define ENEMY_THRESHOLD 200
//---------------------------------------------------//
inline int addr = 0;

inline volatile int lastPos = 0;
inline volatile uint8_t isCalib = 0;
inline volatile int servoPwm = 0;
inline volatile uint8_t sensor = 0;

inline int cnt = 0;
inline uint8_t state = 0;
inline unsigned long stateTimer = 0;
inline unsigned long lastEnemyTime = 0;

inline int avoidPhase = 0;
inline bool isSystemActive = false;
//--------------------------------------------------------//   
inline uint8_t IR_values[3] = {}; //init for testing
inline uint8_t IRS_threshold[3] = {};
inline uint8_t IRS_whiteVal[3] = {};
inline uint8_t IRS_blackVal[3] = {};

inline uint8_t TOF_distance[NUM_SENSORS] = {0}; 
inline float IMU_values[3] = {};
inline float IMU_AccValues[3] = {};
//----------------------------------------------------//
enum SumoState {
    STATE_CALIB,         
    STATE_IDLE,      
    STATE_SEARCH,       
    STATE_ATTACK,       
    STATE_EDGE_AVOID    
};
//----------------------------------------------------//
void logSerial(String s);
void timer_init();

void RGB_loading();
void RGB(short r, bool g, bool b);
void BUZZ(short type);

// void readEeprom();
void calibIR(); 
// void updateLine();
void tcaselect(uint8_t channel);
bool initI2Csensors();
void readTOF(uint8_t distances[]);
void readIMU(float IMU_values[], float IMU_AccValues[]);
void readIR(uint8_t IR_values[]); 

bool isEnemyDetected(uint8_t TOF_distance[]);
bool isEdgeDetected(uint8_t IR_values[]);
float turnFactor(uint8_t TOF_distance[]);

void littleBitchDrive(float IMU_values[]);

void simpleDrive(int speed1, int angle);
void vectorDrive(float speed2, float turnFactor);
void speed_run( int speedDC_left, int speedDC_right);
#endif