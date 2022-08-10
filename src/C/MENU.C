#include <stdio.h>
#include <process.h>
#include <time.h>
#include <stdlib.h>

int choice, result, fchc;
clock_t t1;

main()
{
drawscreen();

menuloop:
	t1 = clock();
	screensave();

	if(choice == 0)
		{
		getch();
		goto menuloop;
		}

	printf("\n");

		switch( choice )
			{

			case '1':

				if( (result = system("milacron") ) == -1 )
					displayerror( "Couldn't run Termtalk");

			break;

			case '2':

				if( (result = system("fadal") ) == -1 )
					displayerror("Couldn't run 'Assist'");

			break;

			case '3':

				if( (result = system("qdir") ) == -1 )
					displayerror("Couldn't run 'Quick Directory'");

			break;

			case '4':

				if( ( result = chdir("c:\\tc\\bin") ) == -1 )
					displayerror("Couldn't find 'tc\bin' directory");
				else
					if ( (result = system("tc") ) == -1 )
						displayerror("Couldn't run 'Turbo C++'");

			break;

			case '5':

				if( ( result = system("format b:") ) == -1 )
					displayerror("Couldn't format drive B:. Check write protect tab");

			break;

			case '6':

				if( ( result = system("format a: /n:9 /t:80") ) == -1 )
					displayerror("Couldn't format drive A:. Check write protect tab");

			break;

			case '7':

				if( ( result = system("format a:") ) == -1 )
					displayerror("Couldn't format drive A:. Check write protect tab");

			break;

			default:

				displayerror("Try another selection");

			}

drawscreen();
goto menuloop;

}

displayerror ( char *errormsg )
{
	clrscr();
	gotoxy( 2,3 );
	printf("Error:%s. Press any key.", errormsg);
	getch();
	clrscr();
	drawscreen();

return 0;
}

drawscreen()
{
clrscr();
gotoxy( 25, 11 );
printf("1 - Cincinatti Milacron Communications\n");
gotoxy( 25, 12 );
printf("2 - Fadal Communications\n");
gotoxy( 25, 13 );
printf("3 - CNC Link\n");
gotoxy( 25, 14 );
printf("4 - Turbo C++\n");
gotoxy( 25, 15 );
printf("5 - FORMAT A 360K 5-1/4 DISK");
gotoxy( 25, 16 );
printf("6 - FORMAT A 720K 3-1/2 DISK");
gotoxy( 25, 17 );
printf("7 - FORMAT A 1.44M 3-1/2 DISK");

gotoxy( 14,19 );
printf("Enter a selection ( Hit <Esc> to exit to dos. ): ");

return 0;
}

screensave()
{
waitloop1:
while( !kbhit() )
	{
	if ( (clock() - t1 ) > 182 )
		{
		waitloop2:
		while( !kbhit() )
			{
			clrscr();
			gotoxy ( (int)1 + random( (int)32 ), (int)1 + random( (int)26 ) );
			printf("Preventing screen-burn. Press enter key.");
			delay( 650 );
			}
		choice = getch();

		if( choice != 13)
			goto waitloop2;
		else
			{
			drawscreen();
			goto skipit;
			}
		}
	}
skipit:
if( (choice = getch()) == 27 )
	{
	clrscr();
	printf("Type 'MENU' from anywhere in DOS to re-run this menu.\n");
	exit( 1 );
	}

if( choice < '1' || choice > '9' )
	goto waitloop1;

return 0;
}