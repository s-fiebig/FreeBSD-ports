--- deps/v8/src/arm/cpu-arm.cc.orig	2019-08-06 20:46:23 UTC
+++ deps/v8/src/arm/cpu-arm.cc
@@ -7,6 +7,9 @@
 #ifdef __QNXNTO__
 #include <sys/mman.h>  // for cache flushing.
 #undef MAP_TYPE
+#elif defined(__FreeBSD__)
+#include <sys/types.h>
+#include <machine/sysarch.h> // for cache flushing.
 #else
 #include <sys/syscall.h>  // for cache flushing.
 #endif
@@ -26,6 +29,9 @@ __attribute__((noinline)) void CpuFeatures::FlushICach
 #if !defined(USE_SIMULATOR)
 #if V8_OS_QNX
   msync(start, size, MS_SYNC | MS_INVALIDATE_ICACHE);
+#elif defined(__FreeBSD__)
+  struct arm_sync_icache_args args = { .addr = (uintptr_t)start, .len = size };
+  sysarch(ARM_SYNC_ICACHE, (void *)&args);
 #else
   register uint32_t beg asm("r0") = reinterpret_cast<uint32_t>(start);
   register uint32_t end asm("r1") = beg + size;
