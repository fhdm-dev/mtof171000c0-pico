/*
 * Copyright (c) 2021 fhdm-dev
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include <stdio.h>

//
#include "fhdm-mtof171000c0-pico.h"

const uint LED_PIN = 25;

int main()
{

    sleep_ms(100);
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);

    stdio_init_all();
    printf("example-simple\n");

    sleep_ms(1000);
    gpio_put(LED_PIN, 0);

    struct fhdm_mtof171000c0 mtof;
    fhdm_mtof171000c0_pico_new(&mtof, uart1, 8, 9);

    while (1)
    {
        int distance;
        mtof.get_distance(&mtof, &distance);
        printf("Distance=%d\n", distance);
        sleep_ms(1 * 1000);
    }
}
