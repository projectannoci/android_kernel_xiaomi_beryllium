/* SPDX-License-Identifier: GPL-2.0 */
#include <assert.h>
#include <stdio.h>
#include <linux/noci_cluster.h>
int main(void)
{
 unsigned int load[8] = {4,3,0,1,5,0,2,3};
 unsigned int allowed, active, top;
 int cpu, choice;
 assert(noci_cluster_cpu(0xff, 0xff, 1, 4, load) == 5);
 assert(noci_cluster_cpu(0xff, 0xff, 0, 0, load) == 2);
 assert(noci_cluster_cpu(0x10, 0xff, 0, 4, load) == -1);
 assert(noci_cluster_cpu(0xff, 0x0f, 1, 4, load) == -1);
 assert(noci_cluster_cpu(0x50, 0xff, 1, 4, load) == 6);
 for (allowed = 0; allowed < 256; allowed++)
  for (active = 0; active < 256; active++)
   for (top = 0; top < 2; top++) {
    unsigned int eligible = allowed & active & (top ? 0xf0 : 0x0f);
    choice = noci_cluster_cpu(allowed, active, top, 5, load);
    if (!eligible) { assert(choice == -1); continue; }
    assert(choice >= 0 && choice < 8);
    assert(eligible & (1U << choice));
    for (cpu = 0; cpu < 8; cpu++)
     if (eligible & (1U << cpu)) assert(load[choice] <= load[cpu]);
   }
 puts("131072 affinity/online/cluster boundary combinations passed");
 return 0;
}
