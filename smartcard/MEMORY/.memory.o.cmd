cmd_/mnt/hgfs/Myproject/smartcard/MEMORY/memory.o := /home/hky/arm/gcc-4.2.0-glibc-2.5_g++/arm-unknown-linux-gnu/bin/arm-unknown-linux-gnu-gcc -Wp,-MD,/mnt/hgfs/Myproject/smartcard/MEMORY/.memory.o.d  -nostdinc -isystem /home/hky/arm/gcc-4.2.0-glibc-2.5_g++/arm-unknown-linux-gnu/bin/../lib/gcc/arm-unknown-linux-gnu/4.2.0/include -Iinclude  -I/usr/src/linux-2.6.32-env/arch/arm/include -include include/linux/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-bcm5892/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -Os -marm -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mabi=apcs-gnu -mno-thumb-interwork -D__LINUX_ARM_ARCH__=6 -march=armv6k -mtune=arm1136j-s -msoft-float -Uarm -fno-stack-protector -fno-omit-frame-pointer -fno-optimize-sibling-calls -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -I /mnt/hgfs/Myproject/smartcard/BOARD -I /mnt/hgfs/Myproject/smartcard/IC -I /mnt/hgfs/Myproject/smartcard/MEMORY -I /mnt/hgfs/Myproject/smartcard/SAM -I /mnt/hgfs/Myproject/smartcard/SC  -DMODULE -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(memory)"  -D"KBUILD_MODNAME=KBUILD_STR(sc_tda8035)"  -c -o /mnt/hgfs/Myproject/smartcard/MEMORY/memory.o /mnt/hgfs/Myproject/smartcard/MEMORY/memory.c

deps_/mnt/hgfs/Myproject/smartcard/MEMORY/memory.o := \
  /mnt/hgfs/Myproject/smartcard/MEMORY/memory.c \
    $(wildcard include/config/sp60.h) \
  include/linux/kernel.h \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  /home/hky/arm/gcc-4.2.0-glibc-2.5_g++/arm-unknown-linux-gnu/bin/../lib/gcc/arm-unknown-linux-gnu/4.2.0/include/stdarg.h \
  include/linux/linkage.h \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/linkage.h \
  include/linux/stddef.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/types.h \
  include/asm-generic/int-ll64.h \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/linux/posix_types.h \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/posix_types.h \
  include/linux/bitops.h \
    $(wildcard include/config/generic/find/first/bit.h) \
    $(wildcard include/config/generic/find/last/bit.h) \
    $(wildcard include/config/generic/find/next/bit.h) \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/bitops.h \
    $(wildcard include/config/smp.h) \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/system.h \
    $(wildcard include/config/cpu/xsc3.h) \
    $(wildcard include/config/cpu/fa526.h) \
    $(wildcard include/config/cpu/sa1100.h) \
    $(wildcard include/config/cpu/sa110.h) \
    $(wildcard include/config/cpu/32v6k.h) \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
    $(wildcard include/config/x86.h) \
  include/linux/typecheck.h \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/irqflags.h \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/ptrace.h \
    $(wildcard include/config/cpu/endian/be8.h) \
    $(wildcard include/config/arm/thumb.h) \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/hwcap.h \
  include/asm-generic/cmpxchg-local.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/sched.h \
  include/asm-generic/bitops/hweight.h \
  include/asm-generic/bitops/lock.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/ratelimit.h \
  include/linux/param.h \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/param.h \
    $(wildcard include/config/hz.h) \
  include/linux/dynamic_debug.h \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/linux/swab.h \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/swab.h \
  include/linux/byteorder/generic.h \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/io.h \
    $(wildcard include/config/mmu.h) \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/memory.h \
    $(wildcard include/config/page/offset.h) \
    $(wildcard include/config/thumb2/kernel.h) \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/dram/size.h) \
    $(wildcard include/config/dram/base.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/discontigmem.h) \
  include/linux/const.h \
  arch/arm/mach-bcm5892/include/mach/memory.h \
    $(wildcard include/config/bcm/ram/base.h) \
  /usr/src/linux-2.6.32-env/arch/arm/include/cfg_global.h \
    $(wildcard include/config/support.h) \
  include/linux/broadcom/cfg_global_defines.h \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/sizes.h \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/sparsemem.h) \
  arch/arm/mach-bcm5892/include/mach/io.h \
  include/linux/ioport.h \
  arch/arm/mach-bcm5892/include/mach/reg_gpio.h \
  arch/arm/mach-bcm5892/include/mach/hardware.h \
  arch/arm/mach-bcm5892/include/mach/platform.h \
  arch/arm/mach-bcm5892/include/mach/bcm5892_reg.h \
    $(wildcard include/config/memaddr.h) \
    $(wildcard include/config/sel.h) \
    $(wildcard include/config/size.h) \
    $(wildcard include/config/mask.h) \
    $(wildcard include/config/init.h) \
    $(wildcard include/config/rwmode.h) \
    $(wildcard include/config/data/coherency/mode/sel.h) \
    $(wildcard include/config/data/coherency/mode/sel/r.h) \
    $(wildcard include/config/data/coherency/mode/sel/l.h) \
    $(wildcard include/config/data/coherency/mode/sel/size.h) \
    $(wildcard include/config/data/coherency/mode/sel/mask.h) \
    $(wildcard include/config/data/coherency/mode/sel/range.h) \
    $(wildcard include/config/data/coherency/mode/sel/init.h) \
    $(wildcard include/config/data/coherency/mode/sel/rwmode.h) \
    $(wildcard include/config/data/coherency/mode/sel/get.h) \
    $(wildcard include/config/arm/cntrl/fifo/path/en.h) \
    $(wildcard include/config/arm/cntrl/fifo/path/en/r.h) \
    $(wildcard include/config/arm/cntrl/fifo/path/en/l.h) \
    $(wildcard include/config/arm/cntrl/fifo/path/en/size.h) \
    $(wildcard include/config/arm/cntrl/fifo/path/en/mask.h) \
    $(wildcard include/config/arm/cntrl/fifo/path/en/range.h) \
    $(wildcard include/config/arm/cntrl/fifo/path/en/init.h) \
    $(wildcard include/config/arm/cntrl/fifo/path/en/rwmode.h) \
    $(wildcard include/config/arm/cntrl/fifo/path/en/get.h) \
    $(wildcard include/config/data/coherency/en.h) \
    $(wildcard include/config/data/coherency/en/r.h) \
    $(wildcard include/config/data/coherency/en/l.h) \
    $(wildcard include/config/data/coherency/en/size.h) \
    $(wildcard include/config/data/coherency/en/mask.h) \
    $(wildcard include/config/data/coherency/en/range.h) \
    $(wildcard include/config/data/coherency/en/init.h) \
    $(wildcard include/config/data/coherency/en/rwmode.h) \
    $(wildcard include/config/data/coherency/en/get.h) \
    $(wildcard include/config/col/bits.h) \
    $(wildcard include/config/col/bits/r.h) \
    $(wildcard include/config/col/bits/l.h) \
    $(wildcard include/config/col/bits/size.h) \
    $(wildcard include/config/col/bits/mask.h) \
    $(wildcard include/config/col/bits/range.h) \
    $(wildcard include/config/col/bits/init.h) \
    $(wildcard include/config/col/bits/rwmode.h) \
    $(wildcard include/config/col/bits/get.h) \
    $(wildcard include/config/arm/aux/fifo/path/en.h) \
    $(wildcard include/config/arm/aux/fifo/path/en/r.h) \
    $(wildcard include/config/arm/aux/fifo/path/en/l.h) \
    $(wildcard include/config/arm/aux/fifo/path/en/size.h) \
    $(wildcard include/config/arm/aux/fifo/path/en/mask.h) \
    $(wildcard include/config/arm/aux/fifo/path/en/range.h) \
    $(wildcard include/config/arm/aux/fifo/path/en/init.h) \
    $(wildcard include/config/arm/aux/fifo/path/en/rwmode.h) \
    $(wildcard include/config/arm/aux/fifo/path/en/get.h) \
    $(wildcard include/config/arm/cntrl/port/config.h) \
    $(wildcard include/config/arm/cntrl/port/config/r.h) \
    $(wildcard include/config/r.h) \
    $(wildcard include/config/arm/cntrl/port/config/l.h) \
    $(wildcard include/config/l.h) \
    $(wildcard include/config/arm/cntrl/port/config/size.h) \
    $(wildcard include/config/arm/cntrl/port/config/mask.h) \
    $(wildcard include/config/arm/cntrl/port/config/range.h) \
    $(wildcard include/config/range.h) \
    $(wildcard include/config/arm/cntrl/port/config/init.h) \
    $(wildcard include/config/arm/cntrl/port/config/rwmode.h) \
    $(wildcard include/config/arm/cntrl/port/config/get.h) \
    $(wildcard include/config/get.h) \
    $(wildcard include/config/row/bits.h) \
    $(wildcard include/config/row/bits/r.h) \
    $(wildcard include/config/row/bits/l.h) \
    $(wildcard include/config/row/bits/size.h) \
    $(wildcard include/config/row/bits/mask.h) \
    $(wildcard include/config/row/bits/range.h) \
    $(wildcard include/config/row/bits/init.h) \
    $(wildcard include/config/row/bits/rwmode.h) \
    $(wildcard include/config/row/bits/get.h) \
    $(wildcard include/config/dram/width.h) \
    $(wildcard include/config/dram/width/r.h) \
    $(wildcard include/config/dram/width/l.h) \
    $(wildcard include/config/dram/width/size.h) \
    $(wildcard include/config/dram/width/mask.h) \
    $(wildcard include/config/dram/width/range.h) \
    $(wildcard include/config/dram/width/init.h) \
    $(wildcard include/config/dram/width/rwmode.h) \
    $(wildcard include/config/dram/width/get.h) \
    $(wildcard include/config/dram/type.h) \
    $(wildcard include/config/dram/type/r.h) \
    $(wildcard include/config/dram/type/l.h) \
    $(wildcard include/config/dram/type/size.h) \
    $(wildcard include/config/dram/type/mask.h) \
    $(wildcard include/config/dram/type/range.h) \
    $(wildcard include/config/dram/type/init.h) \
    $(wildcard include/config/dram/type/rwmode.h) \
    $(wildcard include/config/dram/type/get.h) \
  include/linux/broadcom/gpio_types.h \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/thread_info.h \
    $(wildcard include/config/arm/thumbee.h) \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/fpstate.h \
    $(wildcard include/config/vfpv3.h) \
    $(wildcard include/config/iwmmxt.h) \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/domain.h \
    $(wildcard include/config/io/36.h) \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/poison.h \
  include/linux/prefetch.h \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/processor.h \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/cache.h \
    $(wildcard include/config/arm/l1/cache/shift.h) \
    $(wildcard include/config/aeabi.h) \
  include/linux/stringify.h \
  include/linux/bottom_half.h \
  include/linux/spinlock_types.h \
  include/linux/spinlock_types_up.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/generic/hardirqs.h) \
  include/linux/spinlock_up.h \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/atomic.h \
  include/asm-generic/atomic-long.h \
  include/linux/spinlock_api_up.h \
  arch/arm/mach-bcm5892/include/mach/gpio.h \
    $(wildcard include/config/have/gpio/lib.h) \
  include/asm-generic/gpio.h \
    $(wildcard include/config/gpiolib.h) \
    $(wildcard include/config/gpio/sysfs.h) \
  include/linux/errno.h \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/errno.h \
  include/asm-generic/errno.h \
  include/asm-generic/errno-base.h \
  arch/arm/mach-bcm5892/include/mach/reg_gpio.h \
  arch/arm/mach-bcm5892/include/mach/irqs.h \
  include/linux/delay.h \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/delay.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/string.h \
  include/linux/jiffies.h \
  include/linux/math64.h \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/div64.h \
  include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  include/linux/seqlock.h \
  include/linux/timex.h \
    $(wildcard include/config/no/hz.h) \
  /usr/src/linux-2.6.32-env/arch/arm/include/asm/timex.h \
  arch/arm/mach-bcm5892/include/mach/timex.h \
  /mnt/hgfs/Myproject/smartcard/BOARD/board.h \
  /mnt/hgfs/Myproject/smartcard/BOARD/bcm5892_define.h \
  /mnt/hgfs/Myproject/smartcard/IC/bcm5892sc_define.h \
  /mnt/hgfs/Myproject/smartcard/MEMORY/memory.h \
  /mnt/hgfs/Myproject/smartcard/BOARD/bcm5892_define.h \
  /mnt/hgfs/Myproject/smartcard/MEMORY/ic_memory.h \

/mnt/hgfs/Myproject/smartcard/MEMORY/memory.o: $(deps_/mnt/hgfs/Myproject/smartcard/MEMORY/memory.o)

$(deps_/mnt/hgfs/Myproject/smartcard/MEMORY/memory.o):
