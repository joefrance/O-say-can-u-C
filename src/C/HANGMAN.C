#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <dos.h>

#define ESC			27
#define CTRL		0
#define F1			59
#define MAXWORDS 	200
#define MAXCHARS 	17
#define RETURN    0x0d
#define LINEFEED	0x0a
#define UNUSED		0
#define USED		1
#define SPACE		0x20
#define DASH      0x2d
#define TRUE		0
#define FALSE		1
#define FIRSTCHARACTER	0
#define LETTERROW	22

char visibleword[ MAXCHARS + 1];
char scrambledword[ MAXCHARS + 1 ];
char secretword[ MAXWORDS ][ MAXCHARS + 1 ];
char wordlog[ MAXWORDS ], letterlog[ MAXCHARS ];
char *upperpointer;
int bodypart, lettersfound, letterstoadd, numberofwords, numberofletters, i;
int key, numberofpeices = 17, remainder, lettersmissed;
clock_t t1;
int letterindex, totalwords;
char word[ MAXCHARS + 1 ];

FILE *wordfile;

struct shapefile	{
						int xpos, ypos;
						char shape;
						};

struct shapefile outlaw[] =	{	{ 6, 4, 0xc1 },
											{ 7, 4, 0xbf },
											{ 7, 5, 0xd9 },
											{ 6, 5, 0xc2 },
											{ 5, 5, 0xc0 },
											{ 5, 4, 0xda },
											{ 6, 6, 0xc5 },
											{ 5, 6, 0xda },
											{ 7, 6, 0xbf },
											{ 6, 7, 0xb3 },
											{ 5, 7, 0xb3 },
											{ 7, 7, 0xb3 },
											{ 6, 8, 0xc1 },
											{ 5, 8, 0xda },
											{ 7, 8, 0xbf },
											{ 5, 9, 0xb3 },
											{ 7, 9, 0xb3 },
											{ 0, 0, '~'  }	};

struct shapefile gallows[] =	{	{ 1, 12, 0xc4 },
											{ 2, 12, 0xc1 },
											{ 3, 12, 0xc4 },
											{ 2, 11, 0xb3 },
											{ 2, 10, 0xb3 },
											{ 2, 9, 0xb3 },
											{ 2, 8, 0xb3 },
											{ 2, 7, 0xb3 },
											{ 2, 6, 0xb3 },
											{ 2, 5, 0xb3 },
											{ 2, 4, 0xb3 },
											{ 2, 3, 0xb3 },
											{ 2, 2, 0xda },
											{ 3, 2, 0xc4 },
											{ 4, 2, 0xc4 },
											{ 5, 2, 0xc4 },
											{ 6, 2, 0xbf },
											{ 6, 3, 0xb3 },
											{ 0,  0, '~'  } };

main( int argc, char *argv[] )
{
_setcursortype( _NOCURSOR );
clrscr();
randomize();

if( !argv[ 1 ] )
	{
	printf("\nPlease include a Word File with the command.\n");
	printf("Example: HANGMAN WORDFILE\n");
	cleanup();
	}
else
	{
	if( ( wordfile = fopen( argv[ 1 ], "rb" ) ) == NULL )
		{
		printf("\nCouldn't open file: %s.\n", argv[ 1 ] );
		cleanup();
		}
	}

if( ( numberofwords = readwords() ) == 0 )
	{
	printf("Couldn't find any words to read!");
	cleanup();
	}

clearwordlog();

repeatloop:
	anywordsleft();

	bodypart = 0;
	lettersfound = 0;
	lettersmissed = 0;
	clearletterlog();
	clearrow( LETTERROW );

	clrscr();
	gotoxy( 25, 1 );
	printf("Hit F1 for a hint. Hints cost one wrong letter.");
	buildgallows();

	strcpy( visibleword, secretword[ pickrandomword() ] );
	scramble( visibleword );

	printxy( numberofletters, 1, 16, SPACE );
	numberofletters = strlen( visibleword );
	printxy( numberofletters, 1, 16, DASH );

	letterstoadd = numberofpeices / numberofletters;
	remainder = numberofpeices % numberofletters;
	addtooutlaw( remainder );

	playloop:

		gotoxy( lettersfound + 1, 15 );
		key = getch();
		processkeys( key );

		if( isalpha( key ) )
			if( findletter( uppercase( (char) key ) ) )
				{
				lettersmissed++;
				addtooutlaw( letterstoadd );
				showletter( (char) key );
				}

		if( outlaw[ bodypart ].shape == '~' || lettersmissed > MAXCHARS )
			{
			gotoxy( 1, 19 );
			printf("You are dead!\n");
			printf("Press <Esc> to quit or any other key to play another word.");
			processkeys( getch() );
			printxy( 40, 1, 19, SPACE );
			printxy( 60, 1, 20, SPACE );
			goto repeatloop;
			}

		if( lettersfound == numberofletters )
			{
			gotoxy( 1, 19 );
			printf("Congratulations you solved the puzzle!\n");
			printf("Press <Esc> to quit or any other key to play another word.");
			processkeys( getch() );
			printxy( 40, 1, 19, SPACE );
			printxy( 40, 1, 20, SPACE );
			goto repeatloop;
			}

	goto playloop;

return 0;
} /* End of main() */

addtooutlaw( int numbertoadd )
{
int i;

for(i=0;i<numbertoadd;i++, bodypart++)
	{
	gotoxy( outlaw[ bodypart ].xpos, outlaw[ bodypart ].ypos );
	printf("%c", outlaw[ bodypart ].shape );
	}

return 0;
} /* End of addtooutlaw()  */

addwordtolist()
{
upperpointer = strupr( word );
strcpy( secretword[ totalwords++ ], upperpointer );
zerofill( word, MAXCHARS + 1 );
letterindex = 0;

return 0;
} /* End of addwordtolist() */

anywordsleft()
{
int wordsleft = USED, i;

for(i=0;i<numberofwords;i++)
	if( wordlog[ i ] == UNUSED )
		wordsleft = UNUSED;

if( wordsleft == USED )
	{
	clearrow( LETTERROW );
	gotoxy( 1, LETTERROW );
	printf("Very impressive. You finished the game.");
	cleanup();
	}

return 0;
} /* End of anywordsleft() */

buildgallows()
{
int i;

while( gallows[ i ].shape != '~' )
	{
	gotoxy( gallows[ i ].xpos, gallows[ i ].ypos );
	printf("%c", gallows[ i++ ].shape );
	}

return 0;
} /* End of buildgallows */

cleanup()
{
if( wordfile != NULL )
	fclose( wordfile );

_setcursortype( _NORMALCURSOR );
gotoxy( 1, 23 );
printf("Press any key to exit!");
getch();

clrscr();
printf("Please have a nice day!\n");
exit( 1 );

return 0;
} /* End of cleanup() */

clearletterlog()
{
int i;

for(i=0;i<MAXCHARS;i++)
	letterlog[ i ] = UNUSED;

return 0;
} /* End of clearletterlog() */

clearrow( int rowtoclear )
{
printxy( 79, 1, rowtoclear, SPACE );

return 0;
} /* End of clearline() */

clearwordlog()
{
int i;

for(i=0;i<MAXWORDS;i++)
	wordlog[ i ] = UNUSED;

return 0;
} /* End of clearwordlog() */

findletter( char guess )
{
char result = FALSE;
int i;

for(i=0;i<numberofletters;i++)
	if( visibleword[ i ] == guess && letterlog[ i ] == UNUSED )
		{
		printxy( 1, 1 + i, 15, guess );
		result = TRUE;
		letterlog[ i ] = USED;
		lettersfound++;
		}

return ( result );
} /* End of findletter() */

pickrandomword()
{
int wordchosen;

loop:
	wordchosen = random( numberofwords );

	if( wordlog[ wordchosen ] == UNUSED )
		wordlog[ wordchosen ] = USED;
	else
		goto loop;

return( wordchosen );
} /* End of pickrandomword() */

printxy( int numbertoprint, int xpos, int ypos, char symbol )
{
int i;

for(i=0;i<numbertoprint;i++)
	{
	gotoxy( xpos + i, ypos );
	printf("%c", symbol );
	}

return 0;
} /* End of printxy() */

processcontrolkey( int controlkey )
{
	switch( controlkey )
		{

		case F1:

			printxy( 79, 1, 17, SPACE );
			gotoxy( 1, 17 );
			printf("Hint: Unscramble these letters... %s", scrambledword );
			t1 = clock();
			while( ( clock() - t1 ) < 50 );
			printxy( 79, 1, 17, SPACE );
			addtooutlaw( letterstoadd );

		break;

		}
return 0;
}

processkeys( int keytolookat )
{
int CTRLKEY;
switch( keytolookat )
	{

	case CTRL:

		CTRLKEY = getch();
		processcontrolkey( CTRLKEY );

	break;

	case ESC:

		cleanup();

	break;

	}

return 0;
} /* End of processkeys() */

readwords()
{
char letter;

totalwords = 0;
zerofill( word, MAXCHARS + 1 );

while( !feof( wordfile ) )
	{
	letter = (char) fgetc( wordfile );

	if( isalpha( letter ) > 0 )
		{
		word[ letterindex ] = letter;
		if( letterindex < MAXCHARS )
			letterindex++;
		else
			addwordtolist();
		}
	else
		if( word[ FIRSTCHARACTER ] != UNUSED )
			addwordtolist();
	}

fclose( wordfile );

return( totalwords );
} /* End of readwords() */

scramble( char *wordtoscramble )
{
int i, randomletter, length;

length = strlen( wordtoscramble );
randomletter = random( length );
zerofill( scrambledword, MAXCHARS + 1 );

for(i=0;i<length;i++)
	{
	while( scrambledword[ randomletter ] != UNUSED )
		randomletter = random( length );
	scrambledword[ randomletter ] = wordtoscramble[ i ];
	}

return 0;
} /* End of scramble() */

showletter( char lettertoshow )
{
lettertoshow = uppercase( lettertoshow );
gotoxy( lettertoshow - 0x40, LETTERROW );
printf("%c", lettertoshow );

return 0;
}

uppercase( char lettertoraise )
{
if( lettertoraise > 0x60 && lettertoraise < 0x7b )
	lettertoraise -= 0x20;

return ( lettertoraise );
} /* End of uppercase() */

zerofill( char *stringtofill, int numbertofill )
{
int i;

for(i=0;i<numbertofill;i++)
	stringtofill[ i ] = 0;

return 0;
} /* End of zerofill() */