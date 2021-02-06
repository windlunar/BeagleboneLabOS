ENTRY(_start)

MEMORY
{
    ram : ORIGIN = 0x82000000, LENGTH = 0x1bf40000
}

SECTIONS
{
	.text :
	{
		KEEP(*(.isr_vector))
		*(.text)
		*(.text.*)
		*(.rodata)
		_sromdev = .;
		_eromdev = .;
		_sidata = .;
	} >ram

	.data : AT(_sidata)
	{
		_sdata = .;
		*(.data)
		*(.data*)
		_edata = .;
	} >ram

	.bss :
	{
		_sbss = .;
		*(.bss)
		_ebss = .;
	} >ram

	_end = . + 4 ;

	_estack = ORIGIN(ram) + LENGTH(ram);
}
