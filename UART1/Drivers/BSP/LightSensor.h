#ifndef __LIGHTSENSOR_H
#define __LIGHTSENSOR_H

#define ADC_LIG_SIZE    1   /*!< ADC DMA缓冲区大小,此处为1 */

extern uint16_t lig_adc_buffer[ADC_LIG_SIZE];  /*!< 存储从DMA接收的ADC原始值 */
extern uint8_t Lig;										//光照强度

void Lig_init(void);
uint8_t Lig_Get_Val(void);


#endif
