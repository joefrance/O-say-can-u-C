#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dir.h>
#include <dos.h>
#include <alloc.h>
#include <mem.h>
#include <time.h>
#include <process.h>

#define MONOCHROME '1'
#define CGA '2'
#define EGAVGA '3'

char workdirectory[ MAXPATH ];

int j, done;
int sendout;
int inpt;
unsigned CARDBASE = 0xb800;
int whichcard;
char HIGHLIGHT = 112;
char NORMALTEXT;
char *str = NULL;
struct ffblk ffblk;

#define LEFTCURSOR 75
#define RIGHTCURSOR 77
#define UPCURSOR 72
#define DOWNCURSOR 80
#define MAXENTRIES 200
#define SEND 1
#define NOSEND 0

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
int termx = 0;
int cursorx = 3;
int cursorlx = 3;
int cursory = 4;
int cursorly = 3;
int result;
unsigned cursorpos, lcursorpos = 487;
clock_t t1, t2;
int fileindex = 1, filepointer = -1;
int originaldisk;
char originaldirectory[MAXPATH];
size_t structsize;
FILE *fp, *comp;

char savetext[120];

struct directorynames 	{
								char name[13];
								int  attrib;
								};
struct directorynames files[ MAXENTRIES ];
main()
{
clrscr();

if( (str = calloc( 32767, sizeof(char) ) ) == NULL )
	{
	printf("Couldn't allocate memory for samples.\n");
	exit( 1 );
	}

printf("1. Monochrome\n");
printf("2. CGA\n");
printf("3. EGA/VGA\n");
printf("Choose your video adapter: ");

invalidentry:
whichcard = getch();
switch (whichcard)
	{

	case MONOCHROME:
		CARDBASE = (unsigned)0xb000;
		break;

	case CGA:
		CARDBASE = (unsigned)0xb800;
		break;

	case EGAVGA:
		CARDBASE = (unsigned)0xa000;
		break;
	}

clrscr();
NORMALTEXT = peekb( CARDBASE, (unsigned) 1 );

getoriginaldirectory();
_setcursortype( _NOCURSOR );
drawscreen();
movecursor();
t1 = clock();

for (;;)
{

while( !kbhit() )
	{
	t2 = clock();
	if( (t2 - t1) > 546 )
		{
		clrscr();
		while( !kbhit() )
			{
			clrscr();
			gotoxy ( (int)1 + random( (int)32 ), (int)1 + random( (int)26 ) );
			printf("Preventing screen-burn. Press any key.");
			delay( 650 );
			}
		getch();
		drawscreen();
		movecursor();
		t1 = clock();
		}

	}

t1 = clock();

	if ( (j = getch()) == 27 )
		{
		restoreoriginaldirectory();
		clrscr();
		_setcursortype( _NORMALCURSOR );
		if( str != NULL )
			free( str );
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
			if( (result = spawnl( P_WAIT, files[ fileindex + filepointer ].name, NULL ) ) == -1 )
				displayerror("Couldn't execute file.");
			else
				{
				clrscr();
				drawscreen();
				hidecursor();
				showcursor();
				}
			break;

		case F2:
			directorytoterminal();
			break;

		case F3:
			changedirectory();
			break;

		case F4:
			sendout = NOSEND;
			openfile( files[ fileindex + filepointer ].name );
			break;

		case F5:
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
			playsamples( files[ fileindex + filepointer ].name );
			drawscreen();
		break;

		case F10:
			sendout = SEND;
			openfile( files[ fileindex + filepointer ].name );
			sendout = NOSEND;
		break;

		case LEFTCURSOR:
			if ( (fileindex - 20) > 0 )
				{
				fileindex -=20;
				movecursor();
				}
			break;

		case RIGHTCURSOR:
			if ( (fileindex + 20) < 101 )
				{
				fileindex += 20;
				movecursor();
				}
			break;

		case UPCURSOR:
			if ( (fileindex - 1) > 0 && fileindex != 1 )
				{
				fileindex --;
				movecursor();
				}
			else
				moveup();
			break;

		case DOWNCURSOR:
			if ( (fileindex + 1) < 101 && fileindex != 100 )
				{
				fileindex++;
				movecursor();
				}
			else
				movedown();
			break;

		default:
			displayerror("Uknown control key pressed.");

	}

return 0;
}

movecursor ()
{
	gotoxy( 2,2 );
	printf("    ");
	gotoxy( 2,2 );
	printf("%i", fileindex);
	showcursor();
	hidecursor();
	gotoxy( 33, 2 );
	printf("                    ");
	gotoxy( ( 80 - strlen( files[ fileindex + filepointer ].name ) ) / 2, 2 );
	printf("%s",files[ fileindex + filepointer ].name );
	if ( files[ fileindex + filepointer ].attrib == FA_DIREC )
		printf(" <DIR>");

return 0;
}

drawscreen()
{
clrscr();
getcurrentdirectory();
displaydirectory();
hidecursor();
showcursor();

gotoxy( 1,25 );
printf("<Esc>=EXIT  F1=RUN  F2=DIR  F3=CHDIR  F4=VIEW  F5-F8=A:-D:  F9=TRANS  F10=RECV");

return 0;
}

getcurrentdirectory()
{
	int index;

	drawborder();

	getcwd( workdirectory, MAXPATH );
	gotoxy( ( 80 - strlen( workdirectory ) ) / 2, 1 );
	printf("%s",workdirectory);

	index = 0;

	while( index < 200 )
		{
		strcpy( files[ index ].name, "             " );
		files[ index ].attrib = 0;
		index++;
		}

	index = 0;

	done = findfirst( "*.*", &ffblk, FA_DIREC );
	while( !done )
		{
		strcpy( files[ index ].name, ffblk.ff_name );
		files[ index ].attrib = ffblk.ff_attrib;
		done = findnext( &ffblk );
		index++;
		}

return 0;
}

drawborder()
{
 unsigned int border;

 for( border = 1; border < 160; border += 2 )
	{
	pokeb( CARDBASE, border, HIGHLIGHT );
	pokeb( CARDBASE, border + 3680, HIGHLIGHT );
	}

 for( border = 1; border < 3682; border += 160 )
	{
	pokeb( CARDBASE, border, HIGHLIGHT );
	pokeb( CARDBASE, border + 158, HIGHLIGHT );
	}


return 0;
}

displaydirectory()
{
	int index;
	int row, column;


	for( index=0, fileindex = 1; index<100; index++ )
		{
		column = index / 20;
		row = index % 20;
		gotoxy( 4 + (column * 15), 4 + row );
		printf( "            ");
		gotoxy( 4 + (column * 15), 4 + row );
		printf("%s", files[ fileindex + filepointer ].name );

		if( fileindex < 100)
			fileindex++;
		else
			goto exitloop;
		}
exitloop:
	fileindex = 1;
	cursorx = 3;
	cursory = 4;
	showcursor();

return 0;
}

changedirectory()
{
	int i;
	unsigned offset;

	hidecursor();

	if( files[ fileindex + filepointer ].attrib == FA_DIREC )
		{
		if( workdirectory[ strlen( workdirectory ) - 1 ] != '\\' )
			strcat( workdirectory, "\\" );
		strcat( workdirectory, files[ fileindex + filepointer ].name );
		gotoxy( 2,2 );
		printf("%s", workdirectory );
		if ( chdir( workdirectory ) == -1 )
			displayerror( "Couldn't open directory" );
		else
			{
			cleararea( 2,2, 79,23 );
			getcurrentdirectory();
			displaydirectory();
			}
		}
hidecursor();
showcursor();

return 0;
}

playsamples( char *filename )
{
long strcnt = 0, endcnt;
int find;
char BODY[4];

clrscr();

if( ( fp = fopen( filename, "rb" ) ) == NULL )
	displayerror( "Couldn't open file");
else
	{
	while( !feof( fp ) )
		{
		find = fgetc( fp );
		printf("%c", (char)find );
		if( getch() == 27 )
			goto skipit;
		}
	skipit:

	while( !feof( fp ) )
		str[strcnt++] = (char)fgetc( fp );
playagain:
	endcnt = strcnt;
	outportb( 0x61, inportb( 0x61 ) & 0xfc );

	for(strcnt=0;strcnt<endcnt;strcnt++)
		{
		if( str[strcnt] > 0 )
			outportb( 0x61, inportb( 0x61 ) | 0x02 );
		else
			outportb( 0x61, inportb( 0x61 ) & 0xfc );
		}
	printf("Press any key to play again or <Esc> to return to menu.\n");
	if( getch() != 27 )
		goto playagain;
	fclose( fp );
	}
}

openfile( char *filename )
{
	int lines = 0;
	char buffer;
	if ( sendout == SEND )
	{
	 if ( ( comp = fopen( "com1", "wt" ) ) == NULL )
		displayerror( "Couldn't open COM1: port" );
	}

	if ( ( fp = fopen( filename, "rt" ) ) == NULL )
		displayerror( "Couldn't open file" );
	else
		{
		clrscr();
		if( sendout == SEND && comp != NULL )
			{
			clrscr();
			inittermscreen();
			}

		while ( !feof( fp ) )
			{
			buffer = (char) fgetc( fp );
			if ( buffer == 13 || buffer == 10 )
				lines++;

			if ( lines < 20 )
				{
				if ( sendout == SEND && comp != NULL )
					{
					fputct( buffer, comp );
					}
				else
					printf("%c",buffer);
				}

			else
				{
				lines = 0;
				if ( sendout == NOSEND )
					{
					printf("\n\nPress any key to continue or hit (Esc) to exit.\n");
					if ( getch() == 27 )
						goto getoutofhere;
					clrscr();
					}
				}
			}
		if ( sendout == NOSEND )
			{
			printf("\n\nPress a key.\n");
			getch();
			}
		}
getoutofhere:
if ( comp != NULL )
	fclose( comp );
if ( fp != NULL )
	fclose( fp );
clrscr();
drawscreen();
hidecursor();
showcursor();

return 0;
}

displayerror ( char *errormsg )
{
	beep();
	gotoxy( 2,3 );
	printf("Error:%s. Press any key.", errormsg);
	getch();
	gotoxy( 2,3 );
	printf("                                                                              ");

return 0;
}

beep()
{
	sound( 1440 );
	delay( 200 );
	nosound();

return 0;
}

moveup()
{
	int x;

	if ( (fileindex + filepointer) > 0 )
		{
		hidecursor();
		filepointer --;

		gettext( (int)4, (int)23, (int)62, (int)23, (char *)savetext );
		gotoxy( 2,23 );
		printf("                                                                              ");
		for( x=0; x<5; x++)
			{
			movetext((int)4 + ( (int)15 * x ), (int)4,
						(int)16 + ( (int)15 * x ), (int)22,
						(int)4 + ( (int)15 * x ), (int)5 );
			}

		puttext( (int)19, (int)4, (int)77, (int)4, (char *)savetext );
		gotoxy( 4,4 );
		printf("%s", files[ fileindex + filepointer ].name );

		showcursor();
		}

return 0;
}

movedown()
{
	int x;
	FILE *curious;


	if ( (fileindex + filepointer) < 200 )
		{
		hidecursor();

		filepointer ++;
		gettext( (int)19, (int)4, (int)77, (int)4, (char *)savetext );
		if(( curious = fopen ( "c:\\curious", "wb" ) ) != NULL )
			{
			fwrite( savetext, sizeof(savetext), (size_t) 1, curious );
			fclose( curious );
			}
		gotoxy( 2,4 );
		printf("                                                                              ");
		for( x=0; x<5; x++)
			{
			movetext((int)4 + ( (int)15 * x ), (int)5,
						(int)16 + ( (int)15 * x ), (int)23,
						(int)4 + ( (int)15 * x ), (int)4 );
			}
		puttext( (int)4, (int)23, (int)62, (int)23, (char *)savetext );

		gotoxy( 64,23 );
		printf("%s", files[ fileindex + filepointer ].name );

		showcursor();
		}

return 0;
}

showcursor()
{
	unsigned underline;
	cursorpos = (unsigned) ( 487 + ( (((fileindex - 1) / 20) * 30) +
												(((fileindex - 1) % 20) * 160) ) );

	for(underline=0; underline < 26; underline+=2)
		pokeb( CARDBASE, cursorpos + underline, HIGHLIGHT );

return 0;
}

hidecursor()
{
	unsigned underline;

	for(underline=0; underline < 26; underline+=2)
		pokeb( CARDBASE, lcursorpos + underline, NORMALTEXT );

	lcursorpos = cursorpos;
return 0;
}

getoriginaldirectory()
{
	originaldisk = getdisk();
	if( getcwd( originaldirectory, MAXPATH ) == NULL )
		{
		displayerror("Couldn't get original directory");
		exit( 1 );
		}

return 0;
}

restoreoriginaldirectory()
{
	setdisk( originaldisk );
	chdir( originaldirectory );

return 0;
}

directorytoterminal()
{

int i, j;
char number[3];
FILE *dcom1;

if( (dcom1 = fopen( "com1", "wb" )) == NULL )
	{
	displayerror( "Couldn't open COM1: port" );
	return 0;
	}
else
	{
	displayerror( "Printing dir to COM1: port" );
	clrscr();
	inittermscreen();
	gotoxy( 1,25 );
	j=0;
	for( i=0;i<60;i++)
		{
			strcpy( number, "   ");
			itoa( i, number, 10 );

			/* terminal emulation (See: terminalemulation( int input ) */

			fwritet( number, (size_t) 2, 1, dcom1 );
			fputct( (int)32, dcom1 );
			fputct( (int)32, dcom1 );
			fwritet( files[ i ].name, (size_t) 12,  1, dcom1 );
			fputct( (int)32, dcom1 );
			fputct( (int)32, dcom1 );
			/*Real terminal output*/

		 /*	fwrite( number, (size_t) 2, 1, dcom1 );
			fputc( (int)32, dcom1 );
			fwrite( files[ i ].name, (size_t) 12,  1, dcom1 );
			fputc( (int)32, dcom1 );
			*/
			if( j<3 )
				j++;
			else
				{
				fputct( (int)13, dcom1 );
				j=0;
				}
		}
	fputct( (int)13, dcom1 );
	fwritet( "PRESS A FILE NUMBER OR ANY OTHER KEY TO EXIT", 45, 1, dcom1 );
	fputct( (int)13, dcom1 );
	inpt = fgetnumt();
	printf(" inpt = %i", inpt);
	getch();

/*	fputc( (int)13, dcom1 );
	fwrite( "PRESS A FILE NUMBER OR ANY OTHER KEY TO EXIT", 45, 1, dcom1 );
	fputc( (int)13, dcom1 );
	fclose( dcom1 ); */

	}

if( dcom1 != NULL )
	fclose( dcom1 );

clrscr();
	drawscreen();
	hidecursor();
	showcursor();

return 0;
}

help()
{


return 0;
}

fgetnumt()
{
int inchr, cntinp;
char tt[2];

for (cntinp=0;cntinp<2;cntinp++)
	{
	jmph:
		inchr = getch();
		if(inchr < (int)'0' && inchr > (int)'9')
			goto jmph;
		else
			{
			printf("%c",(char)inchr);
			tt[cntinp] = (char)inchr;
			}
	}
inchr = atoi( tt );
return (inchr);
}

fputct( int output, FILE *device )
{
terminalemulation( output );

return 0;
}

fwritet( char *output, size_t sz, size_t qn, FILE *device )
{
size_t times;
int i;

for(times=0;times<qn; times++)
	for(i=0;i<sz;i++)
		terminalemulation( (int)output[i] );

return 0;
}

terminalemulation( int input )
{
int i, j, x, y;
unsigned scroll;

if( termx >= 158 || input == (int)13 || input == (int)10 )
	goto linefeed;

pokeb( CARDBASE, 3040 + termx, (char) input );
termx+=2;
goto skiplinefeed;

linefeed:
	termx = 0;
	for(scroll=160;scroll<320;scroll+=2)
		{
		pokeb( CARDBASE, scroll, 32 );
		pokeb( CARDBASE, scroll + 1, NORMALTEXT );
		}
	movetext( (int)1,(int)3, (int)80,(int)20,(int)1,(int)2 );
	for(scroll=3040;scroll<3200;scroll+=2)
		{
		pokeb( CARDBASE, scroll, 32 );
		pokeb( CARDBASE, scroll + 1, NORMALTEXT );
		}
skiplinefeed:

return 0;
}

inittermscreen()

{
unsigned i, j;

gotoxy( 1,1 );
printf("TERMINAL EMULATION");
gotoxy( 65,21 );
printf("      F1 =");
gotoxy( 65,22 );
printf("      LOAD");
gotoxy( 65,23 );
printf("      F2 =");
gotoxy( 65, 24 );
printf("      SAVE");

for(i=1;i<37;i+=2)
	pokeb( CARDBASE, i, HIGHLIGHT );

for(i=3327;i<3967;i+=160)
	for(j=0;j<31;j+=2)
		pokeb(CARDBASE, i + j, HIGHLIGHT );

return 0;
}

cleararea( int x1, int y1, int x2, int y2 )
{
unsigned xoffset, yoffset;

for( yoffset=(unsigned) ((y1 - 1) * 160); yoffset<(unsigned) y2; yoffset+=160)
	for( xoffset=(unsigned) ((x1 - 1) * 2); xoffset<(unsigned) x2; xoffset+=2)
		{
		pokeb( CARDBASE, xoffset + yoffset, (char) 32 );
		pokeb( CARDBASE, xoffset + yoffset + (unsigned)1, NORMALTEXT );
		}

return 0;
}