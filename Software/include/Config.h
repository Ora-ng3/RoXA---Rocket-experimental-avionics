// dimensions, expected apogee, expected deploy delay, main deploy altitude
// main deploy consec frames (nb of frames where conditions have to be met)
// apogee minimum time (min tome since liftoff to check for apogee)

//#define PSRAM
//#define FLASH
#define SD_LOG
//#define FINE_ACC_GYRO_CALIBRATION
//#define CANSAT
#define REACTION_WHEEL

#define GyroXOffset 82
#define GyroYOffset -18
#define GyroZOffset 56
#define AccelXOffset -927
#define AccelYOffset -273
#define AccelZOffset 953

#define SLOW_LOG_RATE 1 // Hz
#define FAST_LOG_RATE 10 // Hz

#define FPS 10 // Hz

#define BARO_DROP 5 // m
#define BARO_ASCENDING_TRIGGER 5 // m
#define BARO_MAIN_DEPLOY 200 // m

#define LED_BRIGHTNESS 0.1

//#define TEST