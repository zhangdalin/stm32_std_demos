#include "stm32f10x.h"                  // Device header

uint16_t CountSensor_Count;

void CountSensor_Init(void)
{
	// 1. 开启GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	// 2. 开启AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	// 3. 初始化GPIO pin 14
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	// 上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;		// 14号引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// 4. 选择GPIOB pin 14 作为中断线，实际操作的是AFIO寄存器
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	
	// 5. 初始化EXTI
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;	// 选择 EXTI 14号线
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	// 中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	// 下降沿触发
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	// 2位抢占优先级，2位响应优先级，配置后全局有效
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	// 抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	// 响应优先级
	NVIC_Init(&NVIC_InitStructure);
}

uint16_t CountSensor_Get(void)
{
	return CountSensor_Count;
}

/*
 * 中断函数都是无参无返回值的函数，此处实现中断处理函数。
 * 在STM32中中断函数都是固定的，已经定义在startup_stm32f10x_md.s中断向量表中文件中
 * 该函数不需要再次声明
*/
void EXTI15_10_IRQHandler(void)
{
	// 中断函数内部处理，判断中断是否为14号线触发的中断，因为 EXTI15_10_IRQHandler 可以接收 10 ~ 15 号线的所有中断
	if (EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0)
		{
			CountSensor_Count ++;
		}
		// 退出中断函数时需要手动清理中断标志位，否则会一直触发中断
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}
