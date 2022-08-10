#include <stdlib.h>

#define HIGHLIGHT 219
#define DEAD -1

#define LEFTCURSOR 75
#define RIGHTCURSOR 77
#define UPCURSOR 72
#define DOWNCURSOR 80

int start[3] = { 1980, 2000, 2020 }, save[3][2000];
int direction[4] = { 160, -2, -160, 2 };
int temp, currdir[3];
int count[3],tempcount[3], loop, loopend[3] = { 0, 0, 0 };
int ALLDEAD = 0;
char PEEKHIGH;
unsigned CARDBASE = 0xb000;
int mydir = 2;
int mystart = 3000;
char mykey;


main()
{
currdir[0] = random( 4 );
currdir[1] = random( 4 );
currdir[2] = random( 4 );

clrscr();
drawborder();
PEEKHIGH = peekb( CARDBASE, 0 );
loop3:
while( !kbhit() )
	{
	if( loopend[loop] == DEAD )
		goto incloop;

	save[loop][count[loop]++] = start[loop];
	pokeb( CARDBASE, (unsigned) start[loop], HIGHLIGHT );
	pokeb( CARDBASE, (unsigned) mystart, (char) 90 );

	if( (char) peekb( CARDBASE, (unsigned)(mystart + direction[mydir]) ) != (char) 32)
		{
		clrscr();
		printf("I win!");
		getch();
		exit( 1 );
		}
	else
		mystart += direction[mydir];


	if( (char) peekb( CARDBASE, (unsigned)(start[loop] + direction[currdir[loop]]) ) == (char) 32)
		start[loop] += direction[currdir[loop]];
	else
		{
		currdir[loop] = 0;
		loop12:
		if( (char) peekb( CARDBASE, (unsigned)(start[loop] + direction[currdir[loop]]) ) == (char) 32)
			{
			start[loop] += direction[currdir[loop]];
			goto getouttahere;
			}
			else
				{
				if( currdir[loop] < 4 )
					{
					currdir[loop]++;
					goto loop12;
					}
				else
					{
					for(tempcount[loop]=0;tempcount[loop]<count[loop];tempcount[loop]++)
						pokeb( CARDBASE, (unsigned) save[loop][tempcount[loop]], (char)32 );
					loopend[loop] = (int)DEAD;
					ALLDEAD++;
					}
				}
		}
getouttahere:
delay( 100 );

incloop:
loop++;
if( loop > 2 )
	loop=0;
if ( ALLDEAD >= 3 )
	{
	clrscr();
	printf("You win!");
	exit ( 1 );
	}
  }

if ( ( mykey = getch() )== 0 )
	{
	mykey = getch();

	switch( mykey )
		{
		case UPCURSOR:
			mydir = 2;
		break;

		case DOWNCURSOR:
			mydir = 0;
		break;

		case LEFTCURSOR:
			mydir = 1;
		break;

		case RIGHTCURSOR:
			mydir = 3;
		break;

		}

	goto loop3;

	}
else
	exit( 1 );

}
drawborder()
{
 unsigned int border;

 for( border = 0; border < 160; border += 2 )
	{
	pokeb( CARDBASE, border, HIGHLIGHT );
	pokeb( CARDBASE, border + 3680, HIGHLIGHT );
	}

 for( border = 0; border < 3682; border += 160 )
	{
	pokeb( CARDBASE, border, HIGHLIGHT );
	pokeb( CARDBASE, border + 158, HIGHLIGHT );
	}



return 0;
}