#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <fcntl.h>
#include <string.h>

#define STX 2
#define ETX 3
#define XON 17
#define XOFF 19

#define SERIAL_IO 0x14
#define TIMEOUT 0x80

#define COM1 0x00
#define COM2 0x01
#define COM3 0x02
#define COM4 0x03

#define  B110 0x00
#define  B150 0x20
#define  B300 0x40
#define  B600 0x60
#define B1200 0x80
#define B2400 0xa0
#define B4800 0xc0
#define B9600 0xe0

#define NONE2 0x00
#define ODD   0x08
#define NONE  0x10
#define EVEN  0x18

#define STOP1 0x00
#define STOP2 0x40

#define FIVE  0x00
#define SIX   0x01
#define SEVEN 0x02
#define EIGHT 0x03

union REGS regs;
char linestatus, modemstatus, serialstatus, ERRORMASK = 0x80, rword;

char initcomport( rate, parity, stop, word, port )
char rate, parity, stop, word;
unsigned int port;
{
	regs.h.ah = 0x01;  /* initialize COM #port with rate|parity|stop|word */
	regs.h.al = rate | parity | stop | word ;
	printf("%2x\n",(unsigned char) regs.h.al );
	regs.x.dx = port;
	int86( SERIAL_IO, &regs, &regs);

	return( regs.h.ah );
}

char write1byte( character, port )
char character;
unsigned port;
{
	regs.h.ah = 0x01;
	regs.h.al = character;
	regs.x.dx = port;
	int86( SERIAL_IO, &regs, &regs );

	return ( regs.h.ah );
}

writestring( string, port )
char *string;
unsigned port;
{
char *ptr;
int i;

	for(i=0;i<(int)strlen( string )+1;i++)
		{
		ptr = strchr( string, i );
		write1byte( ptr-string, COM1 );
		}

}

char read1byte( port )
unsigned port;
{
	regs.h.ah = 0x02;
	regs.x.dx = port;
	int86( SERIAL_IO, &regs, &regs );
	linestatus = regs.h.ah;
	if( linestatus & ERRORMASK == TIMEOUT )
		printf("Timeout error on COM%1x: Data is invalid.\n", port + 1 );

	return ( regs.h.al );
}
void readcomstatus( port )
unsigned port;
{
 regs.h.ah = 0x03;
 regs.x.dx = port;
 int86( SERIAL_IO, &regs, &regs );
 linestatus = regs.h.ah;
 modemstatus = regs.h.al;

 return 0;
}

main()
{

	int c, i;
	FILE *handle, *handle2;

	if ( ( handle = fopen( "c:\\temp\\pulley2g.two", "rt") ) == NULL )
		{
		printf("Couldn't open file. Press a key.\n");
		getch();
		exit( 1 );
		}
	else
		printf("Opened file.\n");

		if( (serialstatus = (initcomport( B2400, EVEN, STOP1, SEVEN, COM1 ) & ERRORMASK )) == TIMEOUT )
			{
			printf("Error initializing COM port.");
			getch();
			exit( 1 );
			}
		else
			{
			/* printf("serialstatus = %2x\n", serialstatus);
			getch();
			printf("Writing to COM port.\n");
			write1byte( (char)'A', COM1 );
			getch();
			printf("Reading character from COM port.\n");
			while( (serialstatus = read1byte( COM1 ) ) == (char)1 )
				write1byte( (char)XON, COM1 );
			printf("%c = %2x\n Press any key to Xmit file.", serialstatus, serialstatus ); */
			printf("Press a key to Xmit file\n");
			getch();
			while ( !feof( handle ) )
				{
			  if( (c = fgetc( handle )) != (int)0x0A )
			  {
				if( (serialstatus = write1byte( (char)c, COM1 )) & ERRORMASK == TIMEOUT )
					{
					printf("Error writing to COM port. Press any key.\n");
					fclose( handle );
					getch();
					exit( 1 );
					}

				else
					{
					/*readcomstatus( COM1 );
					gotoxy( 1,24 );
					printf("line status = %2x, modem status = %2x\n", linestatus, modemstatus );*/
					printf("%c",(char)c);
					}

			  }
			  else
				{
					write1byte( (char)0x0d, COM1 );
					write1byte( (char)0x0a, COM1 );
				}
			}
					fclose( handle );
					write1byte( (char)0x0d, COM1 );
					write1byte( (char)0x0a, COM1 );
					write1byte( (char)0x00, COM1 );

		 }
		while( !kbhit() )
		{
		readcomstatus( COM1 );
		gotoxy( 1,25 );
		printf("line status = %2x, modem status = %2x\n", linestatus, modemstatus );
		}
		if( getch() == 27 )
			exit( 1 );


while(!kbhit())
	{
	readcomstatus( COM1 );
	if( linestatus  == 0x63 )
		{
		rword = read1byte( COM1 );
		printf("%c=%i", rword, (int) rword );
		}
	}

}