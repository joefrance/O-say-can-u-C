#include <stdio.h>
#include <conio.h>
#include <dir.h>
#include <dos.h>
#include <alloc.h>
#include <mem.h>

char workdirectory[ MAXPATH ];

#define VIDEO 0x10

union REGS regs;

int j, done;
int sendout;
unsigned CARDBASE = 0xb000;
int HIGHLIGHT = 112;
struct ffblk ffblk;

#define LEFTCURSOR 75
#define RIGHTCURSOR 77
#define UPCURSOR 72
#define DOWNCURSOR 80
#define MAXENTRIES 200
#define SEND 1
#define NOSEND 0
#define LINEFEED 10

enum FUNCTION_KEYS	{
							F1 = 59,
							F2,
							F3,
							F4,
							F5,
							F6,
							F7,
							F8,
							F9,
							F10
							};

int cursorx = 3;
int cursorlx = 3;
int cursory = 4;
int cursorly = 3;
int fileindex = 1, filepointer = -1;
size_t structsize;
FILE *fp, *comp;

struct directorynames 	{
								char name[13];
								int  attrib;
								};
struct directorynames files[ MAXENTRIES ];
main()
{

_setcursortype( _NOCURSOR );
setscreencolor( BLUE );
drawscreen();

gotoxy( 1,4 );
/* for (j=4;j<24;j++)
	{
	gotoxy( 3,j );
	printf("--------------------------------------------------------------------------");
	} */

movecursor();

for (;;)
{

while( !kbhit() );
	if ( (j = getch()) == 27 )
		{
		setdisk( 2 );
		_setcursortype( _NORMALCURSOR );
		clrscr();
		exit(1);
		}
	if( j == 0 )
		{
		j = getch();
		processcntrlkeys();
		}
	else
		{
		}
}

}

processcntrlkeys()
{
	switch( j )
		{

		case F1:
			displaydirectory();
			break;

		case F2:
			getcurrentdirectory();
			break;

		case F3:
			changedirectory();
			break;

		case F4:
			sendout = NOSEND;
			openfile( files[ fileindex + filepointer ].name );
			break;

		case F5:
			gotoxy( 1,1 );
			setdisk( 0 );
			drawscreen();
		break;

		case F6:
			gotoxy( 1,1 );
			setdisk( 1 );
			drawscreen();
		break;

		case F7:
			gotoxy( 1,1 );
			setdisk( 2 );
			drawscreen();
		break;

		case F8:
			gotoxy( 1,1 );
			setdisk( 3 );
			drawscreen();
		break;

		case F9:
			gotoxy( 1,1 );
			setdisk( 4 );
			drawscreen();
		break;

		case F10:
			sendout = SEND;
			openfile( files[ fileindex + filepointer ].name );
			sendout = NOSEND;
		break;

		case LEFTCURSOR:
			if ( (cursorx - 15) > 2 )
				{
				fileindex -=20;
				cursorx -= 15;
				movecursor();
				}
			break;

		case RIGHTCURSOR:
			if ( (cursorx + 15) < 64 )
				{
				fileindex += 20;
				cursorx += 15;
				movecursor();
				}
			break;

		case UPCURSOR:
			if ( (cursory - 1) > 3 )
				{
				fileindex --;
				cursory --;
				movecursor();
				}
			break;

		case DOWNCURSOR:
			if ( (cursory + 1) < 24 )
				{
				fileindex++;
				cursory ++;
				movecursor();
				}
			break;

		default:
			printf("Uknown control key pressed\n");

	}

}

movecursor ()
{
	gotoxy( 2,2 );
	printf("    ");
	gotoxy( 2,2 );
	printf("%i", fileindex);
	gotoxy( cursorx, cursory );
	printf(">");
	gotoxy( cursorx + 14, cursory );
	printf("<");
	gotoxy( cursorlx, cursorly );
	printf(" ");
	gotoxy( cursorlx + 14, cursorly );
	printf(" ");
	gotoxy( 33, 2 );
	printf("               ");
	cursorlx = cursorx;
	cursorly = cursory;
	gotoxy( ( 80 - strlen( files[ fileindex + filepointer ].name ) ) / 2, 2 );
	printf("%s",files[ fileindex + filepointer ].name );
}

drawscreen()
{
clrscr();

getcurrentdirectory();
displaydirectory();

}

getcurrentdirectory()
{
	int index;

	drawborder();

	getcwd( workdirectory, MAXPATH );
	gotoxy( ( 80 - strlen( workdirectory ) ) / 2, 1 );
	printf("%s",workdirectory);

	index = 0;
	done = findfirst( "*.*", &ffblk, FA_DIREC );
	while( !done )
		{
		strcpy( files[ index ].name, ffblk.ff_name );
		files[ index ].attrib = ffblk.ff_attrib;
		done = findnext( &ffblk );
		index++;
		}
	while( index < 200 )
		{
		strcpy( files[ index ].name, "             " );
		files[ index ].attrib = 0;
		index++;
		}


}

drawborder()
{
 unsigned int border;

 for( border = 1; border < 160; border += 2 )
	{
	poke( CARDBASE, border, HIGHLIGHT );
	poke( CARDBASE, border + 3680, HIGHLIGHT );
	}

 for( border = 1; border < 3682; border += 160 )
	{
	poke( CARDBASE, border, HIGHLIGHT );
	poke( CARDBASE, border + 158, HIGHLIGHT );
	}


}

displaydirectory()
{
	int index;
	int row, column;

	for( index=0, fileindex = 0; index<100; index++ )
		{
		column = fileindex / 20;
		row = fileindex % 20;
		gotoxy( 4 + (column * 15), 4 + row );
		printf("%s", files[ index ].name );
		if( fileindex < 100)
			fileindex++;
		else
			goto exitloop;
		}
exitloop:
	fileindex = 1;
	cursorx = 3;
	cursory = 4;
	movecursor();
}

changedirectory()
{
	int i;

	if( files[ fileindex + filepointer ].attrib == FA_DIREC )
		{
		if( workdirectory[ strlen( workdirectory ) - 1 ] != '\\' )
			strcat( workdirectory, "\\" );
		strcat( workdirectory, files[ fileindex + filepointer ].name );
		gotoxy( 2,2 );
		printf("%s", workdirectory );
		if ( chdir( workdirectory ) == -1 )
			{
			gotoxy( 2,3 );
			printf("Error: Couldn't open directory. Press a key.");
			getch();
			gotoxy( 2,3 );
			printf("                                            ");
			}
		else
			{
			for( i = 4;i<24;i++)
				{
				gotoxy( 2, i );
				printf("                                                                            ");
				}
			getcurrentdirectory();
			displaydirectory();
			}
		}
}

openfile( char *filename )
{
	int lines;
	char buffer;
	if ( sendout == SEND )
	{
	 if ( ( comp = fopen( "com1", "wt" ) ) == NULL )
		{
		gotoxy( 2,3 );
		printf("Error: Couldn't open COM1: port." );
		getch();
		gotoxy( 2,3 );
		printf("                                                                              ");
		}
	 else
		lines = 21;
	 }

	if ( ( fp = fopen( filename, "rb" ) ) == NULL )
		{
		gotoxy( 2,3 );
		printf("Error: Couldn't open file %s", filename );
		getch();
		gotoxy( 2,3 );
		printf("                                                                              ");
		}
	else
		{
		clrscr();

		transmitloop:
			buffer = (char) fgetc( fp );
			if ( buffer == 13 || buffer == 10 )
				lines++;

			if ( lines < 20 )
				{
				printf("%c",buffer);
				}

			else
				{
				if( sendout == NOSEND )
					{
					lines = 0;
					printf("\nPress any key to continue or hit (Esc) to exit.\n");
					if ( getch() == 27 )
						goto getoutofhere;
					clrscr();
					}
				}
			if ( sendout == SEND && comp != NULL )
				{
				if ( buffer != (char) LINEFEED )	/* strip LF charactor */
					fputc( (int) buffer, comp );
				printf("%c", buffer);
				}
			if( !feof( fp ) )
				goto transmitloop;
			else
				goto getoutofhere;

		printf("Press a key.\n");
		getch();
		}
getoutofhere:
if ( comp != NULL )
	fclose( comp );
if ( fp != NULL )
	fclose( fp );
clrscr();
drawscreen();
}

directorytoterminal()
{

int i;

if ( comp == NULL )
	if( (comp = fopen( "com1", "wb" )) != NULL )
		{
		for( i=0;i<10;i++)
			{

			}
		}

if( comp != NULL )
	fclose( comp );

}

setscreencolor( color )
char color;
{
	regs.h.ah = 0x0b;
	regs.h.bh = 0x00;
	regs.h.bl = color;
	int86(VIDEO, &regs, &regs);

	return 0;
}