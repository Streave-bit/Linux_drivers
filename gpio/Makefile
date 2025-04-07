obj-m += gpio.o

CPPCHECK = cppcheck
FORMAT = clang-format

KERNEL_DIR = /lib/modules/$(shell uname -r)/build

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean

cppcheck:
	@$(CPPCHECK) --quiet \
		--enable=all \
		--error-exitcode=1 \
		--suppress=missingIncludeSystem \
		gpio.c
format:
	@$(FORMAT) -i gpio.c
