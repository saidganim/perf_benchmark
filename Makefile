BUILD := $(shell uname -r)
PWD := $(shell pwd)
KBUILD := /lib/modules/$(BUILD)/build

obj-m += bad_file.o 

build: bad_file.c
	$(MAKE) -C $(KBUILD) M=$(PWD) modules

clean:
	$(MAKE) -C $(KBUILD) M=$(PWD) clean
