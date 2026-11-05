/* SPDX-License-Identifier: GPL-2.0 */
#include <assert.h>
#include <stdio.h>
/* Linux defines current as a function-like expansion; parameters must avoid it. */
#define current get_current()
#include <linux/noci_profile.h>
int main(void)
{
#ifdef CONFIG_NOCI_BALANCE
 assert(noci_screen_floor(300000,2803200,300000,1)==1209600);
 assert(noci_screen_floor(300000,960000,300000,1)==960000);
 assert(noci_screen_floor(1612800,2803200,300000,1)==1612800);
 assert(noci_screen_floor(300000,2803200,300000,0)==300000);
#else
 assert(noci_screen_floor(300000,2803200,300000,1)==300000);
#endif
 puts("profile floor: display transitions and lower thermal caps passed");
}
