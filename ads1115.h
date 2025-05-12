#ifndef ADS1115_H
#define ADS1115_H

#include <stdint.h>

#define ADS1115_ADDR_GND 0x48  // Địa chỉ mặc định nếu ADDR nối GND

// Các hằng số cấu hình
#define ADS1115_REG_CONVERSION 0x00
#define ADS1115_REG_CONFIG     0x01

// MUX cho single-ended mode
#define ADS1115_MUX_AIN0       0x4000
#define ADS1115_MUX_AIN1       0x5000
#define ADS1115_MUX_AIN2       0x6000
#define ADS1115_MUX_AIN3       0x7000

// PGA - Programmable Gain Amplifier
#define ADS1115_PGA_6_144V     0x0000
#define ADS1115_PGA_4_096V     0x0200
#define ADS1115_PGA_2_048V     0x0400  // Default
#define ADS1115_PGA_1_024V     0x0600
#define ADS1115_PGA_0_512V     0x0800
#define ADS1115_PGA_0_256V     0x0A00

// Mode
#define ADS1115_MODE_CONTINUOUS 0x0000
#define ADS1115_MODE_SINGLESHOT 0x0100

// Data Rate (samples per second)
#define ADS1115_DR_128SPS       0x0080  // default

typedef struct {
    int i2c_fd;
    uint8_t address;
} ADS1115;

int ads1115_init(ADS1115 *dev, const char *i2c_dev_path, uint8_t addr);
int ads1115_read_single_ended(ADS1115 *dev, uint8_t channel);
void ads1115_close(ADS1115 *dev);

#endif
