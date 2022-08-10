#include <conio.h>
#include <stdlib.h>

#define LEFTCURSOR 75
#define RIGHTCURSOR 77
#define UPCURSOR 72
#define DOWNCURSOR 80

#define DEAD 0
#define ALIVE 1

#define ON 1
#define OFF 0

#define MONOCHROME '1'
#define CGA '2'
#define EGAVGA '3'

int SOUND = ON;
int ship = 3920, lship = 3920;
int lmissle = 160, missle =0 ;
int key, i, j, startenemy = 162;
int enemiesremaining = 32;
int shipsremaining = 5;
int toggle = -1;
unsigned CARDBASE;
int whichcard;
int score = 0;
int scoreperhit = 10;
int lastscore= 0;
int level, dlay = 100;

char enemyshape[32];

struct enemy
	{
	int pos;
	int nextenemy;
	int previousenemy;
	char status;
	};

struct bomb
	{
	int pos;
	char status;
	char sound;
	};

struct enemy enemy[32];
struct bomb bomb = { 4000, (char)DEAD, (char)DEAD };

main()
{
clrscr();
choosevideo();
printstatus();
initenemies();
gameloop:
while( !kbhit() )
	{
	if( score > lastscore )
		{
		printstatus();
		lastscore = score;
		}
	delay( dlay );
	Bomb();
	Enemy();
	Missle();
	Ship();
	}

checkkeys();
goto gameloop;

}

Bomb()
{
	i = random( 32 );
	if( bomb.status == (char)DEAD && enemy[i].status == (char)ALIVE )
		{
		bomb.pos = enemy[i].pos + 160;
		bomb.status = bomb.sound = (char)ALIVE;
		}

	if ( bomb.pos < 4000 && bomb.status == (char)ALIVE )
		{
		if( (char)peekb( CARDBASE, (unsigned)bomb.pos ) == (char) 0x0f )
			{
			pokeb( CARDBASE, (unsigned)bomb.pos - 160, (char) 32 );
			checkship();
			}
		else
			{
			pokeb( CARDBASE, (unsigned)bomb.pos, (char) 0x1f );
			pokeb( CARDBASE, (unsigned)bomb.pos - 160, (char) 32 );
			if(bomb.status == (char)ALIVE && SOUND == ON)
				sound( bomb.pos - 1500 );
			}
		bomb.pos += 160;
		}
	else
		{
		pokeb( CARDBASE, (unsigned)bomb.pos - 160, (char) 32 );
		bomb.status = bomb.sound = (char)DEAD;
		nosound();
		}

return 0;
}

checkship()
{
if( shipsremaining == 0 )
	{
	nosound();
	gotoxy( (int)1, (int)24 );
	printf("HA! HA!. I won!");
	getch();
	_setcursortype( _NORMALCURSOR );
	exit( 1 );
	}
else
	{
	shipsremaining--;
	printstatus();
	}

return 0;
}

printstatus()
{
gotoxy(7,1);
printf("     ");
gotoxy(1,1);
printf("SCORE %5i SHIPS %i LEVEL %i    'P' = Pause, 'S' = Sound On/Off, 'Esc' = exit.", score, shipsremaining, level );

return 0;
}

initenemies()
{
startenemy = 162;
enemiesremaining = 32;

for (i=0;i<32;i++)
	{
	enemy[i].pos = startenemy;
	enemyshape[i] = (char) 1 + (char)(i/8);
	pokeb( CARDBASE, (unsigned)enemy[i].pos, enemyshape[i] );
	enemy[i].nextenemy = i + 1;
	if( i > 0 )
		enemy[i].previousenemy = i - 1;
	enemy[i].status = (char)ALIVE;
	startenemy += 20;
	}
enemy[31].nextenemy = 0;
enemy[0].previousenemy = 31;

return 0;
}

delete_enemy()
{
	gotoxy( 23,1 );
/*	printf("Trying to delete enemy #%2i.", i); */
	pokeb( CARDBASE, (unsigned)(enemy[i].pos), (char) 32 );
	enemy[i].status = (char)DEAD;
	enemy[ enemy[i].previousenemy ].nextenemy = enemy[i].nextenemy;
	enemy[ enemy[i].nextenemy ].previousenemy = enemy[i].previousenemy;
	enemiesremaining--;
/*	printf("Enemies remaining = %2i.", enemiesremaining ); */
	score += scoreperhit * (4 - (i/8));

return 0;
}

Enemy()
{
if ( enemiesremaining == 0 )
		{
		if( level < 10 )
			{
			level++;
			shipsremaining++;
			dlay -= 10;
			printstatus();
			initenemies();
			}
		else
			{
			nosound();
			gotoxy( (int)1, (int)24 );
			printf("You win!");
			getch();
			_setcursortype( _NORMALCURSOR );
			exit( 1 );
			}
		}

	for (i=0;i<32;i++)
		{
		if( enemy[i].status == (char)ALIVE )
			{
			if( enemy[i].pos == ship || enemy[i].pos > 3999 )
				{
				checkship();
				delete_enemy();
				}
			else
				{
				if( enemy[i].pos == missle )
					{
					delete_enemy();
					for(j=0;j<((level/2)+1);j++)
						pokeb( CARDBASE, (unsigned)lmissle - (j * 160), (char) 32 );
					lmissle = 160;
					missle = 0;
					}
				else
					{
					enemy[i].pos += 2;
						pokeb( CARDBASE, (unsigned)enemy[i].pos, enemyshape[i] );
						pokeb( CARDBASE, (unsigned)(enemy[i].pos - 2), (char) 32 );
					}
				}
			}
		}

return 0;
}

checkkeys()
{
if( ( key = getch() ) == 0 )
	{
	key = getch();
	switch( key )
		{
		case UPCURSOR:
			if( ship > 160 )
				ship -= 160;
		break;
		case DOWNCURSOR:
			if( ship < 3998 )
				ship += 160;
		break;
		case LEFTCURSOR:
			if( ship > 2 )
				ship -= 2;
		break;

		case RIGHTCURSOR:
			if( ship < 3998 )
				ship += 2;
		break;
		}
	}
else
	{
	if( key == 32 ) /*  && missle < 162 ) */
		missle = ship - 160;

	if( key == (int)'S' || key == (int)'s')
		{
		if( SOUND == ON )
			SOUND = OFF;
		else
			SOUND = ON;
		}

	if( key == (int)'P' || key == (int)'p' )
		{
		nosound();
		getch();
		}

	if( key == 27 )
		{
		nosound();
		clrscr();
		gotoxy( (int)1, (int)24 );
		printf("What a wimp!");
		_setcursortype( _NORMALCURSOR );
		getch();
		exit(1);
		}
	}

return 0;
}

choosevideo()
{
_setcursortype( _SOLIDCURSOR );
printf("1. Monochrome\n");
printf("2. CGA\n");
printf("3. EGA/VGA\n");
printf("Choose your video adapter: ");

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

	default:
		CARDBASE = (unsigned)0xb800;

	}
clrscr();

_setcursortype( _NOCURSOR );

return 0;
}

Missle()
{
if ( missle > 160 )
		{
		pokeb( CARDBASE, (unsigned)missle, (char) 0x1e );
		pokeb( CARDBASE, (unsigned)lmissle, (char) 32 );
		lmissle = missle + (160 * (level / 2));
		missle -= 160;
		}
	else
		for(j=0;j<((level/2)+1);j++)
			pokeb( CARDBASE, (unsigned)lmissle - (j * 160), (char) 32 );

return 0;
}

Ship()
{
if ( ship > 162 && ship < 3998 )
		{
		if( lship != ship )
			pokeb( CARDBASE, (unsigned)lship, (char) 32 );
		pokeb( CARDBASE, (unsigned)ship, (char) 0x0f );
		lship = ship;
		}

return 0;
}