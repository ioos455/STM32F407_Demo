#ifndef __TEMPERATURE_H
#define __TEMPERATURE_H

#define ADC_BUF_SIZE    1   /*!< ADC DMA缓冲区大小,此处为1 */

extern uint16_t g_adc_buffer[ADC_BUF_SIZE];
extern float Tem;									//温度

void Tem_init(void);
float Tem_Get_Val(void);


#endif
