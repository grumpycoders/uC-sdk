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
#include <malloc_wrapper.h>
#include <lwip/inet.h>
#include <lwip/tcp.h>
#include <lwip/ip_frag.h>
#include <lwip/netif.h>
#include <lwip/init.h>
#include <lwip/stats.h>
#include <lwip/tcp_impl.h>
#include <lwip/timers.h>
#include <netif/etharp.h>
#include <netif/lpc17xx-if.h>
#include <webserver/httpd.h>
#include <echo/echo.h>
#include <lwip/dhcp.h>

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

#ifdef SHOW_SIMPLE_TASKS
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
#endif

#ifdef USE_BAD_TASK
static void badTask(void *x) {
    vTaskDelay(5000);
    char * p = (char *) 0x10000000;
    *p = 42;
}
#endif

static const char msg[] = "Hello world - from fwrite!\r\n";

extern uint8_t _binary_test_romfs_bin_start[];

struct ip_addr board_ip;
static struct netif board_netif;
static portTickType ts_etharp, ts_tcp, ts_ipreass;
static xSemaphoreHandle lwip_sem;

#define USE_DHCP

static void net_init() {
    struct ip_addr gw_ip, netmask;
#ifdef USE_DHCP
    IP4_ADDR(&board_ip, 0, 0, 0, 0);
    IP4_ADDR(&gw_ip, 0, 0, 0, 0);
    IP4_ADDR(&netmask, 0, 0, 0, 0);
#else
    inet_aton("192.168.1.2", &board_ip.addr);
    inet_aton("192.168.1.1", &gw_ip.addr);
    inet_aton("255.255.255.0", &netmask.addr);
#endif
    lwip_init();
    vSemaphoreCreateBinary(lwip_sem);
    
    if (netif_add(&board_netif, &board_ip, &netmask, &gw_ip, NULL, lpc17xx_if_init, ethernet_input) == NULL) {
        fprintf(stderr, "net_init: netif_add(lpc17xx_if_init) failed.\r\n");
        return;
    }
    netif_set_default(&board_netif);

#ifdef USE_DHCP
    dhcp_start(&board_netif);
#else
    netif_set_up(&board_netif);
#endif

    ts_etharp = ts_tcp = ts_ipreass = 0;
}

static inline int timestamp_expired(portTickType t, portTickType now, portTickType delay) {
    return (int)(now - (t + delay)) >= 0;
}

uint32_t sys_now() { return xTaskGetTickCount(); }

static void lwip_poll(struct netif * netif) {
    portTickType now = xTaskGetTickCount();

    lpc17xx_if_check_input(netif);
    if (timestamp_expired(ts_etharp, now, ARP_TMR_INTERVAL / portTICK_RATE_MS)) {
        etharp_tmr();
        ts_etharp = now;
    }
    
    if (timestamp_expired(ts_tcp, now, TCP_TMR_INTERVAL / portTICK_RATE_MS)) {
        tcp_tmr();
        ts_tcp = now;
    }
    
    if (timestamp_expired(ts_ipreass, now, IP_TMR_INTERVAL / portTICK_RATE_MS)) {
        ip_reass_tmr();
        ts_ipreass = now;
    }

    sys_check_timeouts();
}

static void lwip_task(void * p) {
    net_init();
#ifdef USE_DHCP
    uint32_t time = 0;
    printf("Starting DHCP query\n");
    while (board_netif.ip_addr.addr == 0) {
        if (time >= DHCP_COARSE_TIMER_SECS * 1000) {
            dhcp_coarse_tmr();
            xSemaphoreTake(lwip_sem, DHCP_COARSE_TIMER_SECS * 1000 / portTICK_RATE_MS);
        } else {
            dhcp_fine_tmr();
            xSemaphoreTake(lwip_sem, DHCP_FINE_TIMER_MSECS / portTICK_RATE_MS);
            time += DHCP_FINE_TIMER_MSECS;
        }
        lwip_poll(&board_netif);
    }

    printf("DHCP query complete; address = %i.%i.%i.%i\n",
        (board_netif.ip_addr.addr >>  0) & 0xff,
        (board_netif.ip_addr.addr >>  8) & 0xff,
        (board_netif.ip_addr.addr >> 16) & 0xff,
        (board_netif.ip_addr.addr >> 24) & 0xff);
#endif
    httpd_init(_binary_test_romfs_bin_start);
    echo_init();
    while(1) {
        xSemaphoreTake(lwip_sem, 10 / portTICK_RATE_MS);
        lwip_poll(&board_netif);
    }
}

void ENET_IRQHandler() {
    uint32_t status = LPC_EMAC->IntStatus;
    long woken = pdFALSE;
    LPC_EMAC->IntClear = status;
    xSemaphoreGiveFromISR(lwip_sem, &woken);
    portEND_SWITCHING_ISR(woken);
}

int main() {
    init_malloc_wrapper();
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
    fprintf(stderr, "f1 = %p\r\n", f1);
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
#ifdef SHOW_SIMPLE_TASKS
    xTaskCreate(simpleTask1, (signed char *) "st1", configMINIMAL_STACK_SIZE, (void *)NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(simpleTask2, (signed char *) "st2", configMINIMAL_STACK_SIZE, (void *)NULL, tskIDLE_PRIORITY, NULL);
#endif
#ifdef USE_BAD_TASK
    xTaskCreate(badTask, (signed char *) "bad", configMINIMAL_STACK_SIZE, (void *)NULL, tskIDLE_PRIORITY, NULL);
#endif
    xTaskCreate(lwip_task, (signed char *) "lwip", 1024, (void *) NULL, tskIDLE_PRIORITY | portPRIVILEGE_BIT, NULL);
    BoardConsolePuts("Scheduler starting.");
    vTaskStartScheduler();
    BoardConsolePuts("Scheduler exitting.");
    return 0;
}
