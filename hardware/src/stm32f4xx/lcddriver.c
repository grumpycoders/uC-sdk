#include "lcddriver.h"

#include <stm32f4xx_ltdc.h>

#define  LCD_PIXEL_WIDTH    ((uint16_t)240)
#define  LCD_PIXEL_HEIGHT   ((uint16_t)320)

#define LCD_FRAME_BUFFER       ((uint32_t)0xD0000000)
#define BUFFER_OFFSET          ((uint32_t)0x50000) 

void lcddriver_config() {
  LTDC_InitTypeDef ltcd;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);

  ltcd.LTDC_HSPolarity = LTDC_HSPolarity_AL;
  ltcd.LTDC_VSPolarity = LTDC_VSPolarity_AL;
  ltcd.LTDC_DEPolarity = LTDC_DEPolarity_AL;
  ltcd.LTDC_PCPolarity = LTDC_PCPolarity_IPC;

  /* Configure R,G,B component values for LCD background color */
  ltcd.LTDC_BackgroundRedValue = 0;
  ltcd.LTDC_BackgroundGreenValue = 0;
  ltcd.LTDC_BackgroundBlueValue = 0;

  /* Configure PLLSAI prescalers for LCD */
  /* Enable Pixel Clock */
  /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
  /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAI_N = 192 Mhz */
  /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAI_R = 192/4 = 48 Mhz */
  /* LTDC clock frequency = PLLLCDCLK / RCC_PLLSAIDivR = 48/8 = 6 Mhz */
  RCC_PLLSAIConfig(192, 7, 4);
  RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div8);

  /* Enable PLLSAI Clock */
  RCC_PLLSAICmd(ENABLE);
  /* Wait for PLLSAI activation */
  while(RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET)
  {
  }

  /* Timing configuration */
  /* Configure horizontal synchronization width */
  ltcd.LTDC_HorizontalSync = 9;
  /* Configure vertical synchronization height */
  ltcd.LTDC_VerticalSync = 1;
  /* Configure accumulated horizontal back porch */
  ltcd.LTDC_AccumulatedHBP = 29;
  /* Configure accumulated vertical back porch */
  ltcd.LTDC_AccumulatedVBP = 3;
  /* Configure accumulated active width */
  ltcd.LTDC_AccumulatedActiveW = 269;
  /* Configure accumulated active height */
  ltcd.LTDC_AccumulatedActiveH = 323;
  /* Configure total width */
  ltcd.LTDC_TotalWidth = 279;
  /* Configure total height */
  ltcd.LTDC_TotalHeigh = 327;

  LTDC_Init(&ltcd);

  //layer init
  LTDC_Layer_InitTypeDef LTDC_Layer_InitStruct;

  /* Windowing configuration */
  /* In this case all the active display area is used to display a picture then :
  Horizontal start = horizontal synchronization + Horizontal back porch = 30
  Horizontal stop = Horizontal start + window width -1 = 30 + 240 -1
  Vertical start   = vertical synchronization + vertical back porch     = 4
  Vertical stop   = Vertical start + window height -1  = 4 + 320 -1      */
  LTDC_Layer_InitStruct.LTDC_HorizontalStart = 30;
  LTDC_Layer_InitStruct.LTDC_HorizontalStop = (LCD_PIXEL_WIDTH + 30 - 1);
  LTDC_Layer_InitStruct.LTDC_VerticalStart = 4;
  LTDC_Layer_InitStruct.LTDC_VerticalStop = (LCD_PIXEL_HEIGHT + 4 - 1);

  /* Pixel Format configuration*/
  LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_RGB565;
  /* Alpha constant (255 totally opaque) */
  LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 255;
  /* Default Color configuration (configure A,R,G,B component values) */
  LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0;
  LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0;
  LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0;
  LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0;
  /* Configure blending factors */
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;

  /* the length of one line of pixels in bytes + 3 then :
  Line Lenth = Active high width x number of bytes per pixel + 3
  Active high width         = LCD_PIXEL_WIDTH
  number of bytes per pixel = 2    (pixel_format : RGB565)
  */
  LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((LCD_PIXEL_WIDTH * 2) + 3);
  /* the pitch is the increment from the start of one line of pixels to the
  start of the next line in bytes, then :
  Pitch = Active high width x number of bytes per pixel */
  LTDC_Layer_InitStruct.LTDC_CFBPitch = (LCD_PIXEL_WIDTH * 2);

  /* Configure the number of lines */
  LTDC_Layer_InitStruct.LTDC_CFBLineNumber = LCD_PIXEL_HEIGHT;

  /* Start Address configuration : the LCD Frame buffer is defined on SDRAM */
  LTDC_Layer_InitStruct.LTDC_CFBStartAdress = LCD_FRAME_BUFFER;

  /* Initialize LTDC layer 1 */
  LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);

  /* Configure Layer2 */
  /* Start Address configuration : the LCD Frame buffer is defined on SDRAM w/ Offset */
  LTDC_Layer_InitStruct.LTDC_CFBStartAdress = LCD_FRAME_BUFFER + BUFFER_OFFSET;

  /* Configure blending factors */
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_PAxCA;
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;

  /* Initialize LTDC layer 2 */
  LTDC_LayerInit(LTDC_Layer2, &LTDC_Layer_InitStruct);

  /* LTDC configuration reload */
  LTDC_ReloadConfig(LTDC_IMReload);

  /* Enable foreground & background Layers */
  LTDC_LayerCmd(LTDC_Layer1, ENABLE);
  LTDC_LayerCmd(LTDC_Layer2, ENABLE);

  /* LTDC configuration reload */
  LTDC_ReloadConfig(LTDC_IMReload);

  /* Set default font */
  //LCD_SetFont(&LCD_DEFAULT_FONT);

  /* dithering activation */
  LTDC_DitherCmd(ENABLE);


  LTDC_Cmd(ENABLE);

//  LCD_SetFont(&Font8x8);
//  LCD_SetLayer(LCD_BACKGROUND_LAYER);
//    LCD_Clear(ASSEMBLE_RGB(0x00, 0x66, 0x00));
//  LCD_SetLayer(LCD_FOREGROUND_LAYER);
//  LCD_SetBackColor(ASSEMBLE_RGB(0x00, 0x00, 0xFF));
}
