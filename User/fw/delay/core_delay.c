#include "core_delay.h"

static uint32_t g_fac_us = 0;       /* us延时倍乘数 */
extern uint32_t SystemCoreClock;

void delay_init(void)
{
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);/* SYSTICK使用内核时钟源,同CPU同频率 */
    g_fac_us = SystemCoreClock / 10000000;                                  /* 不论是否使用OS,g_fac_us都需要使用 */
}

/**
 * @brief       延时nus
 * @param       nus: 要延时的us数.
 * @note        注意: nus的值,不要大于34952us(最大值即2^24 / g_fac_us @g_fac_us = 168)
 * @retval      无
 */
void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;        /* LOAD的值 */
    ticks = nus * g_fac_us;                 /* 需要的节拍数 */
    told = SysTick->VAL;                    /* 刚进入时的计数器值 */
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;        /* 这里注意一下SYSTICK是一个递减的计数器就可以了 */
            }
            else 
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;                      /* 时间超过/等于要延迟的时间,则退出 */
            }
        }
    }
}

/**
 * @brief       延时nms
 * @param       nms: 要延时的ms数 (0< nms <= 65535)
 * @retval      无
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
