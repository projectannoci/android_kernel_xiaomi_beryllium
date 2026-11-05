/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_NOCI_TOUCH_IRQ_H
#define _LINUX_NOCI_TOUCH_IRQ_H
#include <linux/cpu.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/printk.h>
#include <linux/noci_touch_policy.h>
#ifdef CONFIG_NOCI_TOUCH_IRQ_FIX
#define NOCI_FOCAL_IRQ_THREAD_FLAGS (IRQF_ONESHOT | IRQF_NOBALANCING)
#define NOCI_NVT_IRQ_THREAD_FLAGS (IRQF_ONESHOT | IRQF_NOBALANCING)
#else
#define NOCI_FOCAL_IRQ_THREAD_FLAGS (IRQF_ONESHOT | IRQF_PERF_CRITICAL)
#define NOCI_NVT_IRQ_THREAD_FLAGS IRQF_ONESHOT
#endif
static inline int noci_touch_irq_affine(unsigned int irq)
{
#ifdef CONFIG_NOCI_TOUCH_IRQ_FIX
	unsigned int available = (cpu_online(0) ? 1U : 0U) |
		(cpu_online(1) ? 2U : 0U);
	int cpu = noci_touch_target_cpu(available);
	int ret;

	if (cpu < 0)
		return -ENODEV;
	/* Set actual affinity, not just an advisory hint. */
	ret = irq_set_affinity(irq, cpumask_of(cpu));
	if (ret)
		pr_warn_ratelimited("noci: touch IRQ %u affinity failed: %d\n", irq, ret);
	return ret;
#else
	return irq_set_affinity(irq, cpu_perf_mask);
#endif
}
#endif
