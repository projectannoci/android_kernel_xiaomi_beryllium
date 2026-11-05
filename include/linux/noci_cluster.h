/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_NOCI_CLUSTER_H
#define _LINUX_NOCI_CLUSTER_H
static inline int noci_cluster_cpu(unsigned int allowed, unsigned int active,
		int top, int prev, const unsigned int load[8])
{
	unsigned int candidates = allowed & active & (top ? 0xf0U : 0x0fU);
	unsigned int best_load = ~0U;
	int cpu, best = -1;
	for (cpu = 0; cpu < 8; cpu++) {
		if (!(candidates & (1U << cpu)))
			continue;
		if (load[cpu] < best_load ||
		    (load[cpu] == best_load && cpu == prev)) {
			best = cpu;
			best_load = load[cpu];
		}
	}
	return best;
}
#endif
