#
# 2020/7/21
#
# Created by Allen.YY
#

include ../../toolchain.make

obj_destination_path = ../../obj
CFLAGS  += -march=armv7-a -mtune=cortex-a8 -nostdlib
#CFLAGS  += -march=armv7-a -mtune=cortex-a8

c_obj=main.o debug.o task.o interrupt.o syscall.o memory.o file.o kprint.o mmu.o ipc.o
asm_obj=start.o syscall_asm.o exception.o task_asm.o

.PHONY : all
all: $(c_obj) $(asm_obj) move


%.o : %.s
	$(toolchain)-as $^ -o $@


%.o : %.c
	$(toolchain)-gcc $(CFLAGS) -c $^ -o $@


.PHONY : move
move : 
	cp *.o $(obj_destination_path)
	rm -rf *.o	
	rm -rf *.su


#Clean objects
.PHONY : clean
clean :
	-rm $(obj_destination_path)/$(src_objects)













