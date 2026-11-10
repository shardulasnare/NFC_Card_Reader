/*
 * syscalls.c
 *
 *  Minimal newlib syscall stubs (no file/console I/O on this target).
 */

int _close(int file)
{
	(void)file;
	return -1;
}

int _lseek(int file, int ptr, int dir)
{
	(void)file;
	(void)ptr;
	(void)dir;
	return 0;
}

int _read(int file, char *ptr, int len)
{
	(void)file;
	(void)ptr;
	(void)len;
	return 0;
}

int _write(int file, char *ptr, int len)
{
	(void)file;
	(void)ptr;
	return len;
}
