/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_NOCI_PROFILE_H
#define _LINUX_NOCI_PROFILE_H
#ifdef CONFIG_NOCI_BALANCE
#define NOCI_SUGOV_UP_US 1000U
#define NOCI_SUGOV_DOWN_US 10000U
#define NOCI_SWAPPINESS 60
#define NOCI_GPU_BOOST 1U
#else
#define NOCI_SUGOV_UP_US 500U
#define NOCI_SUGOV_DOWN_US 20000U
#define NOCI_SWAPPINESS 0
#define NOCI_GPU_BOOST 3U
#endif
/* A display floor must never override the current thermal/policy maximum. */
static inline unsigned int noci_screen_floor(unsigned int requested_min,
 unsigned int maximum, unsigned int minimum, int screen_on)
{
#ifdef CONFIG_NOCI_BALANCE
	unsigned int floor = requested_min;
	if (screen_on) {
		if (floor < 1209600U)
			floor = 1209600U;
		if (floor < minimum)
			floor = minimum;
		if (floor > maximum)
			floor = maximum;
	}
	return floor;
#else
	(void)maximum; (void)minimum; (void)screen_on;
	return requested_min;
#endif
}
#endif
