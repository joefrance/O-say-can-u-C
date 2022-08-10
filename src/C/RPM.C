
double rpm;
float sfm, d, pi = 3.14159265;

main()
{
loop:
clrscr();

printf("\nEnter SFM: ");
scanf( "%f",&sfm );
printf("\nEnter DIA: ");
scanf( "%f", &d );

rpm = (double)( (float)1/( ( pi * d ) / 12 ) * sfm );
printf( "\nRPM = %lf\n", rpm );

printf("Press any key for another calculation or <Esc> to exit...");
if( getch() != 27 )
	goto loop;

exit( 1 );


}

