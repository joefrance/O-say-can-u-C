#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <alloc.h>

FILE *pcm;
int x;
char pcmv;
char *str = NULL;
long strcnt = 0, endcnt;

int main(void)
{
	if( (str = calloc( 32767, sizeof(char) ) ) == NULL )
		{
		printf("Couldn't allocate memory for samples.\n");
		exit( 1 );
		}

	if( (pcm = fopen("c:\\temp\\baliflut.pcm", "rb")) == NULL )
		{
		printf("Couldn't open c:\\temp\\baliflut.pcm.\n");
		getch();
		closegraph();
		free( str );
		exit( 1 );
		}

	skipit:

	while( !feof( pcm ) )
		{
		pcmv = (char)fgetc( pcm );
		str[strcnt++] = pcmv;
		}
	endcnt = strcnt;
playagain:
	printf("\nPress any key to play samples.\nPress <Esc> to exit.");
	if( getch() == 27 )
		goto cleanup;
	outportb( 0x61, inportb( 0x61 ) & 0xfc );

	for(strcnt=0;strcnt<endcnt;strcnt++)
		{
		if( str[strcnt] > 0 )
			outportb( 0x61, inportb( 0x61 ) | 0x02 );
		else
			outportb( 0x61, inportb( 0x61 ) & 0xfc );
		}
	goto playagain;
cleanup:
	/* clean up */
	free( str );
	fclose( pcm );
   return 0;
}
