#!/bin/bash

export CROSS_COMPILE=mipsel-linux-gnu-

declare -A cfg

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

#ln -s compiler-gcc7.h include/linux/compiler-gcc10.h
mkdir -p ../ingenic-output

for soc in ${!cfg[@]}; do

make distclean
make ${cfg[$soc]}

make -j8
cp u-boot-lzo-with-spl.bin ../ingenic-output/u-boot-${soc}-universal.bin

done

mv ../ingenic-output -T output
