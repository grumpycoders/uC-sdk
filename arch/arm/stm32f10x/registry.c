#include <stdint.h>
#include <stdlib.h>

extern const uint8_t __registry_space_1, __registry_space_1_end;
extern const uint8_t __registry_space_2, __registry_space_2_end;
extern const uint8_t __registry_space_3, __registry_space_3_end;

__attribute__((section(".hwregistry"),weak)) uint8_t mac_address[6] = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15 };

__attribute__((section(".hwregistry"))) const uint8_t * __registry_spaces[8] = {
    &__registry_space_1, &__registry_space_1_end,
    &__registry_space_2, &__registry_space_2_end,
    &__registry_space_3, &__registry_space_3_end,
    NULL, NULL,
};
