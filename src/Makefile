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

c_obj=main.o usr_led.o uart.o print.o debug.o task.o user_task.o

asm_obj=start.o syscall.o context-switch.o

.PHONY : all
all: $(c_obj) $(asm_obj) move


%.o : %.s
	$(toolchain)-as $^ -o $@


%.o : %.c
	$(toolchain)-gcc -c $^ -o $@


.PHONY : move
move : 
	cp *.o $(obj_destination_path)
	rm -rf *.o	


#Clean objects
.PHONY : clean
clean :
	-rm $(obj_destination_path)/$(src_objects)













