# Mod
TWRP_VERSION := 20151227
PREBUILT_KERNEL := true

LOCAL_PATH := device/samsung/matissewifi

# Architecture
TARGET_ARCH := arm
TARGET_ARCH_VARIANT := armv7-a-neon
TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi
TARGET_CPU_SMP := true
TARGET_CPU_VARIANT := krait
ARCH_ARM_HAVE_TLS_REGISTER := true

# Flags
TARGET_GLOBAL_CFLAGS += -mfpu=neon -mfloat-abi=softfp
TARGET_GLOBAL_CPPFLAGS += -mfpu=neon -mfloat-abi=softfp
COMMON_GLOBAL_CFLAGS += -DNO_SECURE_DISCARD
TARGET_RECOVERY_DEVICE_DIRS += device/samsung/matissewifi
COMMON_GLOBAL_CFLAGS += -DQCOM_BSP

# Init
# TARGET_PLATFORM_DEVICE_BASE := /devices/msm_sdcc.1/

# Kernel
TARGET_PREBUILT_KERNEL := $(LOCAL_PATH)/kernel/zImage
BOARD_CUSTOM_BOOTIMG_MK := $(LOCAL_PATH)/mkbootimg.mk
BOARD_MKBOOTIMG_ARGS := --board $(TWRP_VERSION) --ramdisk_offset 0x02000000 --tags_offset 0x1e00000
TARGET_CUSTOM_KERNEL_HEADERS := $(LOCAL_PATH)/include
BOARD_KERNEL_CMDLINE := console=null androidboot.console=null androidboot.hardware=qcom user_debug=23 msm_rtb.filter=0x37 androidboot.bootdevice=msm_sdcc.1
BOARD_KERNEL_BASE := 0x00000000
BOARD_KERNEL_PAGESIZE := 2048
TARGET_KERNEL_SOURCE := kernel/samsung/matisse

ifneq ($(PREBUILT_KERNEL),true)
BOARD_KERNEL_SEPARATED_DT := true
TARGET_KERNEL_CONFIG := twrp_matissewifi_defconfig
endif

# USB Mounting
TARGET_USE_CUSTOM_LUN_FILE_PATH := /sys/devices/platform/msm_hsusb/gadget/lun0/file

# Recovery
TARGET_USERIMAGES_USE_EXT4 := true
BOARD_HAS_LARGE_FILESYSTEM := true

# TWRP-Specific
TARGET_RECOVERY_FSTAB := $(LOCAL_PATH)/recovery/root/etc/recovery.fstab
TW_THEME := landscape_hdpi
TW_HAS_DOWNLOAD_MODE := true
TW_INTERNAL_STORAGE_PATH := "/data/media"
TW_INTERNAL_STORAGE_MOUNT_POINT := "data"
TW_EXTERNAL_STORAGE_PATH := "/external_sd"
TW_EXTERNAL_STORAGE_MOUNT_POINT := "external_sd"
TW_DEFAULT_EXTERNAL_STORAGE := true
TW_BRIGHTNESS_PATH := /sys/class/leds/lcd-backlight/brightness
RECOVERY_GRAPHICS_USE_LINELENGTH := true
RECOVERY_SDCARD_ON_DATA := true
BOARD_SUPPRESS_SECURE_ERASE := true
TW_NO_REBOOT_BOOTLOADER := true
HAVE_SELINUX := true
TARGET_RECOVERY_PIXEL_FORMAT := "RGBX_8888"
TARGET_RECOVERY_QCOM_RTC_FIX := true

