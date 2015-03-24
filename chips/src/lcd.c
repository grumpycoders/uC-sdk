#include "lcd.h"

enum {
    CLEAR         = 0x01,
    HOME          = 0x02,
    ENTRY         = 0x04,
    DISPLAY       = 0x08,
    CDSHIFT       = 0x10,
    FUNCTION      = 0x20,
    SETCGRAMADDR  = 0x40,
    SETDDRAMADDR  = 0x80,
} lcd_command_t;

enum {
    SHIFT         = 0x01,
    CURSORLEFT    = 0x00,
    CURSORRIGHT   = 0x02,
} lcd_entry_option;

enum {
    CURSORBLINK   = 0x01,
    CURSOR        = 0x02,
    EVERYTHING    = 0x04,
} lcd_display_option;

enum {
    CURLEFT       = 0x00,
    CURRIGHT      = 0x40,
    DISPLEFT      = 0x80,
    DISPRIGHT     = 0xc0,
} lcd_cdshift_option;

enum {
    RES8X8DOTS    = 0x00,
    RES5X11DOTS   = 0x04,
    COM1LINE      = 0x00,
    COM2LINE      = 0x08,
    COM4BITS      = 0x00,
    COM8BITS      = 0x10,
} lcd_function_option;

enum {
    NOTBUSY       = 0x00,
    BUSY          = 0x80,
} lcd_state;

//FIX THAT
void delay(lcd_t *lcd, uint32_t t)
{
    uint32_t i;
    for (i = 0 ; i < t ; i++)
        ssp_write(lcd->ssp, 0xff);
}

//we will have to change if we abstract 
void write_register(lcd_t *lcd, uint8_t byte, int rs, int rw)
{
    gpio_set(lcd->rs, rs);
    gpio_set(lcd->rw, rw);

    //if (lcd->type == COM8BITS)
    {
        gpio_set(lcd->cs, 0);
        ssp_write(lcd->ssp, byte);
        gpio_set(lcd->cs, 1);
    }
/*    else
    {
        gpio_set(lcd->cs, 0);
        ssp_write(lcd->ssp, byte);
        gpio_set(lcd->cs, 1);
    }*/

    gpio_set(lcd->e, 0);
    delay(lcd, 1);
    gpio_set(lcd->e, 1);
    delay(lcd, 1); //min: 300ns
    gpio_set(lcd->e, 0);
    delay(lcd, 80); //50us for most of operations
}

int lcd_init(lcd_t *lcd, ssp_port_t ssp_port, pin_t cs, pin_t rs, pin_t rw, pin_t e)
{
    lcd->ssp = ssp_port.ssp;
    lcd->cs = cs;
    lcd->rs = rs;
    lcd->rw = rw;
    lcd->e = e;
    //lcd->type = COM8BITS;

    gpio_config(cs, pin_dir_write, pull_up);
    gpio_set(cs, 1);
    gpio_config(rs, pin_dir_write, pull_down);
    gpio_set(rs, 0);
    gpio_config(rw, pin_dir_write, pull_down);
    gpio_set(rw, 0);
    gpio_config(e, pin_dir_write, pull_down);    
    gpio_set(e, 0);

    ssp_config(ssp_port, 8 * 1000 * 1000);

    delay(lcd, 1000);

    lcd_clear_display(lcd);
    lcd_return_home(lcd);
    lcd_set_display(lcd,1, 1, 1);

    return 1;
}

void lcd_clear_display(lcd_t *lcd)
{
    write_register(lcd, CLEAR, 0, 0);
    delay(lcd, 200); //1.53ms
}

void lcd_return_home(lcd_t *lcd)
{
    write_register(lcd, HOME, 0, 0);
    delay(lcd, 200); //1.53ms
}

void lcd_set_display(lcd_t *lcd, uint8_t display, uint8_t cursor, uint8_t cursorblink)
{
    uint8_t byte = DISPLAY;
    if (display)
        byte |= EVERYTHING;
    if (cursor)
        byte |= CURSOR;
    if (display)
        cursorblink |= CURSORBLINK;
    write_register(lcd, byte, 0, 0);
}

void lcd_move_to(lcd_t *lcd, uint8_t line, uint8_t column)
{
    if (line > 1 || column > 0x0f)
        return;

    write_register(lcd, SETDDRAMADDR | (line * 0x40 + column), 0, 0);
}
/*
location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    write(charmap[i]);
  }
  */
void lcd_print_char(lcd_t *lcd, char c)
{
    write_register(lcd, (uint8_t)c, 1, 0);
}

void lcd_print_string(lcd_t *lcd, char *c)
{
    uint8_t i = 0;
    while (*c && ++i < 16)
        lcd_print_char(lcd, *(++c));
}

