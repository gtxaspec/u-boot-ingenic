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
cfg[t20n]="isvp_t20_sfcnor"
cfg[t20l]="isvp_t20_sfcnor_lite"
cfg[t20x]="isvp_t20_sfcnor_ddr128M"
cfg[t21n]="isvp_t21_sfcnor"
cfg[t30n]="isvp_t30_sfcnor"
cfg[t30l]="isvp_t30_sfcnor_lite"
cfg[t30x]="isvp_t30_sfcnor_ddr128M"
cfg[t30a]="isvp_t30a_sfcnor_ddr128M"
cfg[t30a1]="isvp_t30a1_sfcnor_ddr128M"
cfg[t31n]="isvp_t31_sfcnor"
cfg[t31l]="isvp_t31_sfcnor_lite"
cfg[t31x]="isvp_t31_sfcnor_ddr128M"
cfg[t31a]="isvp_t31a_sfcnor_ddr128M"
cfg[t31al]="isvp_t31al_sfcnor_ddr128M"
cfg[t31lc]="isvp_t31lc_sfcnor"

OUTPUTDIR="./uboot_build"

# Collecting platform names and formatting output
platforms=""
column_count=0
for key in "${!cfg[@]}"; do
	platforms+="${cfg[$key]}\t"
	((++column_count))
	if [ $column_count -eq 1 ]; then
		platforms+="\n"
		column_count=0
	fi
done

echo -e "Building U-Boot 2013.07 on Ingenic XBurst1 for:\n$platforms"

# Start timer
SECONDS=0

# Building process
for soc in "${!cfg[@]}"; do
	echo "Building U-Boot for ${soc}"
	log=building-${soc}.log; :>$log
	make distclean    >>$log 2>&1
	mkdir -p ${OUTPUTDIR} 2>&1 > /dev/null
	make ${cfg[$soc]} >>$log 2>&1
	make -j$(nproc)   >>$log 2>&1
	cp u-boot-lzo-with-spl.bin ${OUTPUTDIR}/u-boot-${soc}-gtxaspec.bin
done

# End timer and report
duration=$SECONDS
echo "Total build time: $(($duration / 60)) minutes and $(($duration % 60)) seconds."