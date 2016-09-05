#include <gpio.h>
#include <adc.h>
#include <stdio.h>

int main() {
    //declare the pin structures for the sensor
    pin_t sensors[2];
    //We plug our first sensor on PA0
    sensors[0] = make_pin(GPIO_PORT_A, 0);
    //and the second sensor on PA1
    sensors[1] = make_pin(GPIO_PORT_A, 1);

    //declare an array where the DMA will put the values
    uint16_t values[] = {0, 0};

    //configure the ADC
    //PA0 is connected to ADC 1, 2 or 3, channel 0
    //PA1 is connected to ADC 1, 2 or 3, channel 1
    //We use ADC 1, with channels 0 and 1
    uint8_t channels[] = {0, 1};
    adc_config_continuous(1, channels, sensors, values, 2);

    //Loop
    while (1)
        printf("x=%4d y=%4d\n", values[0], values[1]);

    return 0;
}
