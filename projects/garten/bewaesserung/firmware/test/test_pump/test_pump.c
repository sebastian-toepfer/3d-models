#include <unity.h>
#include "pump.h"
#include "digital_output_pin.h"
#include "digital_output_pin_test.h"

static digital_pin_event_log_t events;

void setUp()
{
    digital_output_pin_test_event_log_reset(&events);
}

void tearDown()
{
}

void should_create_all_pins()
{
    struct Pump *pump = pump_create(
        &(digital_pin_config_t) {
            .pin = 1,
            .platform_info = &(digital_pin_info_test_t) {
                .pin_index = 1,
                .event_log = &events
            }
        },
        &(lockable_valve_t){
            .relay = &(digital_pin_config_t) {
                .pin = 2,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 2,
                    .event_log = &events
                }
            },
        },
        &(lockable_valve_t){
            .relay = &(digital_pin_config_t) {
                .pin = 3,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 3,
                    .event_log = &events
                }
            },
            .lock_relay = &(digital_pin_config_t) {
                .pin = 4,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 4,
                    .event_log = &events
                }
            },
        },
        &(timeout_config_t){
            .on_delay = 1000,
            .off_delay = 500,
        }
    );

    TEST_ASSERT_EQUAL(4, digital_output_pin_test_count_events(&events, GPIO_TEST_EVENT_CREATE));
    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=1, .type = GPIO_TEST_EVENT_CREATE}));
    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=2, .type = GPIO_TEST_EVENT_CREATE}));
    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=3, .type = GPIO_TEST_EVENT_CREATE}));
    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=4, .type = GPIO_TEST_EVENT_CREATE}));

    pump_destroy(pump);
}

void should_destroy_all_pins()
{
    struct Pump *pump = pump_create(
        &(digital_pin_config_t) {
            .pin = 1,
            .platform_info = &(digital_pin_info_test_t) {
                .pin_index = 1,
                .event_log = &events
            }
        },
        &(lockable_valve_t){
            .relay = &(digital_pin_config_t) {
                .pin = 2,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 2,
                    .event_log = &events
                }
            },
        },
        &(lockable_valve_t){
            .relay = &(digital_pin_config_t) {
                .pin = 3,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 3,
                    .event_log = &events
                }
            },
            .lock_relay = &(digital_pin_config_t) {
                .pin = 4,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 4,
                    .event_log = &events
                }
            },
        },
        &(timeout_config_t){
            .on_delay = 1000,
            .off_delay = 500,
        }
    );
    pump_destroy(pump);

    TEST_ASSERT_EQUAL(4, digital_output_pin_test_count_events(&events, GPIO_TEST_EVENT_DESTROY));
    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=1, .type = GPIO_TEST_EVENT_DESTROY}));
    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=2, .type = GPIO_TEST_EVENT_DESTROY}));
    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=3, .type = GPIO_TEST_EVENT_DESTROY}));
    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=4, .type = GPIO_TEST_EVENT_DESTROY}));
}

void should_open_garden_valve()
{
    struct Pump *pump = pump_create(
        &(digital_pin_config_t) {
            .pin = 1,
            .platform_info = &(digital_pin_info_test_t) {
                .pin_index = 1,
                .event_log = &events
            }
        },
        &(lockable_valve_t){
            .relay = &(digital_pin_config_t) {
                .pin = 2,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 2,
                    .event_log = &events
                }
            },
        },
        &(lockable_valve_t){
            .relay = &(digital_pin_config_t) {
                .pin = 3,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 3,
                    .event_log = &events
                }
            },
            .lock_relay = &(digital_pin_config_t) {
                .pin = 4,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 4,
                    .event_log = &events
                }
            },
        },
        &(timeout_config_t){
            .on_delay = 1000,
            .off_delay = 500,
        }
    );
    digital_output_pin_test_event_log_reset(&events);

    pump_open_garden_valve(pump);

    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=2, .type = GPIO_TEST_EVENT_ON}));
    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=2, .type = GPIO_TEST_EVENT_OFF}));

    pump_destroy(pump);
}

void should_close_garden_valve()
{
    struct Pump *pump = pump_create(
        &(digital_pin_config_t) {
            .pin = 1,
            .platform_info = &(digital_pin_info_test_t) {
                .pin_index = 1,
                .event_log = &events
            }
        },
        &(lockable_valve_t){
            .relay = &(digital_pin_config_t) {
                .pin = 2,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 2,
                    .event_log = &events
                }
            },
        },
        &(lockable_valve_t){
            .relay = &(digital_pin_config_t) {
                .pin = 3,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 3,
                    .event_log = &events
                }
            },
            .lock_relay = &(digital_pin_config_t) {
                .pin = 4,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 4,
                    .event_log = &events
                }
            },
        },
        &(timeout_config_t){
            .on_delay = 1000,
            .off_delay = 500,
        }
    );
    pump_open_garden_valve(pump);
    digital_output_pin_test_event_log_reset(&events);

    pump_close_garden_valve(pump);

    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=1, .type = GPIO_TEST_EVENT_ON}));
    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=1, .type = GPIO_TEST_EVENT_OFF}));

    pump_destroy(pump);
}

void should_created_with_locked_pool_valve() {
    struct Pump *pump = pump_create(
        &(digital_pin_config_t) {
            .pin = 1,
            .platform_info = &(digital_pin_info_test_t) {
                .pin_index = 1,
                .event_log = &events
            }
        },
        &(lockable_valve_t){
            .relay = &(digital_pin_config_t) {
                .pin = 2,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 2,
                    .event_log = &events
                }
            },
        },
        &(lockable_valve_t){
            .relay = &(digital_pin_config_t) {
                .pin = 3,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 3,
                    .event_log = &events
                }
            },
            .lock_relay = &(digital_pin_config_t) {
                .pin = 4,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 4,
                    .event_log = &events
                }
            },
        },
        &(timeout_config_t){
            .on_delay = 1000,
            .off_delay = 500,
        }
    );
    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=4, .type = GPIO_TEST_EVENT_ON}));

    pump_destroy(pump);
}

void should_open_pool_valve() {
    struct Pump *pump = pump_create(
        &(digital_pin_config_t) {
            .pin = 1,
            .platform_info = &(digital_pin_info_test_t) {
                .pin_index = 1,
                .event_log = &events
            }
        },
        &(lockable_valve_t){
            .relay = &(digital_pin_config_t) {
                .pin = 2,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 2,
                    .event_log = &events
                }
            },
        },
        &(lockable_valve_t){
            .relay = &(digital_pin_config_t) {
                .pin = 3,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 3,
                    .event_log = &events
                }
            },
            .lock_relay = &(digital_pin_config_t) {
                .pin = 4,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 4,
                    .event_log = &events
                }
            },
        },
        &(timeout_config_t){
            .on_delay = 1000,
            .off_delay = 500,
        }
    );
    digital_output_pin_test_event_log_reset(&events);

    pump_open_pool_valve(pump);

    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=3, .type = GPIO_TEST_EVENT_ON}));
    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=3, .type = GPIO_TEST_EVENT_OFF}));

    pump_destroy(pump);
}

void should_close_pool_valve()
{
    struct Pump *pump = pump_create(
        &(digital_pin_config_t) {
            .pin = 1,
            .platform_info = &(digital_pin_info_test_t) {
                .pin_index = 1,
                .event_log = &events
            }
        },
        &(lockable_valve_t){
            .relay = &(digital_pin_config_t) {
                .pin = 2,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 2,
                    .event_log = &events
                }
            },
        },
        &(lockable_valve_t){
            .relay = &(digital_pin_config_t) {
                .pin = 3,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 3,
                    .event_log = &events
                }
            },
            .lock_relay = &(digital_pin_config_t) {
                .pin = 4,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 4,
                    .event_log = &events
                }
            },
        },
        &(timeout_config_t){
            .on_delay = 1000,
            .off_delay = 500,
        }
    );
    pump_open_pool_valve(pump);
    digital_output_pin_test_event_log_reset(&events);

    pump_close_pool_valve(pump);

    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=1, .type = GPIO_TEST_EVENT_ON}));
    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=1, .type = GPIO_TEST_EVENT_OFF}));

    pump_destroy(pump);
}

void should_reopen_garden_valve_if_both_are_open_and_pool_should_closed()
{
    struct Pump *pump = pump_create(
        &(digital_pin_config_t) {
            .pin = 1,
            .platform_info = &(digital_pin_info_test_t) {
                .pin_index = 1,
                .event_log = &events
            }
        },
        &(lockable_valve_t){
            .relay = &(digital_pin_config_t) {
                .pin = 2,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 2,
                    .event_log = &events
                }
            },
        },
        &(lockable_valve_t){
            .relay = &(digital_pin_config_t) {
                .pin = 3,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 3,
                    .event_log = &events
                }
            },
            .lock_relay = &(digital_pin_config_t) {
                .pin = 4,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 4,
                    .event_log = &events
                }
            },
        },
        &(timeout_config_t){
            .on_delay = 1000,
            .off_delay = 500,
        }
    );
    pump_open_pool_valve(pump);
    pump_open_garden_valve(pump);
    digital_output_pin_test_event_log_reset(&events);

    pump_close_pool_valve(pump);

    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=1, .type = GPIO_TEST_EVENT_ON}));
    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=1, .type = GPIO_TEST_EVENT_OFF}));
    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=2, .type = GPIO_TEST_EVENT_ON}));
    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=2, .type = GPIO_TEST_EVENT_OFF}));

    pump_destroy(pump);
}

void should_reopen_pool_valve_if_both_are_open_and_garden_should_closed()
{
    struct Pump *pump = pump_create(
        &(digital_pin_config_t) {
            .pin = 1,
            .platform_info = &(digital_pin_info_test_t) {
                .pin_index = 1,
                .event_log = &events
            }
        },
        &(lockable_valve_t){
            .relay = &(digital_pin_config_t) {
                .pin = 2,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 2,
                    .event_log = &events
                }
            },
        },
        &(lockable_valve_t){
            .relay = &(digital_pin_config_t) {
                .pin = 3,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 3,
                    .event_log = &events
                }
            },
            .lock_relay = &(digital_pin_config_t) {
                .pin = 4,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 4,
                    .event_log = &events
                }
            },
        },
        &(timeout_config_t){
            .on_delay = 1000,
            .off_delay = 500,
        }
    );
    pump_open_pool_valve(pump);
    pump_open_garden_valve(pump);
    digital_output_pin_test_event_log_reset(&events);

    pump_close_garden_valve(pump);

    TEST_ASSERT_EQUAL(4, events.count);
    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=1, .type = GPIO_TEST_EVENT_ON}));
    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=1, .type = GPIO_TEST_EVENT_OFF}));
    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=3, .type = GPIO_TEST_EVENT_ON}));
    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=3, .type = GPIO_TEST_EVENT_OFF}));

    pump_destroy(pump);
}

void should_lock_pool_valve()
{
    struct Pump *pump = pump_create(
        &(digital_pin_config_t) {
            .pin = 1,
            .platform_info = &(digital_pin_info_test_t) {
                .pin_index = 1,
                .event_log = &events
            }
        },
        &(lockable_valve_t){
            .relay = &(digital_pin_config_t) {
                .pin = 2,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 2,
                    .event_log = &events
                }
            },
        },
        &(lockable_valve_t){
            .relay = &(digital_pin_config_t) {
                .pin = 3,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 3,
                    .event_log = &events
                }
            },
            .lock_relay = &(digital_pin_config_t) {
                .pin = 4,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 4,
                    .event_log = &events
                }
            },
        },
        &(timeout_config_t){
            .on_delay = 1000,
            .off_delay = 500,
        }
    );
    digital_output_pin_test_event_log_reset(&events);

    pump_lock_pool_valve(pump);

    TEST_ASSERT_EQUAL(1, events.count);
    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=4, .type = GPIO_TEST_EVENT_OFF}));

    pump_destroy(pump);
}

void should_close_pool_valve_before_locking() 
{
    struct Pump *pump = pump_create(
        &(digital_pin_config_t) {
            .pin = 1,
            .platform_info = &(digital_pin_info_test_t) {
                .pin_index = 1,
                .event_log = &events
            }
        },
        &(lockable_valve_t){
            .relay = &(digital_pin_config_t) {
                .pin = 2,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 2,
                    .event_log = &events
                }
            },
        },
        &(lockable_valve_t){
            .relay = &(digital_pin_config_t) {
                .pin = 3,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 3,
                    .event_log = &events
                }
            },
            .lock_relay = &(digital_pin_config_t) {
                .pin = 4,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 4,
                    .event_log = &events
                }
            },
        },
        &(timeout_config_t){
            .on_delay = 1000,
            .off_delay = 500,
        }
    );
    pump_open_pool_valve(pump);
    digital_output_pin_test_event_log_reset(&events);

    pump_lock_pool_valve(pump);

    TEST_ASSERT_EQUAL(3, events.count);
    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=1, .type = GPIO_TEST_EVENT_ON}));
    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=1, .type = GPIO_TEST_EVENT_OFF}));
    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=4, .type = GPIO_TEST_EVENT_OFF}));

    pump_destroy(pump);
}

void should_unlock_pool_valve()
{
    struct Pump *pump = pump_create(
        &(digital_pin_config_t) {
            .pin = 1,
            .platform_info = &(digital_pin_info_test_t) {
                .pin_index = 1,
                .event_log = &events
            }
        },
        &(lockable_valve_t){
            .relay = &(digital_pin_config_t) {
                .pin = 2,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 2,
                    .event_log = &events
                }
            },
        },
        &(lockable_valve_t){
            .relay = &(digital_pin_config_t) {
                .pin = 3,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 3,
                    .event_log = &events
                }
            },
            .lock_relay = &(digital_pin_config_t) {
                .pin = 4,
                .platform_info = &(digital_pin_info_test_t) {
                    .pin_index = 4,
                    .event_log = &events
                }
            },
        },
        &(timeout_config_t){
            .on_delay = 1000,
            .off_delay = 500,
        }
    );
    pump_lock_pool_valve(pump);
    digital_output_pin_test_event_log_reset(&events);

    pump_unlock_pool_valve(pump);

    TEST_ASSERT_EQUAL(1, events.count);
    TEST_ASSERT_TRUE(digital_output_pin_test_has_event(&events, (digital_pin_event_t) {.pin_index=4, .type = GPIO_TEST_EVENT_ON}));

    pump_destroy(pump);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(should_create_all_pins);    
    RUN_TEST(should_open_garden_valve);
    RUN_TEST(should_close_garden_valve);

    RUN_TEST(should_created_with_locked_pool_valve);
    RUN_TEST(should_open_pool_valve);
    RUN_TEST(should_close_pool_valve);

    RUN_TEST(should_reopen_garden_valve_if_both_are_open_and_pool_should_closed);
    RUN_TEST(should_reopen_pool_valve_if_both_are_open_and_garden_should_closed);

    RUN_TEST(should_lock_pool_valve);
    RUN_TEST(should_close_pool_valve_before_locking);

    RUN_TEST(should_unlock_pool_valve);

    RUN_TEST(should_destroy_all_pins);

    return UNITY_END();
}