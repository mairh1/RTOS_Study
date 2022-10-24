#include "stm32f4xx.h"
#include "../Peripheral/Usart/usart1.h"
#include "../Peripheral/Led/led.h"

/* RTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

static void prvSetupHardware(void);

/* 函数声明 */

static TaskHandle_t xHandleTask1;
static TaskHandle_t xHandleTask2;
static TaskHandle_t xHandleTask3;

static QueueHandle_t xQueueHandle1;
static QueueHandle_t xQueueHandle2;
static QueueSetHandle_t xQueueSet;
/*-----------------------------------------------------------------------------------------*/
 /**********************
 * RTOS任务函数 *
 **********************/
 
void TaskFuntion1(void * param)
{
	int i = 0;
	while(1)
	{
		xQueueSend(xQueueHandle1,&i,portMAX_DELAY);
		i++;
		vTaskDelay(10);
	}
}

void TaskFuntion2(void * param)
{
	int i = -1;
	while(1)
	{
		xQueueSend(xQueueHandle2,&i,portMAX_DELAY);
		i--;
		vTaskDelay(20);
	}
}
void TaskFuntion3(void * param)
{
	int i;
	QueueSetMemberHandle_t handle;
	while(1)
	{
		handle=xQueueSelectFromSet(xQueueSet,portMAX_DELAY);
		xQueueReceive(handle,&i,0);
		printf("i = %d\n\r",i);
		
	}
}

void TaskGenericFunctino(void * param)
{
	while(1)
	{
		printf("%s\r\n",(char *) param);
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
	
	/* RTOS队列 */
	xQueueHandle1 = xQueueCreate(2,sizeof(int));
	if(xQueueHandle1==NULL)
	{
		printf("can not create xQueueHandle1\r\n");
	}
	xQueueHandle2 = xQueueCreate(2,sizeof(int));
	if(xQueueHandle2==NULL)
	{
		printf("can not create xQueueHandle2\r\n");
	}
	/* RTOS队列集 */
	xQueueSet = xQueueCreateSet(4);
	
	/* 添加进队列集 */
	xQueueAddToSet(xQueueHandle1,xQueueSet);
	xQueueAddToSet(xQueueHandle2,xQueueSet);
	
	/* RTOS任务 */
	xTaskCreate(TaskFuntion1,"Task1",100,NULL,1,&xHandleTask1);
	xTaskCreate(TaskFuntion2,"Task2",100,NULL,1,&xHandleTask2);
	xTaskCreate(TaskFuntion3,"Task3",100,NULL,1,&xHandleTask3);
	
//	xTaskCreate(TaskGenericFunctino,"Task3",100,"Task 3 is Running",1,NULL);
//	xTaskCreate(TaskGenericFunctino,"Task4",100,"Task 4 is Running",1,NULL);
	
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
