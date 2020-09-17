#
# 2020/7/21
#
# Created by Allen.YY
# EE ,NCHU
#

include ../toolchain.make

include ../src_obj_list.make

total_objects = $(src_objects)
bin_destination_path = ../bin


#Build
#all objects
$(bin_name) : $(total_objects)
	$(toolchain)-ld $(total_objects) -T linker.ld -o $(bin_destination_path)/$(bin_name) 


#Clean objects and bin
.PHONY : clean
clean :
	-rm ./*.o
	-rm $(bin_destination_path)/$(bin_name)












