#ifndef __CONFIG__
#define __CONFIG__


// PIN
#define BTN_PIN     3
#define LED1_PIN    6   // green
#define LED2_PIN    5   // green
#define LED3_PIN    4   // red
#define MOTOR_PIN   10
#define LCD_PORT    0x27
#define TMP_PIN     A0
#define TRIG_PIN    12
#define ECHO_PIN    11
#define PIR_PIN     9

// Servo motor gate
#define GATE_OPENING_DEG   90      // degrees
#define GATE_OPENING_TIME  3000    // ms

// Temperatures time thresholds
#define TEMP_PRE_ALARM_TIME 5000   // ms    T3
#define TEMP_ALARM_TIME 5000       // ms    T4

// Temperatures constraints in °C
#define TEMP_PRE_ALARM  22   // °C  Temp1
#define TEMP_ALARM      30   // °C  Temp2

// PIR presence
#define PIR_SYNC_EXPIRATION 1500  // ms

// Sonar Proximity
#define SONAR_MAX_TIME 3000             // ms
#define TAKE_OFF_SAFE_DISTANCE  0.25    // m    D1
#define LANDING_SAFE_DISTANCE   0.12     // m    D2
#define TAKE_OFF_SAFE_TIME  4000        // ms   T1
#define LANDING_SAFE_TIME   4000        // ms   T2


// LCD
#define LCD_TEST_STRING "! TEST !"
#define LCD_PORT 0x27
#define LCD_COLS_NUM 20
#define LCD_ROWS_NUM 4


#endif
