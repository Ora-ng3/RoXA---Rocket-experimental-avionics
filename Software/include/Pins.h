// Pin definitions for ROXA
#ifndef PINS_H
#define PINS_H

constexpr int STEP = 0;
constexpr int DIR = 1;
constexpr int PYRO1 = 2;
constexpr int PYRO2 = 3;
constexpr int PYRO3 = 4;
constexpr int PYRO4 = 5;
constexpr int LED_B = 6;
constexpr int RX = 7;
constexpr int TX = 8;
constexpr int LED_G = 9;
constexpr int BUZZER = 10;
// MOSI = 11
// MISO = 12
//constexpr int SCK = 13; (already defined in core_pins.h from teensy)
constexpr int BRK_14 = 14; // Top
constexpr int BRK_15 = 15; // Bottom
constexpr int BRK_16 = 16;
constexpr int BRK_17 = 17;
// SDA = 18
// SCL = 19
// 20 : unused (A6, TX5, LRCLK1)
constexpr int BATT = 21;
constexpr int MOT_1 = 22;
constexpr int MOT_2 = 23;
constexpr int LED_R = 24;
constexpr int PYRO_CTY1 = 25;
constexpr int PYRO_CTY2 = 26;
constexpr int PYRO_CTY3 = 27;
constexpr int PYRO_CTY4 = 28;
constexpr int SERVO_1 = 29;
// 30 : unused (CRX3)
// 31 : unused (CTX3)
// 32 : unused (OUT1B)
constexpr int SERVO_4 = 33;
constexpr int CAM_4 = 34;
constexpr int CAM_1 = 35;
constexpr int SERVO_3 = 36;
constexpr int SERVO_2 = 37;
constexpr int CAM_2 = 38;
constexpr int CAM_3 = 39;
constexpr int NRF_CE = 40;
constexpr int NRF_CSN = 41;

#endif