#!/bin/bash

export ARCH=mips
export CROSS_COMPILE=mipsel-linux-gnu-
#export CCACHE=/usr/bin/ccache

OUTPUT_DIR="./uboot_build"

if command -v resize >/dev/null; then
	eval $(resize)
	size="$(( LINES - 4 )) $(( COLUMNS -4 )) $(( LINES - 12 ))"
else
	size="20 50 12"
fi

profiles() {
	awk '/^isvp_/ {sub(/isvp_/, "", $1); print $1}' boards.cfg
}

list_of_profiles() {
	awk '/^isvp_/ {sub(/isvp_/, "", $1); print $1 " " $1 " "}' boards.cfg
}

pick_a_soc() {
	profiles=$(echo $(list_of_profiles))
	whiptail --title "U-Boot SoC selection" --menu "Choose a SoC model" \
		--notags $size $profiles 3>&1 1>&2 2>&3
}

build_version() {
	[ -z "$profile" ] && help_and_exit

	echo "Building U-Boot for ${profile}"
	# Start timer
	local SECONDS=0
	make distclean
	mkdir -p "${OUTPUT_DIR}" >/dev/null
	make isvp_${profile}
	make -j$(nproc)
	cp u-boot-lzo-with-spl.bin "${OUTPUT_DIR}/u-boot-${profile}.bin"
	make distclean

	# End timer and report
	duration=$SECONDS
	echo "Done"
	echo "u-boot-${profile}.bin": $(stat -c %s "${OUTPUT_DIR}/u-boot-${profile}.bin")
	echo "Total build time: $(($duration / 60)) minutes and $(($duration % 60)) seconds."
}

help_and_exit() {
	echo "Usage: $0 [<SoC>]"
	echo "SoC:" $(profiles)
	exit 1
}

soc="$1"
boot="${2:-sfcnor}"

case "$soc" in
	-a | all)
		for profile in $(profiles); do
			build_version $profile
		done
		;;
	t10* | t20* | t21* | t23* | t30* | t31*)
		awk '/^isvp_'${soc}'/ && /'${boot}'/ {print $1}' boards.cfg

		#build_version $profile
		;;
	"")
		profile=$(pick_a_soc)
		build_version $profile
		;;
	*)
		help_and_exit
esac

exit 0
