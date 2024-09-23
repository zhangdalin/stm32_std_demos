#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Buzzer.h"
#include "LightSensor.h"

int main(void)
{
	Buzzer_Init();
	LightSensor_Init();
	
	while (1)
	{
		if (LightSensor_Get() == 1)
		{
			Buzzer_ON();
		}
		else
		{
			Buzzer_OFF();
		}
		
//		Buzzer_ON();
//		Delay_ms(500);
//		Buzzer_OFF();
//		Delay_ms(500);
//		Buzzer_Turn();
//		Delay_ms(500);
//		Buzzer_Turn();
//		Delay_ms(500);
	}
}
