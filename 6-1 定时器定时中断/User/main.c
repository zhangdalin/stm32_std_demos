#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"

uint16_t Num;

int main(void)
{
	OLED_Init();
	Timer_Init();
	
	OLED_ShowString(1, 1, "Num:");
	
	while (1)
	{
		OLED_ShowNum(1, 5, Num, 5);
	}
}

/*
* TIM2的中断处理函数，TIM2_IRQHandler, 在中断处理函数中建议使用:
* TIM_GetITStatus
* TIM_ClearITPendingBit
* 而不使用：
* TIM_GetFlagStatus
* TIM_ClearFlag
*/
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) // 检查中断是否是TIM2上产生的更新中断
	{
		Num ++;
		// 清除中断标志位
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
