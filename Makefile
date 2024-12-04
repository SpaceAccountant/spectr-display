ifneq ($(KERNELRELEASE),)
	EXTRA_CFLAGS := -I$(SPECTR_IO)/src
	KBUILD_EXTRA_SYMBOLS := $(SPECTR_IO)/Module.symvers
	obj-m := spectr_display.o
	spectr_display-y := src/main.o src/ssd1351.o

else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm *.mod*
	rm src/*.o src/.*.cmd .*.cmd *.o *.ko Module.symvers modules.order

endif

