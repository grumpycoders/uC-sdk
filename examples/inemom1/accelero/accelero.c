#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>

#include <gpio.h>
#include <i2c.h>
#include <lsm303dlhc.h>
lsm303dlhc_a_t accelero;
lsm303dlhc_m_t magneto;

float a_values[3], m_values[3];

void initAccelero(){
    i2c_port_t i2c = {
        .scl = make_pin(gpio_port_b, 10),
        .sda = make_pin(gpio_port_b, 11),
        .i2c = i2c_2 };
    accelero.odr = LSM303DLHC_A_400HZ;
    accelero.fs  = LSM303DLHC_A_4G;
    accelero.res = LSM303DLHC_A_HIGH_RESOLUTION;
    accelero.en  = LSM303DLHC_A_LITTLE_ENDIAN;//LSM303DLHC_A_BIG_ENDIAN;
    lsm303dlhc_accelero_init(&accelero, i2c);
}

void initMagneto(){
    i2c_port_t i2c = {
        .scl = make_pin(gpio_port_b, 10),
        .sda = make_pin(gpio_port_b, 11),
        .i2c = i2c_2 };
    magneto.odr = LSM303DLHC_M_75HZ;
    magneto.gain = LSM303DLHC_M_1_9GAUSS;
    lsm303dlhc_magneto_init(&magneto, i2c);
}

void acceleroTask(){
    while(1)
    {
        lsm303dlhc_accelero_read(accelero, a_values);
        lsm303dlhc_magneto_read(magneto, m_values);

        printf("A x:%5.2f y:%5.2f z:%5.2f\n", a_values[0], a_values[1], a_values[2]);
        printf("M x:%5.2f y:%5.2f z:%5.2f\n", m_values[0], m_values[1], m_values[2]);
        vTaskDelay(1000);
    }
}


int main() {
    printf("Init accelero\n");
    initAccelero();
    printf("Init magneto\n");
    initMagneto();

    printf("Start task\n");
    xTaskCreate(acceleroTask, (const signed char *)NULL, configMINIMAL_STACK_SIZE, (void *)NULL, tskIDLE_PRIORITY, NULL);

    vTaskStartScheduler();

    return 0;
}
