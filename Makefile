ifneq ($(KERNELRELEASE),)

moddir := $(M)/..

obj-m := hello1.o hello2.o

ccflags-y := -I$(moddir)/inc

else

KDIR ?= /lib/modules/$(shell uname -r)/build
SRCDIR := $(shell pwd)
BUILDDIR := $(SRCDIR)/build

default:
	mkdir -p $(BUILDDIR)
	$(MAKE) -C $(KDIR) M=$(BUILDDIR) src=$(SRCDIR) modules

clean:
	$(MAKE) -C $(KDIR) M=$(BUILDDIR) clean
	rm -rf $(BUILDDIR)

endif
