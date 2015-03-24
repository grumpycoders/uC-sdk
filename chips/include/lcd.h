#pragma once

#include <gpio.h>
#include <ssp.h>
/*
typedef enum {
    COMM8BITS = 0x00,
    COMM4BITS = 0x01,
} lcd_communication_type;
*/
typedef struct {
    ssp_t ssp;
    pin_t cs;
    pin_t rs;
    pin_t rw;
    pin_t e;
//    lcd_communication_type type;
} lcd_t;

int lcd_init(lcd_t *lcd, ssp_port_t ssp_port, pin_t cs, pin_t rs, pin_t rw, pin_t e);
void lcd_clear_display(lcd_t *lcd);
void lcd_return_home(lcd_t *lcd);
void lcd_set_display(lcd_t *lcd, uint8_t display, uint8_t cursor, uint8_t cursorblink);
void lcd_move_to(lcd_t *lcd, uint8_t line, uint8_t column);
void lcd_print_char(lcd_t *lcd, char c);
void lcd_print_string(lcd_t *lcd, char *c);

