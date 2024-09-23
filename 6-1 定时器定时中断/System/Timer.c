#include "stm32f10x.h"                  // Device header

void Timer_Init(void)
{
	// 1. 开启定时器TIM2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// 2. 选择内部时钟源到定时器TIM2
	TIM_InternalClockConfig(TIM2);
	
	// 3. 初始化时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// 不分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	// 计数模式，向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;	// 自动重装器ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;	// 预分频器PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;	// 重复计数器的值
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	// 4. 清除TIM2的更新中断标志位（解决在初始化后紧接着触发的更新中断事件，避免产生一次中断）
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	// 5. 定时器中断配置（更新中断）
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);	// 定时器2中断使能,并设置为更新中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	// NVIC优先级分组，全局作用域
	
	// 6. 初始化NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	// 中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	// 抢占优先级值
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	// 响应优先级值
	NVIC_Init(&NVIC_InitStructure);
	
	// 7. 定时器使能
	TIM_Cmd(TIM2, ENABLE);
}

/*
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
*/
