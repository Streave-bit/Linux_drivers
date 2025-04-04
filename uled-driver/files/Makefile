obj-m += uled_driver.o

all:
	make -C $(KERNEL_SRC) M=$(PWD) modules

clean:
	make -C $(KERNEL_SRC) M=$(PWD) clean

	