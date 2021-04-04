/*
 * Copyright (c) 2021 fhdm-dev
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

//
#include <stdio.h>

//
#include "fhdm-mtof171000c0.h"

// Starting bytes to identify UART data packet.
const uint8_t START_BYTE_1 = 0x55;
const uint8_t START_BYTE_2 = 0xAA;

// UART command codes.
#define CMD_RD_SEN_REG 0xD1 // Read the ToF sensor register
#define CMD_WR_SEN_REG 0x51 // Write the ToF sensor register
#define CMD_RD_MM 0xD3      // Measure distance
#define CMD_CALI_XTALK 0xFA // Perform crosstalk calibration
#define CMD_CALI_OFS 0xFB   // Perform offset calibration
#define CMD_WR_OFS 0x7C     // Write the offset value manually
#define CMD_WR_XTALK 0x7D   // Write the crosstalk value manually
#define CMD_RD_VAR 0xF0     // Get the module status information, for the engineering debug
#define CMD_RESET 0xF5      // Sensor reset

// Helper function to send a basic command that doesn't require command data.
void mtof171000c0_send_command(struct fhdm_mtof171000c0 *mtof, uint8_t cmd_code)
{
    const int data_length = 2;

    // Send 1st Identification Byte.
    mtof->_send_serial(mtof, START_BYTE_1);

    // Send 2nd Identification Byte.
    mtof->_send_serial(mtof, START_BYTE_2);

    // Send the Command Instruction.
    mtof->_send_serial(mtof, cmd_code);

    // Send the Command Data.
    mtof->_send_serial(mtof, 0);

    // Send the Data Length for this command in bytes.
    mtof->_send_serial(mtof, data_length);

    // Send the CheckSum (which does not include the identification bytes).
    uint32_t checksum = cmd_code + data_length;
    mtof->_send_serial(mtof, checksum & 0xFF);
}

static int mtof171000c0_read_data_packet(struct fhdm_mtof171000c0 *mtof)
{
    DEBUG_PRINT("mtof171000c0_read_data_packet()\n");

    // Look for 1st Identification Byte. A value of -1 indicates a timeout.
    int data = 999;
    while (data != -1 && data != START_BYTE_1)
    {
        data = mtof->_read_serial(mtof);
    }

    if (data != START_BYTE_1)
    {
        DEBUG_PRINT("  not start byte 1: %d\n", data);
        return -2;
    }

    // Look for 2nd Identification Byte.
    data = mtof->_read_serial(mtof);
    if (data != START_BYTE_2)
    {
        DEBUG_PRINT("  not start byte 2: %d\n", data);
        return -3;
    }

    // return the Command Code and process accordingly.
    return mtof->_read_serial(mtof);
}

static void mtof171000c0_start(struct fhdm_mtof171000c0 *mtof)
{
    mtof->_platform_init(mtof);
    mtof->_is_started = true;
}

static bool mtof171000c0_get_distance(struct fhdm_mtof171000c0 *mtof, int *distance)
{
    //DEBUG_PRINT("_fhdm_mtof171000c0_new()\n");
    if (!mtof->_is_started)
    {
        mtof171000c0_start(mtof);
    }

    mtof171000c0_send_command(mtof, CMD_RD_MM);

    int cmd = mtof171000c0_read_data_packet(mtof);
    if (cmd == CMD_RD_MM)
    {
        DEBUG_PRINT("  got CMD_RD_MM result\n");

        // We expect
        // 02
        // 00
        // high byte of distance
        // low byte of distance
        // checksum

        // Read the cmd data (unused).
        int cmd_data = mtof->_read_serial(mtof);
        if (cmd_data != 0)
            return false;

        // Read the Data Length.
        int data_length = mtof->_read_serial(mtof);
        if (data_length != 2)
            return false;

        // Read the Hi byte of the distance.
        int dist_hi = mtof->_read_serial(mtof);

        // Read the Lo byte of the distance.
        int dist_lo = mtof->_read_serial(mtof);

        // Read and verify the CheckSum.
        int checksum = mtof->_read_serial(mtof);
        unsigned int testsum = CMD_RD_MM + cmd_data + data_length + dist_hi + dist_lo;
        if (checksum != (testsum & 0xFF))
            return false;

        *distance = dist_hi * 256 + dist_lo;
        DEBUG_PRINT("Read Distance: %d\n", *distance);

        return true;
    }
    else
    {
        DEBUG_PRINT("  got unexpected result. CMD=%d (%X)\n", cmd, cmd);
        return false;
    }
}

void _fhdm_mtof171000c0_new(struct fhdm_mtof171000c0 *mtof)
{
    //DEBUG_PRINT("_fhdm_mtof171000c0_new()\n");

    mtof->get_distance = mtof171000c0_get_distance;
    mtof->_is_started = false;
}
