#
# 2020/7/21
#
# Created by Allen.YY
#

include ../../toolchain.make

obj_destination_path = ../../obj
ASFLAGS += -mapcs-32 -march=armv7-a
CFLAGS  += -march=armv7-a -mtune=cortex-a8

# -O2是gcc會自動優化
#CFLAGS  += -march=armv7-a -mtune=cortex-a8 -O2
#CFLAGS = -mcpu=cortex-a8 -marm -Wall -O2 -nostdlib -nostartfiles -ffreestanding -fstack-usage -Wstack-usage=256


c_obj=main.o debug.o task.o interrupt.o syscall.o mem.o

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













