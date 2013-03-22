obj-m := pcie.o

KDIR := /lib/modules/$(shell uname -r)/build
#KDIR := /home/craftor/flareGet/Compressed/linux-2.6.20/
PWD := $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean

