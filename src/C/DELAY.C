#include <math.h>

int x, array[80];
int real= 40, delay = 0;
double y;

main()
{
clrscr();
for(x=1;x<78;x++)
	{
	y = ( 12 + 4 * sin( (double)x / 12 ) );
	array[x-1] = (int)y );

	printf("%c", 219);
	}
getch();

}