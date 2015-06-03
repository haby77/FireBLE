#include "uart.h"
#include "system.h"
#include "mpu_6050.h"
void uart0_init(void)
{

	//Initialize uart0 with 115200 baudrate, 8bit data, 1 stop bit, no parity, LSB bitorder, no HW flow control.
	uart_init(QN_UART0, __USART_CLK, UART_115200);
	uart_tx_enable(QN_UART0, MASK_ENABLE);
	uart_rx_enable(QN_UART0, MASK_ENABLE);	

}

#define MPU6050_INT_PIN			GPIO_P31
uint8_t mpu_6050_int_flag = 0;
void gpio_callback(enum gpio_pin pin)
{
		if(pin == MPU6050_INT_PIN)
		{
			mpu_6050_int_flag = 1;
		}
}

void mpu6050_int_init(void)
{
	
	gpio_init(gpio_callback);
	gpio_pull_set(MPU6050_INT_PIN, GPIO_PULL_UP);
	gpio_set_direction(MPU6050_INT_PIN, GPIO_INPUT);
	gpio_set_interrupt(MPU6050_INT_PIN, GPIO_INT_FALLING_EDGE);	
	gpio_enable_interrupt(MPU6050_INT_PIN);	
}

/**
 * @brief mpu6050 excample
 */
int main (void)
{
		int16_t buf[6]; 
    SystemInit();

		//UART io configurate
    uart0_init();
		//MPU-6050 initialization
		MPU_Init();
		//MPU-6050 INT pin configurate
		mpu6050_int_init();

		printf("------Hello MPU6050!!!------\r\n");

    while (1) 
    {
			//如果data raady 中断已产生，采集了新数据
			if(mpu_6050_int_flag == 1)
			{
				mpu_6050_int_flag = 0;
				//读取加速度值
				MPU_Get_Accelerometer(&buf[0],&buf[1],&buf[2]);
				//读取陀螺仪值
				MPU_Get_Gyroscope(&buf[3],&buf[4],&buf[5]);
				//打印采集的值
				printf("ax:%d ay:%d az:%d gx:%d gy:%d gz:%d \r\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
			}
    }
}

