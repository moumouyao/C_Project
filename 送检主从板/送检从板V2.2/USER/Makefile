KERNEL_DIR=/usr/src/linux-headers-5.15.0-94-generic
######################
CURDIR=$(shell pwd)

obj-m := hello.o

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(CURDIR) modules

.PHONE:clean

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(CURDIR) clean
