#pragma once

#include <decl.h>
#include <stdint.h>
#include <stdbool.h>

BEGIN_DECL

typedef struct {
    uint8_t rtc_hour;
    uint8_t rtc_minute;
    uint8_t rtc_second;
} rtc_time_t;

typedef enum {
  monday = 1,
  tuesday = 2,
  wednesday = 3,
  thursday = 4,
  friday = 5,
  saturday = 6,
  sunday = 7
} rtc_weekday_t;

typedef enum {
  january = 1,
  february = 2,
  march = 3,
  april = 4,
  may = 5,
  june = 6,
  july = 7,
  august = 8,
  september = 9,
  october = 10,
  november = 11,
  december = 12
} rtc_month_t;

typedef struct {
    uint16_t rtc_year;
    rtc_month_t rtc_month;
    uint8_t rtc_day;
    rtc_weekday_t rtc_weekday;
} rtc_date_t;

typedef enum {
  rtc_alarm_1,
  rtc_alarm_2
} rtc_alarm_t;

typedef enum {
    rtc_every_second,
    rtc_every_minute,
    rtc_every_hour,
    rtc_every_day
} rtc_repeat_t;

/*
    Initializes the RTC clock
*/
void rtc_init(rtc_date_t *date, rtc_time_t *time);

/*
    Sets the current time
*/
void rtc_set_time(rtc_time_t *time);

/*
    Gets the current time
*/
rtc_time_t *rtc_get_time();

/*
    Sets the current date
*/
void rtc_set_date(rtc_date_t *date);

/*
    Gets the current date
*/
rtc_date_t *rtc_get_date();

/*
    Gets the epoch value from a date and time
*/
uint32_t rtc_date_time_to_epoch(rtc_date_t* date, rtc_time_t *time);

/*
    Sets a callback to be executed at a specified moment
*/
void rtc_set_alarm(rtc_alarm_t alarm, rtc_time_t *time, rtc_repeat_t pattern, void (*cb)(), void *parameter);

END_DECL
