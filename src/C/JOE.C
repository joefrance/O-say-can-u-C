#include <stdio.h>
#include <time.h>
#include <dos.h>
#include <float.h>
#include <math.h>

float times, period,frequency, t1, t2;
clock_t t21, t22;
int i,j;
unsigned long total = 0;

void main( void )
{
	period = 3.14159265;
	t21 = clock();

	for(i=0;i<1000;i++)
		for(j=0;j<10;j++);

	t22 = clock();

	times = ( (float) t22 - (float) t21 ) / (float) 18.2;

	period = times / (float) 65536;

	frequency = (float) 1 / period;
	total = (unsigned long) frequency;

	printf("Time = %f in float and %i in int\n",times, (int) (t22 - t21) );

	printf("%f = period\n", period);
	printf("%f = frequency\n",frequency);
	printf("ULONG frequency = total = %lu\n", total);

	getch();
}

