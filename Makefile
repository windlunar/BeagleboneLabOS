#
# 2020/7/21
#
# Created by Allen.YY
# EE ,NCHU
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

.PHONY : copy
copy:
	cp ./bin/*.elf ../bbb_boot/

#Clean objects and bin
.PHONY : clean
clean :
	-rm $(bin_destination_path)/*.elf $(bin_destination_path)/*.disasm
	-rm $(obj_destination_path)/*.o
	-rm ../bbb_boot/*.elf
	

.PHONY : disasm
disasm:
	$(toolchain)-objdump -D ./bin/beagos.elf > ./bin/beagos.elf.disasm
	$(toolchain)-objdump -D ./u-boot/u-boot > ./u-boot/u-boot.disasm










