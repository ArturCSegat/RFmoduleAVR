#ifndef RF_MOTOR_COMMON_H
#define RF_MOTOR_COMMON_H


#define VARIANCE_PERCENTAGE (0.1)
#define BASE_ERROR (50)
#define _acceptable(att, target) ((att) >= ((target) - ((target) * VARIANCE_PERCENTAGE + BASE_ERROR)) && (att) <= ((target) + (target) * VARIANCE_PERCENTAGE + BASE_ERROR))

#define HIGH_LEN (300)
#define ONE_LEN_US (700)
#define ZERO_LEN_US (550)
#define TRANS_PREAMBLE_BYTES 30
#define BYTE_INTERVAL 5000

#define NOISE_BYTE_ERROR 255


#endif

