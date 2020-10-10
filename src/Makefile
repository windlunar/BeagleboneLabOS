#
# 2020/7/21
#
# Created by Allen.YY
# EE ,NCHU
#

include ../toolchain.make

obj_destination_path = ../obj
ASFLAGS += -mapcs-32 -march=armv7-a
CFLAGS  += -march=armv7-a -mtune=cortex-a8 -O2


.PHONY : all
all: asm_src c_src

.PHONY : asm_src
asm_src:
	$(toolchain)-as  start.s -o $(obj_destination_path)/start.o
	$(toolchain)-as  syscall.s -o $(obj_destination_path)/syscall.o
	$(toolchain)-as  context-switch.s -o $(obj_destination_path)/context-switch.o


.PHONY : c_src
c_src:
	$(toolchain)-gcc  -c main.c -o $(obj_destination_path)/main.o
	$(toolchain)-gcc  -c usr_led.c -o $(obj_destination_path)/usr_led.o
	$(toolchain)-gcc  -c uart.c -o $(obj_destination_path)/uart.o
	$(toolchain)-gcc  -c print.c -o $(obj_destination_path)/print.o

#Clean objects
.PHONY : clean
clean :
	-rm $(obj_destination_path)/$(src_objects)













