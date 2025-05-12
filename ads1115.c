#include "ads1115.h"
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stdio.h>

int ads1115_init(ADS1115 *dev, const char *i2c_dev_path, uint8_t addr) {
    dev->i2c_fd = open(i2c_dev_path, O_RDWR);
    if (dev->i2c_fd < 0) {
        perror("Open I2C device failed");
        return -1;
    }

    if (ioctl(dev->i2c_fd, I2C_SLAVE, addr) < 0) {
        perror("I2C set address failed");
        close(dev->i2c_fd);
        return -1;
    }

    dev->address = addr;
    return 0;
}

int ads1115_read_single_ended(ADS1115 *dev, uint8_t channel) {
    if (channel > 3) return -1;

    uint16_t config = 0x8000 | // Start single conversion
                      ADS1115_MODE_SINGLESHOT |
                      ADS1115_DR_128SPS |
                      ADS1115_PGA_2_048V;

    switch (channel) {
        case 0: config |= ADS1115_MUX_AIN0; break;
        case 1: config |= ADS1115_MUX_AIN1; break;
        case 2: config |= ADS1115_MUX_AIN2; break;
        case 3: config |= ADS1115_MUX_AIN3; break;
    }

    uint8_t buffer[3];
    buffer[0] = ADS1115_REG_CONFIG;
    buffer[1] = (config >> 8) & 0xFF;
    buffer[2] = config & 0xFF;

    if (write(dev->i2c_fd, buffer, 3) != 3) {
        perror("Write config failed");
        return -1;
    }

    usleep(8500); // Chờ 8.5ms cho 128SPS

    buffer[0] = ADS1115_REG_CONVERSION;
    if (write(dev->i2c_fd, buffer, 1) != 1) {
        perror("Set read reg failed");
        return -1;
    }

    if (read(dev->i2c_fd, buffer, 2) != 2) {
        perror("Read conversion failed");
        return -1;
    }

    int16_t value = (buffer[0] << 8) | buffer[1];
    return value;
}

void ads1115_close(ADS1115 *dev) {
    if (dev->i2c_fd >= 0)
        close(dev->i2c_fd);
}
