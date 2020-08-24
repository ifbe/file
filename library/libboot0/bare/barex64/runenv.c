void initgdt();
void initidt();
void init8259();	//interrupt controller
void init825x();	//timer.pit
void initrtc(); 	//timer.rtc




void freerunenv()
{
}
void initrunenv()
{
	asm("cli");
	initgdt();
	initidt();

	init8259();
	//init825x();
	initrtc();
	asm("sti");

	asm("int3");
	asm("int $0x80");
}