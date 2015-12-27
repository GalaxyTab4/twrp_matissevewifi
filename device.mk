LOCAL_PATH := device/samsung/matissewifi

#ifeq ($(TARGET_PREBUILT_KERNEL),)
#	LOCAL_KERNEL := $(LOCAL_PATH)/kernel/kernel
#else
#	LOCAL_KERNEL := $(TARGET_PREBUILT_KERNEL)
#endif

#PRODUCT_COPY_FILES += \
#    device/samsung/matisse/kernel/kernel:kernel \

$(call inherit-product, build/target/product/full.mk)

PRODUCT_NAME := matissewifi
