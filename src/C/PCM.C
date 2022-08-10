#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <alloc.h>

FILE *samples, *pcm;
int x;
char amplitude, pcmv;
char *str = NULL;
long strcnt = 0, endcnt;

int main(void)
{
   /* request auto detection */
	int gdriver = CGA, gmode = CGAHI, errorcode;

	/* initialize graphics mode */

	if( (str = calloc( 32767, sizeof(char) ) ) == NULL )
		{
		printf("Couldn't allocate memory for samples.\n");
		exit( 1 );
		}

   initgraph(&gdriver, &gmode, "");

   /* read result of initialization */
   errorcode = graphresult();

   if (errorcode != grOk)  /* an error occurred */
   {
      printf("Graphics error: %s\n", grapherrormsg(errorcode));
      printf("Press any key to halt:");
		getch();
		free( str );
		exit(1);             /* return with error code */
	}

	if( (pcm = fopen("c:\\temp\\dogbark.pcm", "wb")) == NULL )
		{
		printf("Couldn't open c:\\temp\\dogbark.pcm.\n");
		getch();
		closegraph();
		free( str );
		exit( 1 );
		}
	if( (samples = fopen("a:\\iff\\dogbark.iff", "rb")) == NULL )
		{
		printf("Couldn't open a:\\iff\\dogbark.iff.\n");
		fclose( pcm );
		free( str );
		getch();
		closegraph();
		exit( 1 );
		}

	while( !feof( samples ) )
		{
		amplitude = (char)fgetc( samples );
		printf("%c", (char)amplitude);
		if( getch() == 27 )
			goto skipit;
		}
	skipit:
	moveto( 0, 100 );

	while( !feof( samples ) )
		{
		if( ( amplitude = (char)fgetc( samples ) ) > -1 )
			pcmv = 127;
		else
			pcmv = -128;
		fputc( (int)pcmv, pcm );
		str[strcnt++] = pcmv;

		lineto( x++, 100 + ( (int)amplitude / 2 ) );
		if( x > 639 )
			{
			clearviewport();
			x = 0;
			moveto( 0, 100 );
			}
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
	fclose( samples );
	fclose( pcm );
   getch();
   closegraph();
   return 0;
}
