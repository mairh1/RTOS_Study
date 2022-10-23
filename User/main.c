#include "stm32f4xx.h"
#include "../Peripheral/Usart/usart1.h"
#include "../Peripheral/Led/led.h"

/* RTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

static void prvSetupHardware(void);

/* 函数声明 */
static int sum = 0;
static TaskHandle_t xHandleTask1;
static QueueHandle_t xQueueCalcHandle;
static QueueHandle_t xQueueUARTHandle;
/*-----------------------------------------------------------------------------------------*/
 /**********************
 * RTOS任务函数 *
 **********************/
 
int InitUARTLock(void)
{
	int val;
	xQueueUARTHandle = xQueueCreate(1,sizeof(int));
	if(xQueueUARTHandle==NULL)
	{
		printf("can not create xQueueUARTHandle\r\n");
		return -1;
	}
	xQueueSend(xQueueUARTHandle,&val,portMAX_DELAY);
	return 0;
}

void GetUARTLock(void)
{
	int val;
	xQueueReceive(xQueueUARTHandle,&val,portMAX_DELAY);
}

void PutUARTLock(void)
{
	int val;
	xQueueSend(xQueueUARTHandle,&val,portMAX_DELAY);
}

void TaskFuntion1(void * param)
{
	volatile int i = 0;
	while(1)
	{
		for(i = 0; i < 10000000; i++)
			sum++;
		xQueueSend(xQueueCalcHandle,&sum,portMAX_DELAY);
		sum = 1;

	}
}

void TaskFuntion2(void * param)
{
	int val;
	
	while(1)
	{
		LED_OFF(GPIO_Pin_0);
		xQueueReceive(xQueueCalcHandle ,&val, portMAX_DELAY);
		LED_ON(GPIO_Pin_0);
		printf("sum = %d\r\n",val);
	}
}

void TaskGenericFunctino(void * param)
{
	while(1)
	{
		GetUARTLock();
		printf("%s\r\n",(char *) param);
		PutUARTLock();
		vTaskDelay(1);
	}
}
/*-----------------------------------------------------------------------------------------*/

/**********************
 * RTOS主函数 *
 **********************/
int main(void)
{	
	/* RTOS硬件初始化 */
	prvSetupHardware();
	
	/* RTOS队列创建 */
	xQueueCalcHandle = xQueueCreate(2,sizeof(int));
	if(xQueueCalcHandle==NULL)
	{
		printf("can not create xQueueCalcHandle\r\n");
	}
	InitUARTLock();
	
	/* RTOS任务 */
	xTaskCreate(TaskFuntion1,"Task1",100,NULL,1,&xHandleTask1);
	xTaskCreate(TaskFuntion2,"Task2",100,NULL,1,NULL);
	
	xTaskCreate(TaskGenericFunctino,"Task3",100,"Task 3 is Running",1,NULL);
	xTaskCreate(TaskGenericFunctino,"Task4",100,"Task 4 is Running",1,NULL);
	
	/* 启用调度器 */
	vTaskStartScheduler();
	
	/* 如果程序运行到了这里就表示出错了, 一般是内存不足 */
	return 0;
}

/*-----------------------------------------------------------------------------------------*/

/**********************
 * RTOS硬件初始化函数 *
 **********************/
static void prvSetupHardware(void)
{
	/*开启所有GPIO时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD
													|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG,ENABLE);
	/*初始化RTOS中断*/
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	/*初始化串口*/
	USART_Config();
	/*初始化led*/
	LED1_Config();
}
/*-----------------------------------------------------------------------------------------*/
