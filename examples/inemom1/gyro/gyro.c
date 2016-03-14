#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>

#include <gpio.h>
#include <l3gd20.h>

l3gd20_t gyro;
float gyrovalues[3];

void initGyro()
{
    pin_t sclk = make_pin(GPIO_PORT_B, 13);
    pin_t mosi = make_pin(GPIO_PORT_B, 15);
    pin_t miso = make_pin(GPIO_PORT_B, 14);
    pin_t cs   = make_pin(GPIO_PORT_B, 12);
    ssp_port_t gyroport = { .ssp = ssp_port_2, .sclk = sclk, .mosi = mosi, .miso = miso };

    if (!l3gd20_init_ssp(&gyro, gyroport, cs))
        printf("Cannot initialize gyroscope");
}

void gyroTask()
{
    while(1)
    {
        l3gd20_read(gyro, gyrovalues);
        printf("x:%.2f y:%.2f z:%.2f\n", gyrovalues[0], gyrovalues[1], gyrovalues[2]);

        vTaskDelay(10);
    }
}


int main() {
    printf("Init gyro\n");
    initGyro();

    printf("Start task\n");
    xTaskCreate(gyroTask, (const signed char *)NULL, configMINIMAL_STACK_SIZE, (void *)NULL, tskIDLE_PRIORITY, NULL);

    //start task scheduler
    vTaskStartScheduler();

    return 0;
}
