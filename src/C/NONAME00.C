
double result;
float rpm, sfm, d, pi = 3.14159265;

main()
{
printf("Enter 0 in the feild to be calculated.\n");
printf("\nEnter RPM: ");
scanf( "%f", &rpm );
printf("\nEnter SFM: ");
scanf( "%f",&sfm );
printf("\nEnter DIA: ");
scanf( "%f", &d );

if ( rpm == (float)0.00 )
	{
	result = (double)( (float)1/( ( pi * d ) / 12 ) * sfm );
	printf( "\nRPM = %lf\n", result );
	}

printf("Press any key...");
getch();


}

