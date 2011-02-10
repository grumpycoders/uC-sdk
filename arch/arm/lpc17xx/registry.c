#include <stdint.h>
#include <stdlib.h>

extern const uint8_t __registry_space_1, __registry_space_1_end;
extern const uint8_t __registry_space_2, __registry_space_2_end;
extern const uint8_t __registry_space_3, __registry_space_3_end;

__attribute__((section(".crp"),weak)) uint32_t crp = 0xffffffff;
__attribute__((section(".som"))) uint32_t __som = 0xffffffff;

__attribute__((section(".hwregistry"),weak)) uint8_t mac_address[6] = { 0, 0, 0, 0, 0, 0 };

__attribute__((section(".hwregistry"))) const uint8_t * __registry_spaces[8] = {
    &__registry_space_1, &__registry_space_1_end,
    &__registry_space_2, &__registry_space_2_end,
    &__registry_space_3, &__registry_space_3_end,
    NULL, NULL,
};
