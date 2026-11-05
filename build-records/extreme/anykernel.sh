# AnyKernel3 Ramdisk Mod Script
# osm0sis @ xda-developers

## AnyKernel setup
# begin properties
properties() { '
kernel.string=Singularity v1 Extreme @ProjectNoci #1
do.devicecheck=1
do.modules=0
do.systemless=1
do.cleanup=1
do.cleanuponabort=0
device.name1=beryllium
device.name2=PocoPhone F1
device.name3=
device.name4=
device.name5=
supported.versions=13
supported.patchlevels=
'; } # end properties

# shell variables
block=/dev/block/bootdevice/by-name/boot;
is_slot_device=0;
ramdisk_compression=auto;
patch_vbmeta_flag=auto;


## AnyKernel methods (DO NOT CHANGE)
# import patching functions/variables - see for reference
. tools/ak3-core.sh;

[ -s "$home/Image.gz-dtb" ] || abort "Kernel payload missing.";
mkdir "$home/kernel-check" || abort "Kernel check directory failed.";
if ! (cd "$home/kernel-check" && "$bin/magiskboot" split "$home/Image.gz-dtb") >/dev/null 2>&1; then
  rm -rf "$home/kernel-check";
  abort "Invalid kernel payload.";
fi;
noci_kernel_magic=$(head -c 60 "$home/kernel-check/kernel" | tail -c 4);
rm -rf "$home/kernel-check";
[ "$noci_kernel_magic" = ARMd ] || abort "Invalid ARM64 kernel.";


## AnyKernel file attributes
# set permissions/ownership for included ramdisk files
set_perm_recursive 0 0 755 644 $ramdisk/*;
set_perm_recursive 0 0 750 750 $ramdisk/init* $ramdisk/sbin;


## AnyKernel boot install
# This build uses physical system + cust/System_Ext, not dynamic partitions.
if [ "$(getprop ro.boot.dynamic_partitions)" = "true" ] || [ -e /dev/block/mapper/system ] || [ -e /dev/block/mapper/system_a ]; then
  abort "Requires Android 13 physical System_Ext layout.";
fi;
ui_print "Singularity v1 Extreme @ProjectNoci #1";
dump_boot;

write_boot;
## end boot install


# shell variables
#block=vendor_boot;
#is_slot_device=1;
#ramdisk_compression=auto;
#patch_vbmeta_flag=auto;

# reset for vendor_boot patching
#reset_ak;


## AnyKernel vendor_boot install
#split_boot; # skip unpack/repack ramdisk since we don't need vendor_ramdisk access

#flash_boot;
## end vendor_boot install

