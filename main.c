/*
 * main.c
 */

#include "msp430f5529.h"
#include "HAL_Dogs102x6.h"
#include "config.h"

void Delay400ms(void)
{
    unsigned char i=500;
	unsigned int j;

	while(i--)
	{
		j=7269;
		while(j--);
	}
}
void Delay(unsigned char i){
	while(i--)
	{
		Delay400ms();
	}
}
void main(void) {
	uint8_t contrast;            //��ȡFLASH�жԱȶ�ֵ
	uint8_t brightness;        //��ȡFLASH�б���ֵ
	WDTCTL = WDTPW + WDTHOLD;
    Dogs102x6_init();                            //��ʼ��LCD
    Dogs102x6_backlightInit();                   //�����ʼ��

    // Contrast not programed in Flash Yet
    if (contrast == 0xFF)                        //����ǰFLASH���޶Աȶ�ֵ���򽫶Աȶ�ֵ��Ϊ11��Ĭ�ϣ�
        // Set Default Contrast
        contrast = 11;

    // Brightness not programed in Flash Yet
    if (brightness == 0xFF)                      //����ǰFLASH���ޱ���ֵ���򽫱���ֵ��Ϊ11��Ĭ�ϣ�
        // Set Default Brightness
        brightness = 11;

    Dogs102x6_setBacklight(brightness);          //���ó�ʼ����ֵ
    Dogs102x6_setContrast(contrast);             //���ó�ʼ�Աȶ�ֵ
    Dogs102x6_clearScreen();                     //����
    //Dogs102x6_imageDraw(tiBug, 0, 16);           //��ʾTIͼ��
    Dogs102x6_stringDraw(3, 0, "   Welcome to    ", DOGS102x6_DRAW_NORMAL);
    Delay(10);

}
