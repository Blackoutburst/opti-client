package dev.blackoutburst.game.utils

import com.sun.management.OperatingSystemMXBean
import java.lang.management.ManagementFactory
import java.lang.management.ThreadMXBean

object SystemMonitor {
    private val OS_BEAN: OperatingSystemMXBean = ManagementFactory.getPlatformMXBean(OperatingSystemMXBean::class.java)
    private val RUNTIME: Runtime = Runtime.getRuntime()
    private val THREAD_BEAN: ThreadMXBean = ManagementFactory.getThreadMXBean()

    val cPUProcessUsage: Int
        get() = (OS_BEAN.processCpuLoad * 100).toInt()

    val memoryUsage: Int
        get() {
            val totalMemory = RUNTIME.totalMemory()
            val freeMemory = RUNTIME.freeMemory()
            return ((totalMemory - freeMemory) / (1024 * 1024)).toInt()
        }

    val memoryTotal: Int
        get() = (RUNTIME.totalMemory() / (1024 * 1024)).toInt()

    val memoryFree: Int
        get() = (RUNTIME.freeMemory() / (1024 * 1024)).toInt()

    val threadCount: Int
        get() = THREAD_BEAN.threadCount
}
