#ifndef PIT_DEVICE
#define PIT_DEVICE

#include <stdint.h>
#include "../../../programmableInterruptController/PIC8259.h"

#define PIT_INTERRUPT_VECTOR 32
#define PIT_IRQ (PIT_INTERRUPT_VECTOR-PIC1_OFFSET)

#define CHANNEL0_DATA_PORT 0x40
#define CHANNEL1_DATA_PORT 0x41
#define CHANNEL2_DATA_PORT 0x42
#define MODE_COMMAND_REGISTER 0x43

#define PIT_CMD_CHANNEL0_LH_MODE2 0x34

#define PIT_BASE_FREQUENCY 1193182u

void pit_init(uint32_t desired_hz);
uint64_t pit_get_ticks(void);
uint32_t pit_get_frequency(void);
void pit_sleep(uint64_t ms);
uint64_t pit_measure_time(void(*work)(void));

#endif