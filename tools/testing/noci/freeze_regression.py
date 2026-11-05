from pathlib import Path
import subprocess,sys,json
r=Path(__file__).resolve().parents[3];d=r/'out-tests';d.mkdir(exist_ok=True)
def fn(path,marker):
 s=(r/path).read_text();a=s.index(marker);b=s.index('{',a);depth=1;i=b+1
 while depth:
  depth+=(s[i]=='{')-(s[i]=='}');i+=1
 return s[a:i]
common='#include <assert.h>\n#include <stdbool.h>\n#include <stdio.h>\n#include <linux/noci_profile.h>\n'
rate=common+'''typedef long long s64;
typedef unsigned long long u64;
struct sugov_policy { u64 last_freq_update_time; unsigned int next_freq; s64 up_rate_delay_ns,down_rate_delay_ns; };
'''+fn('kernel/sched/cpufreq_schedutil.c','static bool sugov_up_down_rate_limit(')+'''
int main(void) {
 struct sugov_policy p={1000000,1200000,NOCI_SUGOV_UP_US*1000ULL,NOCI_SUGOV_DOWN_US*1000ULL};
 assert(sugov_up_down_rate_limit(&p,1499999,1800000));
 assert(!sugov_up_down_rate_limit(&p,1500000,1800000));
 assert(sugov_up_down_rate_limit(&p,20999999,600000));
 assert(!sugov_up_down_rate_limit(&p,21000000,600000));
 assert(!sugov_up_down_rate_limit(&p,1000000,1200000));
 puts("schedutil: 499us blocked / 500us allowed; 20ms down retained");
}
'''
s=(r/'drivers/gpu/msm/kgsl_pwrctrl.c').read_text();start=s.index('int kgsl_pwrctrl_init(');a=s.index('\tpwr->max_pwrlevel = 0;',start);b=s.index('\tpwr->thermal_pwrlevel = 0;',a);init=s[a:b]
gpu='''#include <assert.h>
#include <stdio.h>
#define BIT(x) (1U<<(x))
#define KGSL_PWRFLAGS_NAP_OFF 0
#define max(a,b) ((a)>(b)?(a):(b))
#define min_t(t,a,b) ((t)(a)<(t)(b)?(t)(a):(t)(b))
#define max_t(t,a,b) ((t)(a)>(t)(b)?(t)(a):(t)(b))
#define KGSL_CONSTRAINT_PWRLEVEL 1
#define KGSL_CONSTRAINT_PWR_MAX 1
#define KGSL_CONSTRAINT_PWR_MIN 2
struct kgsl_pwrctrl {unsigned int num_pwrlevels,max_pwrlevel,min_pwrlevel,thermal_pwrlevel,thermal_pwrlevel_floor,ctrl_flags;struct {int gpu_freq;}pwrlevels[16];};
struct kgsl_pwr_constraint {int type,sub_type;};
static void init_levels(struct kgsl_pwrctrl *pwr) {int i=0;(void)i;
'''+init+'}\n'+fn('drivers/gpu/msm/kgsl_pwrctrl.c','static unsigned int _adjust_pwrlevel(')+'''
int main(void) {
 struct kgsl_pwrctrl p={.num_pwrlevels=10};struct kgsl_pwr_constraint c={0,0};
 init_levels(&p);assert(p.max_pwrlevel==0);assert(p.min_pwrlevel==8);
 p.thermal_pwrlevel_floor=p.min_pwrlevel;
 assert(_adjust_pwrlevel(&p,5,&c)==5);
 assert(_adjust_pwrlevel(&p,99,&c)==8);
 p.thermal_pwrlevel=3;assert(_adjust_pwrlevel(&p,0,&c)==3);
 puts("KGSL: lowest active floor restored; intermediate levels and thermal cap respected");
}
'''
tests={'schedutil':rate,'gpu':gpu};results=[]
for name,code in tests.items():
 p=d/(name+'_regression.c');p.write_text(code);exe=d/(name+'_regression')
 subprocess.run(['gcc','-O2','-Wall','-Wextra','-Werror','-Wno-sign-compare','-DCONFIG_NOCI_EXTREME','-I'+str(r/'include'),str(p),'-o',str(exe)],check=True)
 run=subprocess.run([str(exe.resolve())],text=True,capture_output=True);results.append({'test':name,'exit':run.returncode,'output':run.stdout+run.stderr});print(name,run.returncode,run.stdout+run.stderr)
(d/'regression-results.json').write_text(json.dumps(results,indent=2))
sys.exit(0 if all(x['exit']==0 for x in results) else 1)
