#include <stdio.h>
#include <malloc.h>

#include <gpio.h>
#include <rtc.h>

void alarm1(void *parameter) {
  //Cast the parameter back to its origin type
  pin_t *pin = (pin_t *)parameter;
  gpio_set(*pin, 1);

  rtc_date_t *d = rtc_get_date();
  rtc_time_t *t = rtc_get_time();

  printf("Alarm A %d/%d/%d %d:%d:%d\n", d->rtc_day, d->rtc_month, d->rtc_year, t->rtc_hour, t->rtc_minute, t->rtc_second);
}

void alarm2(void *parameter) {
  //Cast the parameter back to its origin type
  pin_t *pin = (pin_t *)parameter;
  gpio_set(*pin, 0);

  rtc_date_t *d = rtc_get_date();
  rtc_time_t *t = rtc_get_time();

  printf("Alarm B %d\n", rtc_date_time_to_epoch(d, t));
}

int main() {
  pin_t *pin = malloc(sizeof(pin_t));
  *pin = make_pin(gpio_port_g, 13);

  gpio_config(*pin, pin_dir_write, pull_down);

  rtc_date_t date = {
    .rtc_year = 2015,
    .rtc_month = october,
    .rtc_day = 21
  };

  rtc_time_t time = {
    .rtc_hour = 4,
    .rtc_minute = 29,
    .rtc_second = 0
  };
  rtc_init(&date, &time);

  rtc_set_alarm(rtc_alarm_1, &time, rtc_every_second, alarm1, (void *)pin);

  time.rtc_second = 5;
  rtc_set_alarm(rtc_alarm_2, &time, rtc_every_minute, alarm2, (void *)pin);

  while(1) {};

  return 0;
}
