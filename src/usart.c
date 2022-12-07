#include "usart.h"

#define USART_BLUETOOTH_SPEED 115200

static uint16_t compute_bd(uint32_t periph_clk, uint32_t baud_rate){
    return ((periph_clk + (baud_rate/2U))/baud_rate);
}

void initialize_usart(void){

    USART_DeInit(USART2);

    RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    RCC -> APB1ENR |= RCC_APB1ENR_USART2EN;

    GPIOA -> MODER &=~GPIO_MODER_MODER2_0;
    GPIOA -> MODER |= GPIO_MODER_MODER2_1;

    GPIOA -> MODER &=~GPIO_MODER_MODER3_0;
    GPIOA -> MODER |= GPIO_MODER_MODER3_1;

    GPIOA -> AFR[0] |= (GPIO_AF_USART2 << 8);
    GPIOA -> AFR[0] |= (GPIO_AF_USART2 << 12);

    RCC -> APB1ENR |= RCC_APB1ENR_USART2EN;

    uint16_t baud_rate = compute_bd(HSI_VALUE, USART_BLUETOOTH_SPEED);

    USART2 -> BRR = baud_rate;

    USART2 -> CR3 |= USART_CR3_DMAT;
    USART2 -> CR3 |= USART_CR3_DMAR;

    USART2 -> CR1 |= (USART_CR1_TE | USART_CR1_RE | USART_CR1_UE);
}

void initialize_dma(uint32_t dst, uint32_t len){
    DMA_DeInit(DMA1_Stream5);
    DMA_DeInit(DMA1_Stream6);

    RCC -> AHB1ENR |= RCC_AHB1ENR_DMA1EN;

    DMA1_Stream5 -> CR &=~ DMA_SxCR_EN;
    while(DMA1_Stream5 -> CR & DMA_SxCR_EN){}

    DMA1_Stream6 -> CR &=~ DMA_SxCR_EN;
    while(DMA1_Stream6 -> CR & DMA_SxCR_EN){}

    DMA1 -> HIFCR |= (DMA_HIFCR_CFEIF6 | DMA_HIFCR_CDMEIF6 | DMA_HIFCR_CTEIF6 | DMA_HIFCR_CHTIF6 | DMA_HIFCR_CTCIF6);
    DMA1 -> HIFCR |= (DMA_HIFCR_CFEIF5 | DMA_HIFCR_CDMEIF5 | DMA_HIFCR_CTEIF5 | DMA_HIFCR_CHTIF5 | DMA_HIFCR_CTCIF5);

    DMA1_Stream5 -> PAR |= (uint32_t)&USART2 -> DR;
    DMA1_Stream6 -> PAR |= (uint32_t)&USART2 -> DR;

    DMA1_Stream5 -> M0AR = dst;
    DMA1_Stream5 -> NDTR = len;

    DMA1_Stream5 -> CR |= DMA_SxCR_CHSEL_2;
    DMA1_Stream6 -> CR |= DMA_SxCR_CHSEL_2;

    DMA1_Stream5 -> CR |= DMA_SxCR_CIRC;
    DMA1_Stream6 -> CR |= DMA_SxCR_DIR_0;

    DMA1_Stream5 -> CR |= DMA_SxCR_MINC;
    DMA1_Stream6 -> CR |= DMA_SxCR_MINC;

    DMA1_Stream5 -> CR |= DMA_SxCR_TCIE;
    DMA1_Stream6 -> CR |= DMA_SxCR_TCIE;

    DMA1_Stream5 -> FCR = 0;
    DMA1_Stream6 -> FCR = 0;

    DMA1_Stream5 -> CR |= DMA_SxCR_EN;

    NVIC_EnableIRQ(DMA1_Stream5_IRQn);
    NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}

void initialize_usart_bt(uint32_t dst, uint32_t len){

    initialize_usart();

    initialize_dma(dst, len);
}

void usart_bt_write(uint32_t src, uint32_t dtlen){
    
    DMA1_Stream6 -> M0AR = src;

    DMA1_Stream6 -> NDTR = dtlen;

    DMA1_Stream6 -> CR |= DMA_SxCR_EN;
}