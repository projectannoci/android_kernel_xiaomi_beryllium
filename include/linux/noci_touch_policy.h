/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_NOCI_TOUCH_POLICY_H
#define _LINUX_NOCI_TOUCH_POLICY_H
static inline int noci_touch_target_cpu(unsigned int available_bits)
{
	/* CPU0 remains online on the normal beryllium hotplug path. */
	if (available_bits & 1U)
		return 0;
	if (available_bits & 2U)
		return 1;
	return -1;
}
#endif
