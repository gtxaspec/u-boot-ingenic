#!/bin/bash

export ARCH=mips
export CROSS_COMPILE=mipsel-linux-gnu-
#export CCACHE=/usr/bin/ccache

declare -A cfg

# NOR
# T31N make isvp_t31_sfcnor
# T31L make isvp_t31_sfcnor_lite
# T31X make isvp_t31_sfcnor_ddr128M
# T31A make isvp_t31a_sfcnor_ddr128M
# T31AL make isvp_t31al_sfcnor_ddr128M
# MMC
# T31N make isvp_t31_msc0
# T31L make isvp_t31_msc0_lite
# T31X make isvp_t31_msc0_ddr128M
# T31A make isvp_t31a_msc0_ddr128M
# T31AL make isvp_t31al_msc0_ddr128M

cfg[t10n]="isvp_t10_sfcnor"
cfg[t10l]="isvp_t10_sfcnor_lite"
cfg[t10n_msc0]="isvp_t10_msc0"

cfg[t20n]="isvp_t20_sfcnor"
cfg[t20l]="isvp_t20_sfcnor_lite"
cfg[t20x]="isvp_t20_sfcnor_ddr128M"
cfg[t20n_msc0]="isvp_t20_msc0"

cfg[t21n]="isvp_t21_sfcnor"
cfg[t21n_msc0]="isvp_t21_msc0"

cfg[t30n]="isvp_t30_sfcnor"
cfg[t30l]="isvp_t30_sfcnor_lite"
cfg[t30x]="isvp_t30_sfcnor_ddr128M"
cfg[t30a]="isvp_t30a_sfcnor_ddr128M"
cfg[t30a1]="isvp_t30a1_sfcnor_ddr128M"
cfg[t30n_msc0]="isvp_t30_msc0"

cfg[t31n]="isvp_t31_sfcnor"
cfg[t31l]="isvp_t31_sfcnor_lite"
cfg[t31lc]="isvp_t31lc_sfcnor"
cfg[t31x]="isvp_t31_sfcnor_ddr128M"
cfg[t31a]="isvp_t31a_sfcnor_ddr128M"
cfg[t31al]="isvp_t31al_sfcnor_ddr128M"
cfg[t31n_msc0]="isvp_t31_msc0"
cfg[t31a_msc0]="isvp_t31a_msc0_ddr128M"
cfg[t31al_msc0]="isvp_t31al_msc0_ddr128M"

OUTPUT_DIR="./uboot_build"

# Function to build a specific version
build_version() {
	local soc=$1
	echo "Building U-Boot for ${soc}"
	log="building-${soc}.log"; :>$log
	make distclean    >>$log 2>&1
	mkdir -p ${OUTPUT_DIR} >/dev/null 2>&1
	make ${cfg[$soc]} >>$log 2>&1
	make -j$(nproc)   >>$log 2>&1
	cp u-boot-lzo-with-spl.bin ${OUTPUT_DIR}/u-boot-${soc}.bin
}

# Start timer
SECONDS=0

# Check if an argument was provided
if [ $# -eq 0 ]; then
	# No argument, build all versions
	for soc in "${!cfg[@]}"; do
		build_version $soc
	done
else
	# Argument provided, build specific version
	if [[ -n ${cfg[$1]} ]]; then
		build_version $1
	else
		echo "Invalid argument: $1"
		exit 1
	fi
fi

# End timer and report
duration=$SECONDS
echo "Done"
echo "Total build time: $(($duration / 60)) minutes and $(($duration % 60)) seconds."
exit 0
