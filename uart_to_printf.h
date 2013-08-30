//******************************************************************************
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_TO_PRINF_H
#define __UART_TO_PRINF_H

#include "stm32f4xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// int fputc(int ch, FILE *f);
// int fgetc(FILE *f);
// void _ttywrch(int ch);
// void _sys_exit(int return_code);

void UART_Configuration(void);

#endif /* __UART_TO_PRINF_H */
//******************************************************************************
