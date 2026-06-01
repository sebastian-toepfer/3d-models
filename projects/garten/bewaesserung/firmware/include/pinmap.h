#ifndef PINMAP_H
#define PINMAP_H

#include "digital_output_pin.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const digital_pin_config_t Bewaesserungsrelais_pin_config;
extern const digital_pin_config_t Hauptrelais_pin_config;
extern const digital_pin_config_t Poolrelais_pin_config;
extern const digital_pin_config_t Poolvollrelais_pin_config;

#ifdef __cplusplus
}
#endif

#endif
