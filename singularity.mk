# SPDX-License-Identifier: GPL-2.0
JOBS ?= 8
export KBUILD_BUILD_USER := Singularity
export KBUILD_BUILD_HOST := ProjectNoci
export KBUILD_BUILD_VERSION := 1
ARGS = ARCH=arm64 CC=clang-14 HOSTCC=gcc CROSS_COMPILE=aarch64-linux-gnu- CROSS_COMPILE_ARM32=arm-linux-gnueabi- CLANG_TRIPLE=aarch64-linux-gnu-
.PHONY: all extreme balance
all: extreme balance
extreme:
	$(MAKE) $(ARGS) O=out-extreme singularity_extreme_defconfig
	$(MAKE) -j$(JOBS) $(ARGS) O=out-extreme Image.gz-dtb
balance:
	$(MAKE) $(ARGS) O=out-balance singularity_balance_defconfig
	$(MAKE) -j$(JOBS) $(ARGS) O=out-balance Image.gz-dtb
