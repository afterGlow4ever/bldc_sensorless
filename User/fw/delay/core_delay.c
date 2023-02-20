#include "core_delay.h"

static uint32_t g_fac_us = 0;       /* us��ʱ������ */
extern uint32_t SystemCoreClock;

void delay_init(void)
{
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);/* SYSTICKʹ���ں�ʱ��Դ,ͬCPUͬƵ�� */
    g_fac_us = SystemCoreClock / 10000000;                                  /* �����Ƿ�ʹ��OS,g_fac_us����Ҫʹ�� */
}

/**
 * @brief       ��ʱnus
 * @param       nus: Ҫ��ʱ��us��.
 * @note        ע��: nus��ֵ,��Ҫ����34952us(���ֵ��2^24 / g_fac_us @g_fac_us = 168)
 * @retval      ��
 */
void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;        /* LOAD��ֵ */
    ticks = nus * g_fac_us;                 /* ��Ҫ�Ľ����� */
    told = SysTick->VAL;                    /* �ս���ʱ�ļ�����ֵ */
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;        /* ����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ����� */
            }
            else 
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;                      /* ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳� */
            }
        }
    }
}

/**
 * @brief       ��ʱnms
 * @param       nms: Ҫ��ʱ��ms�� (0< nms <= 65535)
 * @retval      ��
 */
void delay_ms(uint16_t nms)
{
    uint32_t repeat = nms / 10;  
    uint32_t remain = nms % 10;

    while (repeat)
    {
        delay_us(10 * 1000);      
        repeat--;
    }

    if (remain)
    {
        delay_us(remain * 1000);  
    }
}

void HAL_Delay(uint32_t Delay)
{
     delay_ms(Delay);
}
