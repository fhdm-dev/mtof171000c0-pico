/*
 * Copyright (c) 2021 fhdm-dev
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>

//
#include "hardware/uart.h"
#include "pico/stdlib.h"

//
#include "fhdm-mtof171000c0-pico.h"

static void pico_platform_init(struct fhdm_mtof171000c0 *mtof)
{
    DEBUG_PRINT("pico_platform_init()\n");

    // Initialise the selected UART and gpio
    uart_init(mtof->_uart, 9600);
    uart_set_format(mtof->_uart, 8, 1, UART_PARITY_NONE);
    gpio_set_function(mtof->_tx_gpio, GPIO_FUNC_UART);
    gpio_set_function(mtof->_rx_gpio, GPIO_FUNC_UART);

    DEBUG_PRINT("  done\n");
}

static void pico_platform_send_serial(struct fhdm_mtof171000c0 *mtof, uint8_t data) {
    DEBUG_PRINT("    pico_platform_send_serial(): data=0x%X\n", (unsigned)data);
    uart_write_blocking(mtof->_uart, &data, 1);
}

static int pico_platform_read_serial(struct fhdm_mtof171000c0 *mtof) {
    DEBUG_PRINT("    pico_platform_read_serial()\n");

    if (! uart_is_readable_within_us(mtof->_uart, 2000000)) {
        DEBUG_PRINT("      timeout\n");
        return -1;
    }

    uint8_t data;
    uart_read_blocking(mtof->_uart, &data, 1);
    DEBUG_PRINT("      data=0x%X\n", (unsigned)data);

    return (int)data;
}

void fhdm_mtof171000c0_pico_new(struct fhdm_mtof171000c0 *mtof171000c0, uart_inst_t *uart, uint tx, uint rx)
{
    DEBUG_PRINT("fhdm_mtof171000c0_pico_new()\n");
    mtof171000c0->_platform_init = pico_platform_init;
    mtof171000c0->_send_serial = pico_platform_send_serial;
    mtof171000c0->_read_serial = pico_platform_read_serial;

    mtof171000c0->_uart = uart;
    mtof171000c0->_tx_gpio = tx;
    mtof171000c0->_rx_gpio = rx;

    _fhdm_mtof171000c0_new(mtof171000c0);
}
