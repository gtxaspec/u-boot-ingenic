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
- **SDStart**: Specialized command for automated loading of the linux kernel from an SD card.
- **USB Networking Support**: Integrated support for ASIX USB adapters, facilitating network connectivity.
- **USB Storage**: Enables the use of USB storage devices.

---

## Sample Default Environment Settings:
```
OpenIPC # printenv
baseaddr=0x80600000
baudrate=115200
bootargs=mem=${osmem} rmem=${rmem} console=${serialport},${baudrate}n8 panic=${panic_timeout} root=/dev/mtdblock3 rootfstype=squashfs init=/init mtdparts=jz_sfc:256k(boot),64k(env),${kern_size}(kernel),${rootfs_size}(rootfs),-(rootfs_data)
bootcmd=sf probe;setenv setargs setenv bootargs ${bootargs};run setargs;sf read ${baseaddr} 0x50000 ${kern_len};bootm ${baseaddr};
bootdelay=1
ethaddr=00:00:23:34:45:66
gatewayip=192.168.1.1
gpio_default=62O
gpio_default_ircut=25o 26o 52o 53o 49o 50o
gpio_default_net=61o 48o
ipaddr=192.168.1.10
loads_echo=1
netmask=255.255.255.0
osmem=64M
panic_timeout=10
restore=n
rmem=64M@0x4000000
serialport=ttyS1
serverip=192.168.1.254
soc=t31
totalmem=128M
```
---

## GPIOS

### GPIO Specific Variables:
- `gpio_default`: GPIOs configured during device boot.
- `gpio_default_ircut`: GPIOs set at device boot for IRCUT saftey.  De-energizes common IRCUT GPIOs to prevent damage to IRCUT coils.
- `gpio_default_net`: GPIOs configured at boot only if the device lacks a physical ethernet connection.

- **GPIO Configuration Format:**
  - **GPIO Number**: This is a numerical identifier for the specific GPIO.
  - **Operation Mode**: This is indicated by appending a letter to the GPIO number.
    - `I` (uppercase 'i'): Configures the GPIO as an Input.
    - `O` (uppercase 'o'): Sets the GPIO as an Output with a High (logic 1) state.
    - `o` (lowercase 'o'): Sets the GPIO as an Output with a Low (logic 0) state.
    - Space Separation: Each GPIO setting must be separated by a space to distinguish individual configurations

### GPIO Specific MOTOR Variables:
- **Different Format Alert**: Unlike other GPIO configurations, `gpio_motor_v` and `gpio_motor_h` use a unique format.
- **`gpio_motor_v` and `gpio_motor_h`**: These variables configure the GPIOs for vertical and horizontal motors. They use a space-separated format for GPIO numbers, each critical for managing motor operation phases. For example, `gpio_motor_v=12 13 14 15` assigns GPIOs 12 to 15 to different phases of the vertical motor's function.  These motor-specific GPIOs will be set to an input state at boot.  This setup prevents motor overheating, particularly if the unit is stuck in the bootloader, thereby ensuring motor integrity and efficient, safe operation.

#### GPIO Examples:
- `gpio_default=62O`:
  - This sets GPIO 62 as an Output with a High state.
- `gpio_default=62o 54O 64I`:
  - GPIO 62 is set as an Output with a Low state.
  - GPIO 54 is set as an Output with a High state.
  - GPIO 64 is configured as an Input.
  - Each GPIO configuration is distinctly separated by spaces.
- `gpio_motor_h=52 53 57 51`:
  - GPIOs 52, 53, 57, and 51 correspond to Phases A, B, C, and D of the horizontal motor, respectively.

This format provides a straightforward and efficient method for defining the state and direction (input or output) of GPIO pins in the system.

---

### Usage Instructions:

- **USB Ethernet Activation**: Initiate with the command `usb start`.
- **Booting Kernel from SD Card**: Utilize the command `sdstart`.
- **Updating Flash Storage via SD Card**: Employ a full image file named `autoupdate-full.bin` on the SD card, followed by executing `sdupdate`.
- **Command Reference**: Access a comprehensive list of commands using `help`.

---

## Disclaimer
### **USE AT YOUR OWN RISK**
While this U-Boot version is designed to enhance functionality, its usage comes with inherent risks, particularly when modifying firmware or device settings. Users should proceed with caution and at their own risk.
