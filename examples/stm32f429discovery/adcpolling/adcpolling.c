#include <gpio.h>
#include <adc.h>
#include <stdio.h>

int main() {
    //declare the pin structures for the sensor
    //We plug our first sensor on PA0
    pin_t sensor1 = make_pin(GPIO_PORT_A, 0);
    //and the second sensor on PA1
    pin_t sensor2 = make_pin(GPIO_PORT_A, 1);

    //configure the ADC
    //PA0 is connected to ADC 1, 2 or 3, channel 0
    //We use ADC 1
    adc_config_single(1, 0, sensor1);
    //PA1 is connected to ADC 1, 2 or 3, channel 1
    //We use ADC 2
    adc_config_single(2, 1, sensor2);

    //Loop
    while (1)
        printf("x=%4d y=%4d\n", adc_get(1), adc_get(2));

    return 0;
}
