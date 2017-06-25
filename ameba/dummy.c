/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Chester Tseng
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*****************************************************************************
 *                              Header includes
 * ***************************************************************************/
#include "py/runtime.h"
#include "section_config.h"
#include "FreeRTOS.h"
#include  "task.h"
#include "lwipopts.h"
#include "lwip/mem.h"

/*****************************************************************************
 *                              Internal variables
 * ***************************************************************************/

/*****************************************************************************
 *                              External variables
 * ***************************************************************************/
/*
 * MicroPython heap size
 */
SECTION(".sdram.bss") uint8_t mpHeap[MP_HEAP_SIZE];      // MicroPython core' heap 

/*
 * MicroPython networking core stack
 */
SECTION(".sdram.bss") StackType_t mpNetworkCoreStack[MICROPY_NETWORK_CORE_STACK_DEPTH];

struct mem {
  /** index (-> ram[next]) of the next struct */
  mem_size_t next;
  /** index (-> ram[prev]) of the previous struct */
  mem_size_t prev;
  /** 1: this area is used; 0: this area is unused */
  u8_t used;
};

#define MIN_SIZE_ALIGNED     LWIP_MEM_ALIGN_SIZE(MIN_SIZE)
#define SIZEOF_STRUCT_MEM    LWIP_MEM_ALIGN_SIZE(sizeof(struct mem))
#define MEM_SIZE_ALIGNED     LWIP_MEM_ALIGN_SIZE(MEM_SIZE)

SECTION(".sdram.bss") unsigned char lwip_ram_heap[MEM_SIZE_ALIGNED + (2*SIZEOF_STRUCT_MEM) + MEM_ALIGNMENT];
