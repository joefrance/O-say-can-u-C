int choice, count;

main()
{
clrscr();
gotoxy( 1,4 );
for(choice=0;choice<256;choice++, count++)
	{
	printf("%x %c, ",choice, (char)choice);
	if( count == 13 )
		{
		printf("\n");
		count = 0;
		}
	}


	loop1:
	if( ( choice  = getch()) == 27 )
		exit( 1 );
	else
		{
		switch( choice )
			{
			case 'q':
				pokeb( (unsigned) 0xb000, 0, peekb( 0xb000, 0 ) + 1 );
			break;

			case 'w':
				pokeb( (unsigned) 0xb000, 2, peekb( 0xb000, 2 ) + 1 );
			break;

			case 'a':
				pokeb( (unsigned) 0xb000,160, peekb( 0xb000, 160 ) + 1 );
			break;

			case 's':
				pokeb( (unsigned) 0xb000, 162, peekb( 0xb000, 162 ) + 1 );
			break;

			}
		}
	goto loop1;
}