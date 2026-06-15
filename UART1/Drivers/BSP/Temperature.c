#include "adc.h"		//ADC1
#include "Temperature.h"								
											
float Tem = 0;
uint16_t g_adc_buffer[ADC_BUF_SIZE] = {0};  /*!< 存储从DMA接收的ADC原始值 */
												
void Tem_init(void)
{
	
		HAL_ADC_Start_DMA(&hadc1, (uint32_t*)g_adc_buffer, ADC_BUF_SIZE);
	
}

float Tem_Get_Val(void)
{
	float temperature;												
	
		temperature = ((float)g_adc_buffer[0]) *3.3f /4096.0f; 	/*!< 转换成ADC电压，加.0f消除警告 */
		temperature = (temperature - 0.76f) /0.0025f +25.0f;		/*!< 将ADC原始值转换成摄氏度 */
	
	return temperature;
}

