# Ingenic OpenIPC U-Boot

This repository hosts the unofficial, enhanced OpenIPC U-Boot tailored for the Ingenic Xburst T10-T31 SoCs. It comes packed with a range of features to enhance usability and functionality for these specific SoCs.

---

## Features

- **Automatic Partition Recognition**: Seamless compatibility with OpenIPC or custom firmware, recognizing partitions automatically.
- **DHCP Client**: DHCP functionality for automatic network configuration.
- **FAT-Write Capability**: Allows writing to FAT file systems, enhancing file management flexibility.
- **Filesystem Compatibility**: Supports ext2 and ext4 filesystems for broader storage options.
- **Functional Autoupdate**: An effective, built-in system for automatic firmware updates via SD card.
- **GPIO Environment Variable Storage**: Store and set GPIO configurations right at the device's startup.
- **Hush Command Shell**: An advanced, interactive command-line interface.
- **JFFS2 Support**: Integrated support for JFFS2 filesystems.
- **SDStart**: Specialized command for automated loading of the linux kernel from an SD card.
- **TFTP Server**: Built-in TFTP server to facilitate loading and recovery.
- **USB Networking Support**: Integrated support for ASIX USB adapters, facilitating network connectivity.
- **USB Storage**: Enables the use of USB storage devices.

---

## Please visit the [Wiki](https://github.com/gtxaspec/u-boot-ingenic/wiki) for useful information regarding compiling, configuration, and usage!

---

## Disclaimer
### **USE AT YOUR OWN RISK**
While this U-Boot version is designed to enhance functionality, its usage comes with inherent risks, particularly when modifying firmware or device settings. Users should proceed with caution and at their own risk.
