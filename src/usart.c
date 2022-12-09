#include "usart.h"

#define USART_BLUETOOTH_SPEED 9600

static uint16_t compute_bd(uint32_t periph_clk, uint32_t baud_rate){
    return ((periph_clk + (baud_rate/2U))/baud_rate);
}

void initialize_usart(void){

    USART_DeInit(USART6);

    RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    RCC -> APB2ENR |= RCC_APB2ENR_USART6EN;

    GPIOA -> MODER &=~GPIO_MODER_MODER11_0;
    GPIOA -> MODER |= GPIO_MODER_MODER11_1;

    GPIOA -> MODER &=~GPIO_MODER_MODER12_0;
    GPIOA -> MODER |= GPIO_MODER_MODER12_1;

    GPIOA -> AFR[1] |= (GPIO_AF_USART6 << 12);
    GPIOA -> AFR[1] |= (GPIO_AF_USART6 << 16);

    uint16_t baud_rate = compute_bd(HSI_VALUE, USART_BLUETOOTH_SPEED);

    USART6 -> BRR = baud_rate;

    USART6 -> CR3 |= USART_CR3_DMAT;
    USART6 -> CR3 |= USART_CR3_DMAR;

    USART6 -> CR1 |= (USART_CR1_TE | USART_CR1_RE | USART_CR1_UE);
}

void initialize_dma(uint32_t dst, uint32_t len){
    DMA_DeInit(DMA2_Stream2);
    DMA_DeInit(DMA2_Stream6);

    RCC -> AHB1ENR |= RCC_AHB1ENR_DMA2EN;

    DMA2_Stream2 -> CR &=~ DMA_SxCR_EN;
    while(DMA2_Stream2 -> CR & DMA_SxCR_EN){}

    DMA2_Stream6 -> CR &=~ DMA_SxCR_EN;
    while(DMA2_Stream6 -> CR & DMA_SxCR_EN){}

    DMA2 -> HIFCR |= (DMA_HIFCR_CFEIF6 | DMA_HIFCR_CDMEIF6 | DMA_HIFCR_CTEIF6 | DMA_HIFCR_CHTIF6 | DMA_HIFCR_CTCIF6);
    DMA2 -> HIFCR |= (DMA_LIFCR_CFEIF2 | DMA_LIFCR_CDMEIF2 | DMA_LIFCR_CTEIF2 | DMA_LIFCR_CHTIF2 | DMA_LIFCR_CTCIF2);

    DMA2_Stream2 -> PAR |= (uint32_t)&USART6 -> DR;
    DMA2_Stream6 -> PAR |= (uint32_t)&USART6 -> DR;

    DMA2_Stream2 -> M0AR = dst;
    DMA2_Stream2 -> NDTR = len;

    DMA2_Stream2 -> CR |= (DMA_SxCR_CHSEL_2 | DMA_SxCR_CHSEL_0);
    DMA2_Stream6 -> CR |= (DMA_SxCR_CHSEL_2 | DMA_SxCR_CHSEL_0);

    DMA2_Stream2 -> CR |= DMA_SxCR_CIRC;
    DMA2_Stream6 -> CR |= DMA_SxCR_DIR_0;

    DMA2_Stream2 -> CR |= DMA_SxCR_MINC;
    DMA2_Stream6 -> CR |= DMA_SxCR_MINC;

    DMA2_Stream2 -> CR |= DMA_SxCR_TCIE;
    DMA2_Stream6 -> CR |= DMA_SxCR_TCIE;

    DMA2_Stream2 -> FCR = 0;
    DMA2_Stream6 -> FCR = 0;

    DMA2_Stream2 -> CR |= DMA_SxCR_EN;

    NVIC_EnableIRQ(DMA2_Stream2_IRQn);
    NVIC_EnableIRQ(DMA2_Stream6_IRQn);
}

void initialize_usart_bt(uint32_t dst, uint32_t len){

    initialize_usart();

    initialize_dma(dst, len);
}

void usart_bt_write(uint32_t src, uint32_t dtlen){
    
    DMA2_Stream6 -> M0AR = src;

    DMA2_Stream6 -> NDTR = dtlen;

    DMA2_Stream6 -> CR |= DMA_SxCR_EN;
}