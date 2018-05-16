ifeq ($(CONFIG_USE_OF),y)
DTB_NAME := $(subst $\",,$(CONFIG_BUILD_X86_APPENDED_DTB_IMAGE_NAME))
ifneq ($(DTB_NAME),)
dtb-y += $(DTB_NAME).dtb
endif
endif

dtstree	:= $(srctree)/$(src)

targets += dtbs dtbs_install
targets += $(dtb-y)
always		:= $(dtb-y)
clean-files	:= *.dtb
