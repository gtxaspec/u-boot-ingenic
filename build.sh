#!/bin/bash

: ${CROSS_COMPILE:="mipsel-linux-"}

OUTPUT_DIR="./uboot_build"

if command -v resize; then
	eval $(resize)
	size="$(( LINES - 4 )) $(( COLUMNS -4 )) $(( LINES - 12 ))"
else
	size="20 76 12"
fi

pick_a_soc() {
	eval `resize`
	soc=$(whiptail --title "U-Boot SoC selection" \
		--menu "Choose a SoC model" $size \
		"isvp_t10_sfcnor_lite"		"Ingenic T10L"		\
		"isvp_t10_sfcnor"		"Ingenic T10N"		\
		"isvp_t10_msc0"			"Ingenic T10N  MSC0"	\
		"isvp_t20_sfcnor_lite"		"Ingenic T20L"		\
		"isvp_t20_sfcnor"		"Ingenic T20N"		\
		"isvp_t20_sfcnor_ddr128M"	"Ingenic T20X"		\
		"isvp_t20_msc0"			"Ingenic T20N  MSC0"	\
		"isvp_t21_sfcnor"		"Ingenic T21N"		\
		"isvp_t21_msc0"			"Ingenic T21N  MSC0"	\
		"isvp_t23n_sfcnor"		"Ingenic T23N"		\
		"isvp_t23n_msc0"		"Ingenic T23N  MSC0"	\
		"isvp_t30_sfcnor_lite"		"Ingenic T30L"		\
		"isvp_t30_sfcnor"		"Ingenic T30N"		\
		"isvp_t30_sfcnor_ddr128M"	"Ingenic T30X"		\
		"isvp_t30a_sfcnor_ddr128M"	"Ingenic T30A"		\
		"isvp_t30a1_sfcnor_ddr128M"	"Ingenic T30A1"		\
		"isvp_t30_msc0"			"Ingenic T30N  MSC0"	\
		"isvp_t31_sfcnor_lite"		"Ingenic T31L"		\
		"isvp_t31lc_sfcnor"		"Ingenic T31LC"		\
		"isvp_t31_sfcnor"		"Ingenic T31N"		\
		"isvp_t31_sfcnor_ddr128M"	"Ingenic T31X"		\
		"isvp_t31a_sfcnor_ddr128M"	"Ingenic T31A"		\
		"isvp_t31al_sfcnor_ddr128M"	"Ingenic T31AL"		\
		"isvp_t31_msc0"			"Ingenic T31N  MSC0"	\
		"isvp_t31a_msc0_ddr128M"	"Ingenic T31A  MSC0"	\
		"isvp_t31al_msc0_ddr128M"	"Ingenic T31AL MSC0"	\
		"isvp_t31_msc0_lite"		"Ingenic T31L  MSC0"	\
		"isvp_t31_msc0_ddr128M"		"Ingenic T31X  MSC0"	\
		"isvp_t31_xiaomi_sfcnor"	"Ingenic T31 XIAOMI SPL"	\
		--notags 3>&1 1>&2 2>&3)
}

# Function to build a specific version
build_version() {
	# Start timer
	SECONDS=0

	local soc=$1
	echo "Building U-Boot for ${soc}"

	make distclean
	mkdir -p "${OUTPUT_DIR}" >/dev/null
	make $soc
	make CROSS_COMPILE=$CROSS_COMPILE -j$(nproc)
	if [ -f u-boot-lzo-with-spl.bin ]; then
		echo "u-boot-lzo-with-spl.bin exists, copying..."
		cp u-boot-lzo-with-spl.bin "${OUTPUT_DIR}/u-boot-${soc}.bin"
	elif [ -f u-boot-with-spl.bin ]; then
		echo "u-boot-with-spl.bin exists, copying..."
		cp u-boot-with-spl.bin "${OUTPUT_DIR}/u-boot-${soc}.bin"
	fi
}

soc="$1"
[ -z "$soc" ] && pick_a_soc
[ -z "$soc" ] && echo No SoC && exit 1
build_version "$soc"

# End timer and report
duration=$SECONDS
echo "Done"
echo "Total build time: $(($duration / 60)) minutes and $(($duration % 60)) seconds."
exit 0
