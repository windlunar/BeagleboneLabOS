#
# 2020/7/21
#
# Created by Allen.YY
# EE ,NCHU
#

include toolchain.make

obj_destination_path = ./obj
bin_destination_path = ./bin

include src_obj_list.make

root_dir = $(shell pwd)

#Build
#all
BUILD:
	make -C ./src
	make -C ./obj
	
	@echo
	@echo Build sucessfully!

copy:
	cp ./bin/$(bin_name) ../bbb_boot/

#Clean objects and bin
.PHONY : clean
clean :
	-rm $(bin_destination_path)/$(bin_name) 
	-rm $(obj_destination_path)/*.o












