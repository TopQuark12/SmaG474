#ifndef SMAAFE
#define SMAAFE

void smaAfeInit(void);
void smaAfeLoop(void);

#define BUTTON_COMP_MASK (uint32_t)0xFF0000FF
#define BUTTON_COMP_PRES (uint32_t)0x000000FF
#define BUTTON_COMP_RELS (uint32_t)0xFF000000

typedef struct button_t
{
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin;
    uint32_t history;
    // osMutexId mutexID;
} button_t;

#endif  // SMAAFE