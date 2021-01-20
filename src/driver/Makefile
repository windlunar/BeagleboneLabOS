#
# 2020/7/21
#
# Created by Allen.YY
#

include ../../toolchain.make

obj_destination_path = ../../obj
CFLAGS  += -march=armv7-a -mtune=cortex-a8

c_obj=usr_led.o uart.o timer.o


.PHONY : all
all: $(c_obj) move


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













