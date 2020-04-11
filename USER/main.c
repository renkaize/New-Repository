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

 
 short accx,accy,accz;		//���ٶȴ�����ԭʼ����
 short gyrox,gyroy,gyroz;	//������ԭʼ����
 float ax,ay,az;		//���ٶȴ�����ԭʼ����
 float gx,gy,gz;	//������ԭʼ����






/* �û���ǰ�豸״̬�ṹ��*/
//dataPoint_t currentDataPoint;


//	wifi����״̬
//		wifi_sta 0:�Ͽ�  1��������
u8 wifi_sta=0;

//Э���ʼ��
 void Gizwits_Init(void)
{	
	TIM3_Int_Init(9,7199);//1msϵͳ��ʱ
    usart3_init(9600);//wifi��ʼ
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//�豸�ṹ���ʼ
	gizwitsInit();//��������ʼ
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
	u8 wifi_con=0;//��¼wifi����״̬ 1������0���Ͽ�
	 
	 
 	u16 t;  
	u16 len;	
	u16 times=0;
	delay_init();	    	 //��ʱ������ʼ��	  

	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();  
	 
	//TIM3_Int_Init(19,7199);//10Khz�ļ���Ƶ�ʣ�������50Ϊ5ms  
	I2C_Configuration();
	MPU_Init();
	
	MPU_Get_Accelerometer(&accx,&accy,&accz);	//�õ����ٶȴ���������
	MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
	
	
	
	
	
	
  Gizwits_Init(); 
	
	LED0=1;
	
	
 	while(1)
	{
		
		MPU_Get_Accelerometer(&accx,&accy,&accz);
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
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
		 
	
		userHandle();//�û�ʱ�����
		
		gizwitsHandle((dataPoint_t *)&currentDataPoint);//Э�鴦��
		
		key=KEY_Scan(0);
		
		if(key==!0)
			LED0=!LED0;
		if(key==KEY0_PRES)//KEY0����
		{
			LED0=!LED0;
			printf("WIFI����AirLink����ģʽ\r\n");
			gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-linkģʽ����
		}			
		if(key==WKUP_PRES)//KEY_UP����
		{  
			LED1=!LED1;
			printf("WIFI��λ����������������\r\n");
			gizwitsSetMode(WIFI_RESET_MODE);//WIFI��λ 
			wifi_sta=0;//��־wifi�ѶϿ�
		}
		delay_ms(250);	
		
		
		
		
		
		
  
		}
	}	 



 

