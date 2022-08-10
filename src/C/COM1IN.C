#include <stdio.h>
#include <dos.h>
#include <dir.h>

int in;
FILE *receive, *transmit;

main()
{
	if ( ( transmit = fopen("com1", "wb" ) ) == NULL )
		{
		printf("Error: Couldn't open COM1:.\nPress any key to exit.");
		getch();
		exit( 1 );
		}
	else
		printf("Opened COM1:\n");
		fputs( "Hello there\n", transmit );

	if ( ( receive = fopen("com1", "rb" ) ) == NULL )
		{
		printf("Error: Couldn't open COM1:.\nPress any key to exit.");
		getch();
		exit( 1 );
		}
	else
		printf("Opened COM1:\n");

	while( in != 10 || in != 13 )
		{
		in = fgetc( receive );
		printf("in = %c = %i", (char) in, in);
		fputc( in, receive );
		}
	if ( receive != NULL )
		{
		fclose( receive );
		exit( 1 );
		}
}