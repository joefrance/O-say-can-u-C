#include <bios.h>
#include <conio.h>

#define COM1       0
#define DATA_READY 0x100
#define TRUE       1
#define FALSE      0

#define PARM 0
#define XMIT 1
#define RECV 2
#define STAT 3

#define SETTINGS ( 0xa0 | 0x18 | 0x00 | 0x02)

int main(void)
{
   int in, out, status, DONE = FALSE;

	bioscom( PARM, SETTINGS, COM1);
	cprintf("... BIOSCOM [ESC] to exit ...\n");
	while (!DONE)
	{
		bioscom(XMIT, 0, COM1);
		status = bioscom(STAT, 0, COM1);
		if (status & DATA_READY)
			if ((out = bioscom(RECV, 0, COM1) & 0x7F) != 0)
				{
				putch(out);
				}
         if (kbhit())
         {
            if ((in = getch()) == '\x1B')
               DONE = TRUE;
				/* bioscom(XMIT, in, COM1); */
			}
	}
	return 0;
}
