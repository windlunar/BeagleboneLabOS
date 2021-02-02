#
# 2020/7/21
#
# Created by Allen.YY
#

include toolchain.make

obj_destination_path = ./obj
bin_destination_path = ./bin

root_dir = $(shell pwd)

#Build
.PHONY : build
build:
	mkdir -p bin
	make -C ./src
	make -C ./obj
	
	@echo
	@echo Build sucessfully!


#Clean objects and bin
.PHONY : clean
clean :
	-rm $(bin_destination_path)/*.*
	-rm $(obj_destination_path)/*.o

.PHONY : clobj
clobj :
	-rm $(obj_destination_path)/*.o


.PHONY : clean_diasm
cldisasm :
	-rm $(bin_destination_path)/*.disasm
	#-rm ./*.disasm


.PHONY : disasm
disasm:
	$(toolchain)-objdump -D ./bin/bos > ./bin/bos.disasm
	#$(toolchain)-objdump -D ./u-boot > ./u-boot.disasm










