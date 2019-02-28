/*
    Kernel-PMU
    Enabling user-mode access to the performance monitor unit (PMU) on ARMv8 Aarch64 and ARMv7
    Copyright (C) 2019 Bruno Pairault

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* Inspired with
        https://community.arm.com/dev-platforms/f/discussions/10366/help-configuring-pmu-s
        https://patchwork.kernel.org/patch/5217341/
*/

/* Enable user-mode ARM performance counter access on ARMv7 & Armv8 Aarch64 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/smp.h>


#if !defined(__arm__) && !defined(__aarch64__)
#error Module can only be compiled on ARM.
#endif

/* #define from /lib/modules/uname-r/source/arch/arm64/include/asm/perf_event.h in ASM Aarch 64 */
#define ARMV8_PMCR_MASK         0x3f
#define ARMV8_PMCR_E            (1 << 0) /*  Enable all counters */
#define ARMV8_PMCR_P            (1 << 1) /*  Reset all counters */
#define ARMV8_PMCR_C            (1 << 2) /*  Cycle counter reset */
#define ARMV8_PMCR_N_MASK       0x1f

#define ARMV8_PMUSERENR_EN_EL0  (1 << 0) /*  EL0 access enable */
#define ARMV8_PMUSERENR_CR      (1 << 2) /*  Cycle counter read enable */
#define ARMV8_PMUSERENR_ER      (1 << 3) /*  Event counter read enable */

#define ARMV8_PMCNTENSET_EL0_ENABLE (1<<31) /* *< Enable Perf count reg */

#define PERF_DEF_OPTS (1 | 16)

static inline u32 armv8pmu_read(void)
{
        u64 val=0;
        asm volatile("MRS %0, pmcr_el0" : "=r" (val));
        return (u32)val;
}
static inline void armv8pmu_write(u32 val)
{
        val &= ARMV8_PMCR_MASK;
        asm volatile("isb" : : : "memory");
        asm volatile("MSR pmcr_el0, %0" : : "r" ((u64)val));
}

static void
enable_cpu_counters(void* data)
{
    printk(KERN_INFO "ENABLE_ARM_PMU enabling user PMU access on CPU-Core #%d", smp_processor_id());
#if __aarch64__
        /* Enable user-mode access to counters. */
        asm volatile("MSR pmuserenr_el0, %0" : : "r"((u64)ARMV8_PMUSERENR_EN_EL0|ARMV8_PMUSERENR_ER|ARMV8_PMUSERENR_CR));
        /* Initialize & Reset PMNC: C and P bits. */
        armv8pmu_write(ARMV8_PMCR_P | ARMV8_PMCR_C);
        asm volatile("MSR pmintenset_el1, %0" : : "r" ((u64)(0 << 31)));
        /* Count Enable Set register bit  31 enable */
        asm volatile("MSR pmcntenset_el0, %0" : : "r" (ARMV8_PMCNTENSET_EL0_ENABLE));
        armv8pmu_write(armv8pmu_read() | ARMV8_PMCR_E);

#elif defined(__ARM_ARCH_7A__)

        /* Enable user-mode access to counters. */
        asm volatile("MCR p15, 0, %0, c9, c14, 0" :: "r"(1));
        /* Program PMU and enable all counters */
        asm volatile("MCR p15, 0, %0, c9, c12, 0" :: "r"(PERF_DEF_OPTS));
        asm volatile("MCR p15, 0, %0, c9, c12, 1" :: "r"(0x8000000f));
#else
#error Module Does Not Support your ARM
#endif
}

static void
disable_cpu_counters(void* data)
{
        printk(KERN_INFO "ENABLE_ARM_PMU disabling user PMU access on CPU-Core #%d", smp_processor_id());

#if __aarch64__
        /*  Performance Monitors Count Enable Set register bit 31:0 disable */
        asm volatile("MSR pmcntenset_el0, %0" : : "r" (0<<31));
        /*  Disable all counters and user-mode access to counters. */
        armv8pmu_write(armv8pmu_read() |~ ARMV8_PMCR_E);
        asm volatile("MSR pmuserenr_el0, %0" : : "r"((u64)0));
#elif defined(__ARM_ARCH_7A__)
        asm volatile("MCR p15, 0, %0, c9, c12, 0" :: "r"(0));
        /*  Disable all counters and user-mode access to counters. */
        asm volatile("MCR p15, 0, %0, c9, c12, 2" :: "r"(0x8000000f));
        asm volatile("MCR p15, 0, %0, c9, c14, 0" :: "r"(0));
#else
#error Module Does Not Support your ARM
#endif
}

static int __init
init(void)
{
        on_each_cpu(enable_cpu_counters, NULL, 1);
        printk(KERN_INFO "ENABLE_ARM_PMU Initialized");
        return 0;
}

static void __exit
leave(void)
{
        on_each_cpu(disable_cpu_counters, NULL, 1);
        printk(KERN_INFO "ENABLE_ARM_PMU Unloaded");
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Enables user-mode access to ARMv7A-v8 Aarch64 PMU counters");
MODULE_VERSION("1:0.0-dev");
module_init(init);
module_exit(leave);
