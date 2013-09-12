/*----------------------------------------------------------------------*/
#ifndef _FF_DEBUG_H
#define _FF_DEBUG_H

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "stm32f4xx.h"
#include "stm32f4_discovery_sdio_sd.h"
#include "ff.h"

//#define DEBUG

#ifdef DEBUG
#include "sdio_debug.h"
# define DEBUG_PRINT(x) printf x
#else
# define DEBUG_PRINT(x) do {} while (0)
#endif

void FR_print_error( FRESULT fresult );
FRESULT scan_files (char* path);

#endif /* _FF_DEBUG_H */
