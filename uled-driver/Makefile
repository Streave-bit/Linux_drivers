obj-m += uled_driver.o

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
		uled_driver.c
format:
	@$(FORMAT) -i uled_driver.c


	