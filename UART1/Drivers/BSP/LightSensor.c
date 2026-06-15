#include "adc.h"					//ADC3
#include "LightSensor.h"

uint8_t Lig = 0;
uint16_t lig_adc_buffer[ADC_LIG_SIZE] = {0};  /*!< 存储从DMA接收的ADC原始值 */
												
void Lig_init(void)
{
	HAL_ADC_Start_DMA(&hadc3, (uint32_t*)lig_adc_buffer, ADC_LIG_SIZE);
}

uint8_t Lig_Get_Val(void)
{
	uint32_t light =  (uint32_t)lig_adc_buffer[0];			/*!< 获取ADC原始值 */
	
	light /= 40;																				/*光照强度 = 读取值 / 参考系数*/

    if (light > 100)light = 100; 
    return (uint8_t)(100-light);          						/*值越小，关照强度越小*/
}

