#include <stdlib.h>

unsigned CARDBASE = 0xb000;
unsigned urock = 1076, drock = 2996;
unsigned ship = 1322, lship = 1322;

main()
{
int z;
clrscr();

while( !kbhit() )
	{
	newrocks();
	scroll_left();
	showship();
	}

if( getch() == 0 )
  processcntlkeys();

else
	exit( 1 );


}

newrocks()
{
int uadd, dadd;

uadd = (random( 3 ) - 1) * 160;
dadd = (random( 3 ) - 1) * 160;

if( (urock += uadd) < 916 | urock > 1716 )
	urock -= uadd;

if( (drock += dadd) > 2996 | drock < 2356 )
	drock -= dadd;

pokeb( CARDBASE, urock, (char) 219 );
pokeb( CARDBASE, drock, (char) 219 );

return 0;
}

scroll_left()
{
unsigned x, y;

for( x = 42; x < 120; x += 2 )
	{
	for( y = 5; y < 20; y++ )
		{
		pokeb( CARDBASE, ( y * (unsigned) 160 ) + ( x - (unsigned) 2 ), peekb( CARDBASE, ( y * (unsigned) 160) + x ) );
		/*pokeb( CARDBASE, ( y * (unsigned) 160 ) + ( x - (unsigned) 1 ), peekb( CARDBASE, ( y * (unsigned) 160) + ( x + (unsigned) 1 ) ) );*/
		}
	}

return 0;
}

space( int numberofspaces )
{
int i;

for( i = 0; i < (numberofspaces + 1); i++ )
	printf("%c",(char) 32 );

return 0;
}

showship()
{

pokeb( CARDBASE, ship, (char) 0xe8 );
pokeb( CARDBASE, lship, (char) 0x20 );

return 0;
}