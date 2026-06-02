/*
 * MIT License
 * Copyright (c) 2026 Sebastian Toepfer
 */
#include "board/i2c_peripheral.h"
#include "i2c_sercom.h"

const i2c_config_t i2c_eccx08 = {
    .platform_info =
        &(I2CSercomInfo){
            .sercom = SERCOM2,
            .peripheral_mask = PM_APBCMASK_SERCOM2,
            .core_clock_id = SERCOM2_GCLK_ID_CORE,
            .slow_clock_id = SERCOM2_GCLK_ID_SLOW,
            .sda_group = &PORT->Group[0],
            .sda_pin_index = 8,
            .sda_pin_mux = PORT_PMUX_PMUXE_D_Val,
            .scl_group = &PORT->Group[0],
            .scl_pin_index = 9,
            .scl_pin_mux = PORT_PMUX_PMUXE_D_Val,
            .baudrate = 100000UL,
            .rise_time_nanoseconds = 125UL,
            .timeout = 100000UL,
        },
};
