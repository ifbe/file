void vt100_create(void*);
void vt100_delete();




void lib2d_read()
{
}
void lib2d_write()
{
}
void lib2d_list()
{
}
void lib2d_choose()
{
}
void lib2d_stop()
{
}
void lib2d_start()
{
}
void lib2d_delete()
{
	vt100_delete();
}
void lib2d_create(void* base)
{
	vt100_create(base);
}
