# for simple.c
# obj-m += simple.o

# for Q1.c 
# obj-m += Q1.o

#for Q2.c
obj-m += Q2.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean