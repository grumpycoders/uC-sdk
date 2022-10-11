#include <gpio.h>
#include <malloc.h>
#include <FreeRTOS.h>
#include <task.h>

void callback(void *parameter) {
    uint8_t status = 0;
    //Cast the parameter back to its origin type
    pin_t *pin = (pin_t *)parameter;
    while(1) {
        status ^= 1;
        gpio_set(*pin, status);
        //Sleep
        vTaskDelay(1000);
    }
}

int main() {
    //Initialize the pin_t structure with the pin port and number
    //On this board there is a LED on PG13
    pin_t *pin = malloc(sizeof(pin_t));
    *pin = make_pin(gpio_port_g, 13);

    //configure the pin for output.
    gpio_config(*pin, pin_dir_write, pull_down);

    //Create the task
    xTaskCreate(callback,           //callback function
        (const signed char *)NULL,  //task name
        configMINIMAL_STACK_SIZE,   //stack size
        (void *)pin,               //parameter (cast it to void *)
        tskIDLE_PRIORITY,
        NULL);

    //Run the tasks
    vTaskStartScheduler();

    return 0;
}
