LOCAL_PATH := device/samsung/matissevewifi

ifeq ($(TARGET_PREBUILT_KERNEL),)
	LOCAL_KERNEL := $(LOCAL_PATH)/kernel/zImage
else
	LOCAL_KERNEL := $(TARGET_PREBUILT_KERNEL)
endif

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/kernel/zImage:kernel

$(call inherit-product, build/target/product/full.mk)

PRODUCT_NAME := matissevewifi
