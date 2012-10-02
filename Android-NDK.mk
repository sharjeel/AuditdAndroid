# Build the auditctl binary
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

ETC_DIR := $(TARGET_OUT)/etc/audit

LOCAL_MODULE := auditctl
LOCAL_MODULE_TAGS := eng
LOCAL_SRC_FILES:= lib/libaudit.c \
			lib/message.c \
			lib/netlink.c \
			lib/lookup_table.c \
			lib/audit_logging.c \
			lib/deprecated.c \
			src/auditctl.c \
			src/auditctl-llist.c \
			src/delete_all.c


LOCAL_C_INCLUDES :=	./ \
			./lib \
			./libc \
			./kernel \
			./src

LOCAL_CFLAGS :=  -fPIE -DPIE -g -D_GNU_SOURCE -fno-strict-aliasing 

LOCAL_SHARED_LIBRARIES := libc libcutils
LOCAL_LDLIBS := -llog

include $(BUILD_EXECUTABLE)


# Build the auditd binary
include $(CLEAR_VARS)

MY_ROOT_PATH := external/android_audit

LOCAL_MODULE := auditd
LOCAL_MODULE_TAGS := eng
LOCAL_SRC_FILES:= lib/libaudit.c \
			lib/message.c \
			lib/netlink.c \
			lib/lookup_table.c \
			lib/audit_logging.c \
			lib/deprecated.c \
			src/auditd.c \
			src/auditd-event.c \
			src/auditd-config.c \
			src/auditd-reconfig.c \
			src/auditd-sendmail.c \
			src/auditd-listen.c \
			src/auditd-parse.c \
			src/ev.c \
			src/event.c


LOCAL_C_INCLUDES :=	./ \
			./lib \
			./libc \
			./kernel \
			./src

LOCAL_CFLAGS :=  -fPIE -DPIE -g -D_GNU_SOURCE -fno-strict-aliasing 

LOCAL_SHARED_LIBRARIES := libc libcutils
LOCAL_LDLIBS := -llog

include $(BUILD_EXECUTABLE)

# audit-reader utility build
include $(CLEAR_VARS)

LOCAL_MODULE := audit-reader
LOCAL_MODULE_TAGS := eng
LOCAL_SRC_FILES:= src/audit-reader.c
LOCAL_CFLAGS :=  -fPIE -DPIE -g -D_GNU_SOURCE -fno-strict-aliasing 
LOCAL_SHARED_LIBRARIES := libc libcutils
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog

include $(BUILD_EXECUTABLE)

# Start copying configuration files
include $(CLEAR_VARS)
LOCAL_MODULE := auditd.conf
LOCAL_MODULE_TAGS := eng
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(ETC_DIR)
LOCAL_SRC_FILES := etc/auditd.conf
include $(BUILD_PREBUILT)


