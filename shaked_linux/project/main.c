
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "bsp_uart.h"
#include "bsp_lcd.h"
#include "bsp_lcdapi.h"
#include "bsp_rtc.h"
#include "bsp_i2c.h"
#include "stdio.h"
#include "bsp_ap3216c.h"
#include "bsp_spi.h"
#include "bsp_icm20608.h"
void integer_display(unsigned short x, unsigned short y, unsigned char size, signed int num)
{
	char buf[200];
	
	lcd_fill(x, y, x + 50, y + size, tftlcd_dev.backcolor);
	
	memset(buf, 0, sizeof(buf));
	if(num < 0)
		sprintf(buf, "-%d", -num);
	else 
		sprintf(buf, "%d", num);
	lcd_show_string(x, y, 50, size, size, buf); 
}



void decimals_display(unsigned short x, unsigned short y, unsigned char size, signed int num)
{
	signed int integ; 	
	signed int fract;	
	signed int uncomptemp = num; 
	char buf[200];

	if(num < 0)
		uncomptemp = -uncomptemp;
	integ = uncomptemp / 100;
	fract = uncomptemp % 100;

	memset(buf, 0, sizeof(buf));
	if(num < 0)
		sprintf(buf, "-%d.%d", integ, fract);
	else 
		sprintf(buf, "%d.%d", integ, fract);
	lcd_fill(x, y, x + 60, y + size, tftlcd_dev.backcolor);
	lcd_show_string(x, y, 60, size, size, buf); 
}


 void imx6ul_hardfpu_enable(void)
{
	uint32_t cpacr;
	uint32_t fpexc;

	
	cpacr = __get_CPACR();
	cpacr = (cpacr & ~(CPACR_ASEDIS_Msk | CPACR_D32DIS_Msk))
		   |  (3UL << CPACR_cp10_Pos) | (3UL << CPACR_cp11_Pos);
	__set_CPACR(cpacr);
	fpexc = __get_FPEXC();
	fpexc |= 0x40000000UL;	
	__set_FPEXC(fpexc);
}
/*
 * @description	: main函数
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{
	unsigned char key = 0;
	
	char buf[160];
	struct rtc_datetime rtcdate;
	unsigned char state = OFF;
	unsigned short ir, als, ps;
	imx6ul_hardfpu_enable();
	int_init(); 				/* 初始化中断(一定要最先调用！) */
	imx6u_clkinit();			/* 初始化系统时钟 			*/
	delay_init();				/* 初始化延时 			*/
	clk_enable();				/* 使能所有的时钟 			*/
	led_init();					/* 初始化led 			*/
	beep_init();				/* 初始化beep	 		*/
	uart_init();				/* 初始化串口，波特率115200 */
	lcd_init();					/* 初始化LCD 			*/
	rtc_init(); 				/* 初始化RTC	 		*/
	tftlcd_dev.forecolor = LCD_RED;
	lcd_show_string(50, 10, 400, 24, 24, (char*)"ALPHA-IMX6UL RTC TEST");    /* 显示字符串 */
	lcd_show_string(50, 40, 200, 16, 16, (char*)"ATOM@ALIENTEK");  
	lcd_show_string(50, 60, 200, 16, 16, (char*)"2023/3/16");  
	tftlcd_dev.forecolor = LCD_BLUE;
	while(icm20608_init())
	{
		lcd_show_string(50, 220, 200, 16, 16, (char*)"ICM20608 Check Failed!");
		delayms(500);
		lcd_show_string(50, 240, 200, 16, 16, (char*)"Please Check!        ");
		delayms(500);
	}
	lcd_show_string(300, 100, 200, 16, 16, (char*)"ICM20608 Ready");
	
	lcd_show_string(300, 130, 200, 16, 16, (char*)"accel x:");  
	lcd_show_string(300, 150, 200, 16, 16, (char*)"accel y:");  
	lcd_show_string(300, 170, 200, 16, 16, (char*)"accel z:");  
	lcd_show_string(300, 190, 200, 16, 16, (char*)"gyro  x:"); 
	lcd_show_string(300, 210, 200, 16, 16, (char*)"gyro  y:"); 
	lcd_show_string(300, 230, 200, 16, 16, (char*)"gyro  z:"); 
	lcd_show_string(300, 250, 200, 16, 16, (char*)"temp   :"); 

	lcd_show_string(300 + 181, 130, 200, 16, 16, (char*)"g");  
	lcd_show_string(300 + 181, 150, 200, 16, 16, (char*)"g");  
	lcd_show_string(300 + 181, 170, 200, 16, 16, (char*)"g");  
	lcd_show_string(300+ 181, 190, 200, 16, 16, (char*)"o/s"); 
	lcd_show_string(300+ 181, 210, 200, 16, 16, (char*)"o/s"); 
	lcd_show_string(300+ 181, 230, 200, 16, 16, (char*)"o/s"); 
	lcd_show_string(300+ 181, 250, 200, 16, 16, (char*)"C");
	
	tftlcd_dev.forecolor = LCD_BLUE;
	while(ap3216c_init())
	{

        lcd_show_string(50, 70, 200, 16, 16, (char*)"check device");
		delay(500);
        lcd_show_string(50, 80, 200, 16, 16, (char*)"failed");
	}
	//memset(buf, 0, sizeof(buf));
	lcd_show_string(50, 130, 200, 16, 16, (char*)"AP3216C Ready!"); 
	lcd_show_string(50, 160, 200, 16, 16, (char*)" IR:"); 
	lcd_show_string(50, 180, 200, 16, 16, (char*)" PS:"); 
	lcd_show_string(50, 200, 200, 16, 16, (char*)"ALS:");
			rtcdate.year = 2018;
   			rtcdate.month = 1;
    		rtcdate.day = 15;
    		rtcdate.hour = 16;
    		rtcdate.minute = 23;
    		rtcdate.second = 0;
			rtc_setdatetime(&rtcdate); /* 初始化时间和日期 */
			printf("\r\n RTC Init finish\r\n");
			rtc_getdatetime(&rtcdate);

	while(1)					
	{	
		rtc_getdatetime(&rtcdate);
		sprintf(buf,"%d/%d/%d %d:%d:%d",rtcdate.year, rtcdate.month, rtcdate.day, rtcdate.hour, rtcdate.minute, rtcdate.second);
		lcd_fill(50,110, 300,130, tftlcd_dev.backcolor);
		lcd_show_string(50, 110, 250, 16, 16,(char*)buf);  /* 显示字符串 */
		
		ap3216c_readdata(&ir, &ps, &als);
		lcd_shownum(50 + 32, 160, ir, 5, 16); /* 显示 IR 数据 */
		lcd_shownum(50 + 32, 180, ps, 5, 16); /* 显示 PS 数据 */
		lcd_shownum(50 + 32, 200, als, 5, 16); /* 显示 ALS 数据 */

		icm20608_getdata();
		integer_display(300 + 70, 130, 16, icm20608_dev.accel_x_adc);
		integer_display(300 + 70, 150, 16, icm20608_dev.accel_y_adc);
		integer_display(300+ 70, 170, 16, icm20608_dev.accel_z_adc);
		integer_display(300+ 70, 190, 16, icm20608_dev.gyro_x_adc);
		integer_display(300+ 70, 210, 16, icm20608_dev.gyro_y_adc);
		integer_display(300+ 70, 230, 16, icm20608_dev.gyro_z_adc);
		integer_display(300+ 70, 250, 16, icm20608_dev.temp_adc);

		decimals_display(300 + 70 + 50, 130, 16, icm20608_dev.accel_x_act);
		decimals_display(300+ 70 + 50, 150, 16, icm20608_dev.accel_y_act);
		decimals_display(300+ 70 + 50, 170, 16, icm20608_dev.accel_z_act);
		decimals_display(300+ 70 + 50, 190, 16, icm20608_dev.gyro_x_act);
		decimals_display(300+ 70 + 50, 210, 16, icm20608_dev.gyro_y_act);
		decimals_display(300+ 70 + 50, 230, 16, icm20608_dev.gyro_z_act);
		decimals_display(300+ 70 + 50, 250, 16, icm20608_dev.temp_act);

		state = !state;
		led_switch(LED0,state);
		delayms(1000);	/* 延时一秒 */
	}
	return 0;
}
