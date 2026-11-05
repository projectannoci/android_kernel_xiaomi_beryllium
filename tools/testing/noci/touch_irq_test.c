/* SPDX-License-Identifier: GPL-2.0 */
#include <assert.h>
#include <stdio.h>
#include <linux/noci_touch_policy.h>
int main(void)
{
 assert(noci_touch_target_cpu(0xff)==0);
 assert(noci_touch_target_cpu(0x01)==0);
 assert(noci_touch_target_cpu(0xfe)==1);
 assert(noci_touch_target_cpu(0x02)==1);
 assert(noci_touch_target_cpu(0xfc)==-1);
 assert(noci_touch_target_cpu(0)==-1);
 puts("touch IRQ: CPU0 preferred, CPU1 fallback, no BIG fallback");
}
