#
# 2020/7/21
#
# Created by Allen.YY
# EE ,NCHU
#

include ../toolchain.make

include ../src_obj_list.make
obj_destination_path = ../obj

all: start.o main.o usr_led.o uart.o print.o

#Build lib obj exampke
###lib.o : lib.c
###	$(toolchain) -c lib.c -o $(obj_destination_path)/lib.o
start.o : start.s
	$(toolchain)-as start.s -o $(obj_destination_path)/start.o
main.o : main.c
	$(toolchain)-gcc -c main.c -o $(obj_destination_path)/main.o
	
usr_led.o : usr_led.c
	$(toolchain)-gcc -c usr_led.c -o $(obj_destination_path)/usr_led.o

uart.o : uart.c
	$(toolchain)-gcc -c uart.c -o $(obj_destination_path)/uart.o

print.o : print.c
	$(toolchain)-gcc -c print.c -o $(obj_destination_path)/print.o

#Clean objects
.PHONY : clean
clean :
	-rm $(obj_destination_path)/$(src_objects)













