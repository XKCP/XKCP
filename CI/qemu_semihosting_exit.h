#pragma GCC push_options
#pragma GCC optimize ("O0")
// Function injected to exit Qemu semi-hosting mode
// on Cortex-M MCUs
static inline void _exit_qemu(void) {
    register unsigned int r0 __asm__("r0");
    r0 = 0x18;
    register unsigned int r1 __asm__("r1");
    r1 = 0x20026;
    __asm__ volatile("bkpt #0xAB");
}
#pragma GCC pop_options
