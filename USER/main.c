#include "led.h"
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "mpu6050.h"
#include "usart.h"
#include "timer.h"
#include "sendware.h"
#include "math.h"
#include "usart3.h"
#include "gizwits_product.h"
#include "key.h"

 
 short accx,accy,accz;		//加速度传感器原始数据
 short gyrox,gyroy,gyroz;	//陀螺仪原始数据
 float ax,ay,az;		//加速度传感器原始数据
 float gx,gy,gz;	//陀螺仪原始数据






/* 用户当前设备状态结构体*/
//dataPoint_t currentDataPoint;


//	wifi连接状态
//		wifi_sta 0:断开  1：已连接
u8 wifi_sta=0;

//协议初始化
 void Gizwits_Init(void)
{	
	TIM3_Int_Init(9,7199);//1ms系统定时
    usart3_init(9600);//wifi初始
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//设备结构体初始
	gizwitsInit();//缓冲区初始
}


//void userHandle(void)
//{
	//	MPU_Get_Accelerometer(&accx,&accy,&accz);	
  //  currentDataPoint.valueD001_ACCA_X = accx ;//Add Sensor Data Collection

   
//}



void userHandle(void)

{

		MPU_Get_Accelerometer(&accx,&accy,&accz);
	  ax=-accx/1657;
		ay=-accy/1657;
    currentDataPoint.valueACCA_X = ax ;//Add Sensor Data Collection
    currentDataPoint.valueACCA_Y = ay ;
   
}






 int main(void)
 {	

	 
	int key;
	u8 wifi_con=0;//记录wifi连接状态 1：连接0：断开
	 
	 
 	u16 t;  
	u16 len;	
	u16 times=0;
	delay_init();	    	 //延时函数初始化	  

	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化
	KEY_Init();  
	 
	//TIM3_Int_Init(19,7199);//10Khz的计数频率，计数到50为5ms  
	I2C_Configuration();
	MPU_Init();
	
	MPU_Get_Accelerometer(&accx,&accy,&accz);	//得到加速度传感器数据
	MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
	
	
	
	
	
	
  Gizwits_Init(); 
	
	LED0=1;
	
	
 	while(1)
	{
		
		MPU_Get_Accelerometer(&accx,&accy,&accz);
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
		ax=-accx;
		ay=-accy;
		az=-accz;
		gx=gyrox;
		gy=gyroy;
		gz=gyroz;
	  printf("//ax=%f//ay=%f//az=%f//gx=%f//gy=%f//gz=%f\r\n",ax,ay,az,gx,gy,gz);
		
		delay_ms(1000);
		
		
		
		
		
		if(wifi_con!=wifi_sta)
		{
			wifi_con=wifi_sta;
			wifi_con?printf("connect"):printf("close");	
		}
		 
	
		userHandle();//用户时间过程
		
		gizwitsHandle((dataPoint_t *)&currentDataPoint);//协议处理
		
		key=KEY_Scan(0);
		
		if(key==!0)
			LED0=!LED0;
		if(key==KEY0_PRES)//KEY0按键
		{
			LED0=!LED0;
			printf("WIFI进入AirLink连接模式\r\n");
			gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-link模式接入
		}			
		if(key==WKUP_PRES)//KEY_UP按键
		{  
			LED1=!LED1;
			printf("WIFI复位，请重新配置连接\r\n");
			gizwitsSetMode(WIFI_RESET_MODE);//WIFI复位 
			wifi_sta=0;//标志wifi已断开
		}
		delay_ms(250);	
		
		
		
		
		
		
  
		}
	}	 



 

