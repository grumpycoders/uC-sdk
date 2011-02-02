#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <lpc17xx_gpio.h>
#include <BoardConsole.h>
#include <osdebug.h>
#include <stdio.h>
#include <fio.h>
#include <romfs.h>
#include <semifs.h>

#define LED1_wire 18
#define LED2_wire 20
#define LED3_wire 21
#define LED4_wire 23

static void setupLEDs() {
    GPIO_SetDir(1, (1 << LED1_wire) | (1 << LED2_wire) | (1 << LED3_wire) | (1 << LED4_wire), 1);
}

static void litLED(int led, int value) {
    if ((led > 4) || (led < 0))
        return;
    
    switch (led) {
        case 1: led = 1 << LED1_wire; break;
        case 2: led = 1 << LED2_wire; break;
        case 3: led = 1 << LED3_wire; break;
        case 4: led = 1 << LED4_wire; break;
    }
    
    if (value) {
        GPIO_SetValue(1, led);
    } else {
        GPIO_ClearValue(1, led);
    }
}

xSemaphoreHandle handle;

static void simpleTask1(void *p) {
    while (1) {
        xSemaphoreTake(handle, portMAX_DELAY);
        BoardConsolePuts("Task 1");
        xSemaphoreGive(handle);
        vTaskDelay(1234);
    }
}

static void simpleTask2(void *p) {
    while (1) {
        xSemaphoreTake(handle, portMAX_DELAY);
        BoardConsolePuts("Task 2");
        xSemaphoreGive(handle);
        vTaskDelay(1357);
    }
}

static void badTask(void *x) {
    vTaskDelay(5000);
    char * p = (char *) 0x10000000;
    *p = 42;
}

static const char msg[] = "Hello world - from fwrite!\r\n";

extern uint8_t _binary_test_romfs_bin_start[];

int main() {
    FILE * f1, * f2, * f3;
    char buf[32];
    int c;
    register_devfs();
    register_semifs();
    register_romfs("romfs", _binary_test_romfs_bin_start);
    handle = xSemaphoreCreateMutex();
    printf("Hello world - from stdio!\r\n");
    fflush(stdout);
    f1 = fopen("/dev/stdout", "w");
    fwrite(msg, 1, sizeof(msg), f1);
    f2 = fopen("/romfs/test.txt", "r");
    c = fread(buf, 1, 32, f2);
    fwrite(buf, 1, c, f1);
    f3 = fopen("/host/TEST.TXT", "r");
    c = fread(buf, 1, 32, f3);
    fwrite(buf, 1, c, f1);
    fflush(f1);
    fclose(f1);
    fclose(f2);
    fclose(f3);
    setupLEDs();
    litLED(1, 0);
    litLED(2, 0);
    litLED(3, 0);
    litLED(4, 0);
    BoardConsolePuts("Creating simple tasks.");
    xTaskCreate(simpleTask1, (signed char *) "st1", configMINIMAL_STACK_SIZE, (void *)NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(simpleTask2, (signed char *) "st2", configMINIMAL_STACK_SIZE, (void *)NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(badTask, (signed char *) "bad", configMINIMAL_STACK_SIZE, (void *)NULL, tskIDLE_PRIORITY, NULL);
    BoardConsolePuts("Scheduler starting.");
    vTaskStartScheduler();
    BoardConsolePuts("Scheduler exitting.");
    return 0;
}
