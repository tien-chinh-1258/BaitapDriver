#include "ads1115.h"
#include <stdio.h>

int main() {
    ADS1115 adc;

    if (ads1115_init(&adc, "/dev/i2c-1", ADS1115_ADDR_GND) < 0) {
        return 1;
    }

    while (1) {
        int value = ads1115_read_single_ended(&adc, 0);
        float voltage = (value * 2.048f) / 32768.0f;

        printf("ADC AIN0 = %d, Voltage = %.4f V\n", value, voltage);
        usleep(500000); // 0.5 giây
    }

    ads1115_close(&adc);
    return 0;
}

