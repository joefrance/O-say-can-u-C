#include <stdio.h>
#include <alloc.h>

unsigned long runlength, filelength;

char far *origsamps[10], *samples[10];

float frequency, period;

unsigned int maxfreq;

struct CKID {
				unsigned long ID, LENGTH;
				} chunk;

struct VHDR {
				unsigned long  headerlength;
				unsigned long	oneshotsamples,
									repeatsamples,
									samplespercycle;
				unsigned int	samplespersecond;
				unsigned char	ctoctave,
									scompression;
				long				volume;
				} voiceheader;

FILE *fp;
int i,k,l;
char vl, vh;

unsigned long j;

#define DEBUG 1

/*#define MAKEID( a,b,c,d ) ( (a) << 24 | (b) << 16 | (c) << 8 | (d) )*/

unsigned long 	FORM,
				  _8SVX,
					VHDR,
					NAME,
					ANNO,
					ATAK,
					RLSE,
					AUTH,
					COPY,
					CHAN,
					BODY;

readint( char far *add, FILE *in )
{
unsigned int i;

for(i=0;i<2;i++)
	add[1-i] = (char) fgetc( in );


return 0;
}

readlong( char far *add, FILE *in )
{
unsigned int i;
char far *followc;
unsigned long followl;

followc = &followl;

for(i=0;i<4;i++)
	followc[3-i] = add[3-i] = (char) fgetc( in );

return 0;
}

unsigned long makelongfrom4chars( char l0, char l1, char l2, char l3 )
{

	return ( ((unsigned long)l0 * (unsigned long)0x1000000) | ((unsigned long)l1 * (unsigned long)0x10000)
				| ((unsigned long)l2 * (unsigned long)0x100)| (unsigned long)l3 );
}

main( int argc, char *argv[] )
{

printf("Free memory = %lu bytes.\n", farcoreleft() );

create4();

printf("Opening %s.\n", argv[1] );

if( (fp = fopen( argv[1], "rb" ) ) == NULL )
	cleanup( "Couldn't open file." );

readlong( &chunk.ID, fp );

if( chunk.ID == FORM )
	{
	readlong( &chunk.LENGTH, fp );
	readlong( &chunk.ID, fp );
	if( chunk.ID == _8SVX )
		filelength = chunk.LENGTH;
	else
		cleanup( "Not an 8SVX sound file." );
	}
else
	cleanup( "Not a FORM sound file." );

while( !feof( fp ) && runlength < filelength )
	{
	readlong( &chunk.ID, fp );
	readlong( &chunk.LENGTH, fp );
	runlength += chunk.LENGTH;
	if( chunk.ID == VHDR )
		{
		readlong ( &voiceheader.oneshotsamples, fp );
		readlong ( &voiceheader.repeatsamples, fp );
		readlong ( &voiceheader.samplespercycle, fp );
		readint	( &voiceheader.samplespersecond, fp );
		voiceheader.ctoctave = (unsigned char) fgetc( fp );
		voiceheader.scompression = (unsigned char) fgetc( fp );
		readlong( &voiceheader.volume, fp );
		printf("One shot samples = %lu.\n", voiceheader.oneshotsamples );
		printf("Repeat samples = %lu.\n", voiceheader.repeatsamples);
		printf("Samplespercycle = %lu.\n", voiceheader.samplespercycle );
		printf("Sampling rate = %u.\n", voiceheader.samplespersecond );
		printf("Number of octaves = %i.\n", (int)voiceheader.ctoctave );
		printf("Compression = %i.\n", (int)voiceheader.scompression );
		printf("Volume = %8lx.\n", voiceheader.volume );
		maxfreq = voiceheader.samplespersecond / 2;
		}

	if( chunk.ID == ANNO || chunk.ID == AUTH || chunk.ID == COPY || chunk.ID == NAME )
		{
		for( j=0;j<chunk.LENGTH;j++)
			{
			k = fgetc( fp );
			printf("%c", (char) k );
			}
		printf("\n");
		}

		if( chunk.ID == ATAK || chunk.ID == RLSE || chunk.ID == CHAN )
			{
			for(j=0;j<chunk.LENGTH;j++)
				k = fgetc( fp );
			}

		if( chunk.ID == BODY )
			{
			printf("chunk.LENGTH = %lu bytes. Attempting to procure memory for %i octave(s).\n", chunk.LENGTH, (int)voiceheader.ctoctave );
			for( l = 0;l < (int)voiceheader.ctoctave; l++ )
				{
				samples[l] = farmalloc( (unsigned long)( ( voiceheader.oneshotsamples + voiceheader.repeatsamples ) * ( l + 1) ) );
				if( samples[l] != NULL )
					{
					origsamps[l] = samples[l];
					printf("Allocated %lu bytes for samples[%i].\n", (unsigned long)( ( voiceheader.oneshotsamples + voiceheader.repeatsamples ) * ( l + 1) ), l );
					printf("Reading sound data for octave %i.\n", l + 1 );
					for( j=0;j<(unsigned long)( ( voiceheader.oneshotsamples + voiceheader.repeatsamples ) * ( l + 1) );j++)
						{
						*samples[l] = (char)fgetc( fp );
						samples[l]++;
						}
					samples[l] = origsamps[l];
					}
				else
					cleanup( "Couldn't allocate memory for octave." );
				}
			}

	}

clrscr();
runthrusamples();

printf("Press any key to exit.\n");
getch();

cleanup( "Normal Program Termination." );

return 0;
}

cleanup( char far *msg )
{
int ci;
for(ci=0;ci<10;ci++)
	if( samples[ci] != NULL )
		farfree( samples[ci] );
if( fp != NULL )
	fclose( fp );
printf("%s\n", msg );
printf("\nPress any key.\n");
getch();
clrscr();
printf("Free memory = %lu bytes.\n", farcoreleft() );

exit( 1 );
return 0;
}

create4()
{
FORM = makelongfrom4chars( 'F','O','R','M' );
_8SVX = makelongfrom4chars( '8','S','V','X' );
VHDR = makelongfrom4chars( 'V','H','D','R' );
NAME = makelongfrom4chars( 'N','A','M','E' );
ANNO = makelongfrom4chars( 'A','N','N','O' );
COPY = makelongfrom4chars( 'C','O','P','Y' );
AUTH = makelongfrom4chars( 'A','U','T','H' );
ATAK = makelongfrom4chars( 'A','T','A','K' );
RLSE = makelongfrom4chars( 'R','L','S','E' );
BODY = makelongfrom4chars( 'B','O','D','Y' );
CHAN = makelongfrom4chars( 'C','H','A','N' );

return 0;
}


runthrusamples()
{
unsigned long sampleindex;
unsigned int samplecounter;
int toggle = 1, ltoggle = 1;
int cycle = 0, levelx, iter;
int ypos;
unsigned char frequencies[80], level = 0, llevel =0;

for(levelx=0;levelx<80;levelx++);
	frequencies[ levelx ] = 0;

clrscr();

samples[0] = origsamps[0];
for(iter=0;iter<(int)(voiceheader.oneshotsamples/80);iter++)
{

for(sampleindex=0;sampleindex<80;sampleindex++) /*( voiceheader.oneshotsamples + voiceheader.repeatsamples );samplesindex++)*/
	{
	level = (unsigned char)abs( (int) *samples[0] );
	if( level > llevel )
		{
		llevel = level;
		gotoxy( 1, 24 );
		printf("%i, %i", llevel, level );
		}

	if( *samples[0] >  0 )
		{
		toggle = 1;
		gotoxy( (int)sampleindex+1, 1 );
		printf("-");
		}
	else
		{
		toggle = -1;
		gotoxy( (int)sampleindex+1, 1 );
		printf("_");
		}
	if( toggle != ltoggle )
		{
		cycle++;
		ltoggle = toggle;
		}
	if( cycle > 1 )
		{
		ypos = calcfrequency( samplecounter );
		frequencies[ypos] = llevel / 8;
		gotoxy( ypos + 1, (13 - (llevel / 10)) + 2);
		printf("%c", (char)219 );
		samplecounter = 1;
		cycle = 0;
		level = llevel = 0;
		}
	else
		{
		samplecounter++;
		samples[0]++;
		}
	if( kbhit() )
		{
		getch();
		cleanup( "Outta here!");
		}
	}
if(getch() == 27)
	cleanup("Outta here!");
}


return 0;
}

calcfrequency( unsigned int numberofsamples )
{
double period, frequency;
int f;

period = (float) 1 / (float) voiceheader.samplespersecond;
frequency = 1 / (period * (double)numberofsamples);
gotoxy( 1, 25 );
printf("Frequency = %lf.", frequency );

f = (int)frequency / (int)(voiceheader.samplespersecond / 80) - 1;
if( f < 0 )
	f = 0;
printf("    Sampling rate = %i.", (int)voiceheader.samplespersecond);
return ( f );
}

showequaltemperedscale()
{
double root12, freq = 27.5;
int i;

clrscr();

root12 = (double)2 * power( (double)2, (double)1 / (double)12 )

for(i=0;i<96;i++)
	{
	gotoxy(1,25);
	printf("Note frequency = %lf.", freq);
	freq *= root12;
	getch();
	}

return 0;
}