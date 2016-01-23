#cd ____coderoot____
#ln -s ____fullpathofme____ ____linkname____

#source ____build/envsetup.sh____
#lunch ____devicename____
#mmm ____linkname____

#adb root
#adb remount
#adb push coderoot/out/target/product/devicename/system/bin/42 /system/bin/




#0.head
LOCAL_PATH:=$(call my-dir)/../../..
include $(CLEAR_VARS)




#1.boot1
LOCAL_SRC_FILES := \
	library/libboot0/none/serial.rx.c \
	library/libboot0/none/serial.tx.c \
	library/libboot0/none/basic.c

#1.boot1
LOCAL_SRC_FILES += \
	library/libboot1/listen.c \
	library/libboot1/say.c \
	library/libboot1/debug.c

#2.hard0
ifeq ($(TARGET_ARCH),x86)
LOCAL_SRC_FILES += \
	library/libhard0/x86/x86.fp.c \
	library/libhard0/x86/x86.port.c \
	library/libhard0/x86/x86.platform.c \
	library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),x86_64)
LOCAL_SRC_FILES += \
	library/libhard0/x86/x86.fp.c \
	library/libhard0/x86/x86.port.c \
	library/libhard0/x86/x86.platform.c \
	library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),arm)
LOCAL_SRC_FILES += \
	library/libhard0/arm/arm.platform.c \
	library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),arm64)
LOCAL_SRC_FILES += \
	library/libhard0/arm/arm.platform.c \
	library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),mips)
LOCAL_SRC_FILES += \
	library/libhard0/mips/mips.platform.c \
	library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),mips64)
LOCAL_SRC_FILES += \
	library/libhard0/mips/mips.platform.c \
	library/libhard0/driver.c
endif

#2.hard1
LOCAL_SRC_FILES += \
	library/libhard1/body.c

#3.soft0
LOCAL_SRC_FILES += \
	library/libsoft0/linux/linux.arg.c \
	library/libsoft0/linux/linux.file.c \
	library/libsoft0/linux/linux.other.c \
	library/libsoft0/memory.c

#3.soft1
LOCAL_SRC_FILES += \
	library/libsoft1/compress/7z.c \
	library/libsoft1/compress/cpio.c \
	library/libsoft1/compress/gz.c \
	library/libsoft1/compress/tar.c \
	library/libsoft1/compress/zip.c \
	library/libsoft1/executable/elf.c \
	library/libsoft1/executable/macho.c \
	library/libsoft1/executable/pe.c \
	library/libsoft1/filesystem/fs.ext.c \
	library/libsoft1/filesystem/fs.fat.c \
	library/libsoft1/filesystem/fs.hfs.c \
	library/libsoft1/filesystem/fs.ntfs.c \
	library/libsoft1/manager/logic.c \
	library/libsoft1/manager/real.c \
	library/libsoft1/partitiontable/pt.gpt.c \
	library/libsoft1/partitiontable/pt.mbr.c \
	library/libsoft1/text/ascii.c \
	library/libsoft1/text/unicode.c \
	library/libsoft1/master.c \

#4.ui0
LOCAL_SRC_FILES += \
	library/libui0/cli/cli.cli.c \
	library/libui0/window.c

#4.ui1
LOCAL_SRC_FILES += \
	library/libui1/draw/background.c \
	library/libui1/draw/shape.c \
	library/libui1/text/ascii.c \
	library/libui1/character.c

#1.librarian
LOCAL_SRC_FILES += \
	library/user/librarian.c \
	library/user/cli.c




#2.so
LOCAL_SHARED_LIBRARIES += libc
LOCAL_LDFLAGS := -Wl,--hash-style=sysv
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := 42




#3.don't know
include $(BUILD_EXECUTABLE)
