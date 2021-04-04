/*
 * Copyright (c) 2021 fhdm-dev
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __FHDM_MTOF171000C0__
#define __FHDM_MTOF171000C0__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdint.h>

//
#include "hardware/uart.h"

#ifndef NDEBUG
#define DEBUG_PRINT(fmt, args...) printf(fmt, ##args)
#else
#define DEBUG_PRINT(fmt, args...)
#endif

struct fhdm_mtof171000c0 {

    // Public functions
    bool (*get_distance)(struct fhdm_mtof171000c0 *mtof171000c0, int *distance);

    //
    // Private functions and variables. Should not be accessed directly by users of the library
    //
    bool _is_started;

    uart_inst_t *_uart;
    uint _tx_gpio;
    uint _rx_gpio;

    void (*_platform_init)(struct fhdm_mtof171000c0 *mtof);
    void (*_send_serial)(struct fhdm_mtof171000c0 *mtof, uint8_t data);
    int (*_read_serial)(struct fhdm_mtof171000c0 *mtof);
};

//
// Private functions. Should not be called by users of the library.
//

void _fhdm_mtof171000c0_new(struct fhdm_mtof171000c0* mtof171000c0);

#endif // __FHDM_MTOF171000C0__
