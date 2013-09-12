//******************************************************************************
#include <stdio.h>
#include <inttypes.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "discoveryf4utils.h"
#include "uart_to_printf.h"
#include "stm32f4_discovery_sdio_sd.h"
#include "sdio_debug.h"
#include "diskio.h"
#include "ff.h"
#include "ff_debug.h"
#include "stm32f4xx_rng.h"
//******************************************************************************

/**
  * @brief  RNG configuration
  * @param  None
  * @retval None
  *    Get a 32bit Random number     
  *    random32bit = RNG_GetRandomNumber();
  */
void RNG_Config(void)
{  
 /* Enable RNG clock source */
  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);

  /* RNG Peripheral enable */
  RNG_Cmd(ENABLE);
}

//******************************************************************************
int main(void)
{
	/*!< At this stage the microcontroller clock setting is already configured,
	   this is done through SystemInit() function which is called from startup
	   file (startup_stm32f4xx.s) before to branch to application main.
	   To reconfigure the default setting of SystemInit() function, refer to
	   system_stm32f4xx.c file
	 */
 	uint32_t 	i = 0;
	uint32_t 	random32bit;
 	FATFS   	fs;
	uint8_t 	Buff[24]= {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0};
	FIL     	file;
	FRESULT		fresult;
// 	FILINFO		filinfo;
// 	DIR			dir;
	TCHAR		buffstr[100];
	TCHAR		filename[16];
	
	STM_EVAL_LEDInit(LED_BLUE);
	STM_EVAL_LEDInit(LED_GREEN);
	STM_EVAL_LEDInit(LED_ORANGE);
	STM_EVAL_LEDInit(LED_RED);
	
	UART_Configuration();
	RNG_Config();
	#ifdef SD_DMA_MODE
	SD_NVIC_Configuration();
	#endif
	
	//SD_test();
	/* Wait until one RNG number is ready */
	while(RNG_GetFlagStatus(RNG_FLAG_DRDY)== RESET){};
	random32bit = RNG_GetRandomNumber();
	
  	printf("f_mount:\n");
  	fresult = f_mount(0, &fs);
  	FR_print_error(fresult);
	
	//=========================================================================
	printf("f_open:");
	sprintf(filename,"0:%X.txt",random32bit);
	printf("%s\n",filename);
	fresult = f_open(&file, filename, FA_OPEN_ALWAYS | FA_WRITE);
	FR_print_error(fresult);
	
	printf("f_printf:");
 	i = f_printf(&file,"Hello world!!! On SD card!\n");
	printf(" i=%i",i);
 	i = f_printf(&file,"End string.");
	printf(" i=%i",i);
	printf("\n");
	printf("\0");
	
	printf("f_close:\n");
	fresult = f_close(&file);
	FR_print_error(fresult);
	//=========================================================================
	
	//=========================================================================
	printf("f_open: %s\n",filename);
	fresult = f_open(&file, filename, FA_OPEN_EXISTING | FA_READ);
	FR_print_error(fresult);
	
 	printf("f_gets:\n");
 	f_gets(buffstr,100,&file);
 	printf("%s\n",buffstr);

	f_lseek(&file, 0);
 	printf("f_read:\n");
 	fresult = f_read(&file,Buff,sizeof(Buff),0);
	FR_print_error(fresult);
	for(i=0;i<sizeof(Buff);++i)
	{
		printf("%02X ",Buff[i]);
	}
	
	printf("f_close:\n");
	fresult = f_close(&file);
	FR_print_error(fresult);
	//=========================================================================

	//=========================================================================
	printf("f_open:\n");
	fresult = f_open(&file, "0:qwe.txt", FA_OPEN_EXISTING | FA_READ);
	FR_print_error(fresult);
	
 	printf("f_gets:\n");
 	f_gets(buffstr,100,&file);
 	printf("%s\n",buffstr);

	f_lseek(&file, 0);
 	printf("f_read:\n");
 	f_read(&file,Buff,sizeof(Buff),0);
	for(i=0;i<sizeof(Buff);++i)
	{
		printf("%02X ",Buff[i]);
	}

 	printf("f_close:\n");
 	fresult = f_close(&file);
 	FR_print_error(fresult);
	//=========================================================================


   	printf("Scan files:\n");
    fresult = scan_files("0:");
 	FR_print_error(fresult);
	
	f_mount(0, NULL);
	
	
	while(1) /* Infinite loop */
	{
		//STM_EVAL_LEDToggle(LED_BLUE);
		//STM_EVAL_LEDToggle(LED_GREEN);
		STM_EVAL_LEDToggle(LED_ORANGE);
		STM_EVAL_LEDToggle(LED_RED);
		for(i=0;i<0x01FFFFFF;++i){__NOP();};
		//printf("k = %i\n",k++);
	}
}
//******************************************************************************

