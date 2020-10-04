/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: Temperature & Humidity sensor
     File Name		: SENSOR_SHT20.c
     Function		: SENSOR_SHT20
     Create Date	: 2017/07/18
---------------------------------------------------------------------- */

#include <stdio.h>
#include <math.h>
#include <delay.h>
#include <alcd.h>
#include <i2c.h>
#include <stdlib.h>
#include <datatype_Layer.h>
#include <swi2c_Layer.h>
#include <SENSOR_SHT20.h>



void  EXAMPLE_SHT20(void);


void  EXAMPLE_SHT20(void)
{
	CHAR8S status = 0;
	FLOAT temperature =0.0,humidity = 0.0;
	CHAR8U mode = 0,battery_status = 0;
  	INT32U lcd_char_data[2][5]={0}; 
	INT32S data;

	
		printf("-------------------- Temperature & Humidity sensor  SHT20 --------------------\r\n");

		/* Temperature & Humidity sensor  HTU21D */
				
		/* RESET SHT20 */
		status = SHT20_RESET();
		if(status==0)
		{ 
			printf(" RESET SHT20 success!\r\n");
		}
		else 
		{
			printf(" RESET SHT20 fail!\r\n");
		}

		delay_ms(40);	/* after RESET SHT20 -> wait 15mS */

	
		/* disable Heat IC function */
		/* mode 0 : Humidity -> 12bit  ; Temperature -> 14bit */
		status = SHT20_INIT();
		if(status==0) printf(" INITIAL SHT20 success!\r\n");
		else printf(" INITIAL SHT20 fail!\r\n");


		/* check battery status */
		status = SHT20_CHECK_BATTERY_STATUS(&battery_status);
		if(status==0)
		{		
			if(battery_status==0x01)printf(" SHT20 Battery VDD<2.25v \r\n");
			if(battery_status==0x00)printf(" SHT20 Battery VDD>2.25v \r\n");
		}
		else 		
		{
			printf(" SHT20 read battery status fail!\r\n");
		}


		/*  check mode  */
		status = SHT20_READ_MODE(&mode);
		if(status ==0)
		{
			if(mode | SHT20_MODE0) printf("SHT20 is mode 0, 0x%x\r\n",mode);
			else printf("SHT20 is NOT mode , 0x%x\r\n",mode);
		}
		else 
		{
			printf(" SHT20 read mode fail!\r\n");
		}
			

		while(1)
		{
			/* read Temperature & Check CRC checksum  & Calculate final temperature */
			status = SHT20_READ_TEMPERATURE(&temperature,mode);
			 if(status!=0) printf(" read SHT20 temperature data fail %d\r\n",status);

			/* wait 10mS */
			delay_ms(10);
			 
			/* read Humidity & Check CRC checksum  & Calculate final Humidity */	
			status = SHT20_READ_HUMIDITY(&humidity,mode);
			 if(status!=0) printf(" read SHT20 humidity data fail\r\n");

			/* print result */		
			printf("SHT20 T:%f C	H:%f %RH\r\n",temperature,humidity);	



			/* --------- Temperature bolck --------- */
			if(temperature<0)/* temperature < 0 */
			{				
				/* Temperautre */
				lcd_char_data[0][0] = (INT32U)(abs(temperature/100))%10;		/* use abs() */
				lcd_char_data[0][1] = (INT32U)(abs(temperature/10))%10;		/* use abs() */
				lcd_char_data[0][2] = (INT32U)(abs(temperature))%10;			/* use abs() */  
				lcd_char_data[0][3] = (INT32U)(abs(temperature*10))%10;		/* use abs() */   
				lcd_char_data[0][4] = (INT32U)(abs(temperature*100))%10; 		/* use abs() */  		
			}
			else /* temperature >= 0 */
			{
				/* Temperautre */
				lcd_char_data[0][0] = (INT32U)(temperature/100)%10;     
				lcd_char_data[0][1] = (INT32U)(temperature/10)%10;
				lcd_char_data[0][2] = (INT32U)(temperature)%10;  
				lcd_char_data[0][3] = (INT32U)(temperature*10)%10;   
				lcd_char_data[0][4] = (INT32U)(temperature*100)%10;   						
			}
				
			/* SHOW Temperautre */                    
	            	lcd_gotoxy(0,0);
	            	lcd_putsf("T:");     
	            	if(temperature<0)
	            	{
	            		lcd_putchar(45);		/* LCD show "-"  mean negative */
			}                             
	           	else
			{
				lcd_putchar(32);		/* LCD show " "  mean positive */
			}   
					
			/* show Temperautre data on LCD */
			lcd_putchar(48+lcd_char_data[0][0]);
			lcd_putchar(48+lcd_char_data[0][1]);
			lcd_putchar(48+lcd_char_data[0][2]);
			lcd_putchar(46);    /* print "."  */					
			lcd_putchar(48+lcd_char_data[0][3]);
			lcd_putchar(48+lcd_char_data[0][4]);
			lcd_putsf("C");
			/* --------- Temperature bolck --------- */

			
			/* --------- Humidity bolck --------- */
			/* Humidity */
			lcd_char_data[0][0] = (INT32U)(humidity/100)%10;     
			lcd_char_data[0][1] = (INT32U)(humidity/10)%10;
			lcd_char_data[0][2] = (INT32U)(humidity)%10;  
			lcd_char_data[0][3] = (INT32U)(humidity*10)%10;   
			lcd_char_data[0][4] = (INT32U)(humidity*100)%10;   	                   
				
			/* SHOW Humidity */                    
	            	lcd_gotoxy(0,1);
	            	lcd_putsf("H:");     

			/* show Humidity data on LCD */
			lcd_putchar(48+lcd_char_data[0][0]);
			lcd_putchar(48+lcd_char_data[0][1]);
			lcd_putchar(48+lcd_char_data[0][2]);
			lcd_putchar(46);    /* print "."  */					
			lcd_putchar(48+lcd_char_data[0][3]);
			lcd_putchar(48+lcd_char_data[0][4]);
			lcd_putsf("%RH");
			/* --------- Humidity bolck --------- */


			/* --------- Display ID bolck --------- */
			/* SHOW IC ID */                    
	            	lcd_gotoxy(0,2);
	            	lcd_putsf("SHT20");    
			/* --------- Display ID bolck --------- */	
				
			/* Delay */
			delay_ms(200);
				
		}		

		printf("-------------------- Temperature & Humidity sensor  SHT20 --------------------\r\n");
	  
}

