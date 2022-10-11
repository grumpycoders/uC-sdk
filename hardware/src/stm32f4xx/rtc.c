#include "rtc.h"

#include <stm32f4xx.h>
#include <stm32f4xx_exti.h>
#include <stm32f4xx_rtc.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_pwr.h>
#include <misc.h>

#include <stdio.h>
#include <stddef.h>
#include <malloc.h>

#include "hardware.h"

void rtc_init(rtc_date_t *date, rtc_time_t *time) {
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  PWR_BackupAccessCmd(ENABLE);

  //Reset the RTC config
  RCC_BackupResetCmd(ENABLE);
  RCC_BackupResetCmd(DISABLE);

  RTC_InitTypeDef def;
  RTC_StructInit(&def);

  RCC_HSEConfig(RCC_HSE_ON);
  if (RCC_WaitForHSEStartUp() == ERROR) {
    printf("Error while activating the HSE\n");
    return;
  }
  RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div8);
  //(1 Mhz / 125) = 7999 ticks per second
  def.RTC_AsynchPrediv = 124;
  def.RTC_SynchPrediv = 7999;
  RCC_RTCCLKCmd(ENABLE);

  if (RTC_Init(&def) == ERROR) {
    printf("Error while initializing the RTC\n");
    return;
  }

  RTC_WaitForSynchro();
  rtc_set_time(time);
  rtc_set_date(date);
}

void rtc_set_time(rtc_time_t *time) {
  RTC_TimeTypeDef def;
  def.RTC_H12 = RTC_H12_AM;
  def.RTC_Hours = time->rtc_hour;
  def.RTC_Minutes = time->rtc_minute;
  def.RTC_Seconds = time->rtc_second;
  if (RTC_SetTime(RTC_Format_BIN, &def) == ERROR) {
    printf("Error while setting the RTC time\n");
      return;
  }
}

rtc_time_t *rtc_get_time() {
  RTC_TimeTypeDef def;
  RTC_WaitForSynchro();
  RTC_GetTime(RTC_Format_BIN, &def);
  rtc_time_t *time = (rtc_time_t *)malloc(sizeof(rtc_time_t));
  time->rtc_hour = def.RTC_Hours;
  time->rtc_minute = def.RTC_Minutes;
  time->rtc_second = def.RTC_Seconds;
  return time;
}

void rtc_set_date(rtc_date_t *date) {
  RTC_DateTypeDef def;
  def.RTC_Date = date->rtc_day;
  def.RTC_Month = date->rtc_month;
  def.RTC_WeekDay = date->rtc_weekday;
  def.RTC_Year = date->rtc_year - 1970;
  if (RTC_SetDate(RTC_Format_BIN, &def) == ERROR) {
    printf("Error while setting the RTC date\n");
      return;
  }
}

rtc_date_t *rtc_get_date() {
  RTC_DateTypeDef def;
  RTC_WaitForSynchro();
  RTC_GetDate(RTC_Format_BIN, &def);
  rtc_date_t * date = (rtc_date_t *)malloc(sizeof(rtc_date_t));
  date->rtc_day = def.RTC_Date;
  date->rtc_month = def.RTC_Month;
  date->rtc_weekday = def.RTC_WeekDay;
  date->rtc_year = def.RTC_Year + 1970;
  return date;
}

uint32_t rtc_date_time_to_epoch(rtc_date_t* date, rtc_time_t *time) {
  uint32_t days = 0, seconds = 0;

  for (uint16_t i = 1970 ; i < date->rtc_year; i++) {
    if (((i % 4 == 0) && (i % 100 != 0)) || (i % 400 == 0))
      days += 366;
    else
      days += 365;
  }

  for (rtc_month_t m = january; m < date->rtc_month; m++) {
    switch(m) {
      case april :
      case june:
      case september:
      case november:
        days += 30;
        break;
      case february:
        if (((date->rtc_year % 4 == 0) && (date->rtc_year % 100 != 0)) || (date->rtc_year % 400 == 0))
          days += 29;
        else
          days += 28;
        break;
      default:
        days += 31;
    }
  }

  days += date->rtc_day - 1;
  seconds = days * 24 * 60 * 60;
  seconds += time->rtc_hour * 60 * 60;
  seconds += time->rtc_minute * 60;
  seconds += time->rtc_second;
  
  return seconds;
}

static void (*exti_alarm_callback[2])(void *);
static void *exti_alarm_param[2];

void rtc_set_alarm(rtc_alarm_t alarm, rtc_time_t *time, rtc_repeat_t pattern, void (*cb)(), void *parameter) {
  if (alarm > rtc_alarm_2)
    return;

  exti_alarm_callback[alarm] = cb;
  exti_alarm_param[alarm] = parameter;

  EXTI_ClearITPendingBit(EXTI_Line17);
  EXTI_InitTypeDef exti;
  exti.EXTI_Line = EXTI_Line17;
  exti.EXTI_LineCmd = ENABLE;
  exti.EXTI_Mode = EXTI_Mode_Interrupt;
  exti.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_Init(&exti);

  NVIC_InitTypeDef nvic;
  nvic.NVIC_IRQChannel = RTC_Alarm_IRQn;
  nvic.NVIC_IRQChannelPreemptionPriority = 0x01;
  nvic.NVIC_IRQChannelSubPriority = 0x01;
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);

  RTC_AlarmTypeDef def;
  def.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;
  def.RTC_AlarmTime.RTC_Hours = time->rtc_hour;
  def.RTC_AlarmTime.RTC_Minutes = time->rtc_minute;
  def.RTC_AlarmTime.RTC_Seconds = time->rtc_second;
  def.RTC_AlarmDateWeekDay = 0;
  def.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay;

  switch(pattern) {
    case rtc_every_second:
      def.RTC_AlarmMask = RTC_AlarmMask_All;
      break;
    case rtc_every_minute:
      def.RTC_AlarmMask = RTC_AlarmMask_Minutes | RTC_AlarmMask_Hours | RTC_AlarmMask_DateWeekDay;
      break;
    case rtc_every_hour:
      def.RTC_AlarmMask = RTC_AlarmMask_Hours | RTC_AlarmMask_DateWeekDay;
      break;
    case rtc_every_day:
      def.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
      break;
  }

  uint32_t alarmid = (alarm + 1) << 8;

  if (RTC_AlarmCmd(alarmid, DISABLE) == ERROR) {
    printf("Error while disabling the RTC alarm\n");
      return;
  }
  RTC_SetAlarm(RTC_Format_BIN, alarmid, &def);

  RTC_ITConfig(alarmid << 4,ENABLE);

  if (RTC_AlarmCmd(alarmid, ENABLE) == ERROR) {
    printf("Error while disabling the RTC alarm\n");
      return;
  }

  RTC_ClearITPendingBit(alarmid << 4);

  RTC_ClearFlag(alarmid);
}

void RTC_Alarm_IRQHandler() {
  if (RTC_GetFlagStatus(RTC_FLAG_ALRAF) != RESET) {
    RTC_ClearITPendingBit(RTC_IT_ALRA);
    RTC_ClearFlag(RTC_FLAG_ALRAF); 
    if (exti_alarm_callback[0])
      exti_alarm_callback[0](exti_alarm_param[0]);
  }
  else if (RTC_GetFlagStatus(RTC_FLAG_ALRBF) != RESET) {
    RTC_ClearITPendingBit(RTC_IT_ALRB);
    RTC_ClearFlag(RTC_FLAG_ALRBF); 
    if (exti_alarm_callback[1])
      exti_alarm_callback[1](exti_alarm_param[1]);
  }
  EXTI_ClearITPendingBit(EXTI_Line17);
}
