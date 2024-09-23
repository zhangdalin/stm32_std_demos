#include "stm32f10x.h"                  // Device header

void IC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	// 上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM3);	// 设置TIM3采用内部时钟 默认 72M
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR	CNT的量程范围，最多计数65535个数据
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;		//PSC 标准频率等于1M Hz (72M/72)
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	// 输入捕获部分初始化
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;	// 直连通道输入
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	// 选择触发源
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
	// 设置从模式(捕获后自动重置CNT为0)
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
	
	TIM_Cmd(TIM3, ENABLE);
}

uint32_t IC_GetFreq(void)
{
	return 1000000 / (TIM_GetCapture1(TIM3) + 1);	// 获取目标频率的值 fc/N (测周法)
}
