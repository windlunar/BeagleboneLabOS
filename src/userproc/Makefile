#
# 2020/7/21
#
# Created by Allen.YY
#

include ../../toolchain.make

obj_destination_path = ../../obj
ASFLAGS += -mapcs-32 -march=armv7-a
CFLAGS  += -march=armv7-a -mtune=cortex-a8 -O2

c_obj=user_task.o


.PHONY : all
all: $(c_obj) move


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













