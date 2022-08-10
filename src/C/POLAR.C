#include<stdio.h>
#include<math.h>

int key;
double r,t;
double z,x;
double radians = 57.295779;

main()
{
clrscr();
printf("\nPolar to Rectangular(p) or Rectangular to Polar(r). Press a key: \n");
if( (key = getch()) == 'p' )
	{
	printf("Enter the magnitude: ");
	scanf("%lf", &r);
	printf("Enter the direction( 0 degrees points right ): ");
	scanf("%lf", &t);

	z = r * cos( t / radians );
	x = r * sin( t / radians );

	printf("z = %lf, x = %lf. Press a key",z,x);
	getch();
	clrscr();
	exit(1);
	}
else
if( key == 'r' )
	{
	printf("Enter z: ");
	scanf("%lf", &z);
	printf("Enter x: ");
	scanf("%lf", &x);

	r = sqrt( z * z + x * x );

	t = radians * atan( x / z );

	printf("magnitude = %lf, direction = %lf(0 degrees points right). Press a key",r,t);
	getch();
	clrscr();
	exit(1);
	}
}