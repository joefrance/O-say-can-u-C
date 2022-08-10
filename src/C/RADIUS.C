#include<stdio.h>
#include<math.h>

double ta, r, ma, x, z, xd1, xd2, zd, bx, bz, ex, ez;
double fx, fz, result;
double radians = 57.29577951;

main()
{
	clrscr();
	printf("Enter the X dimension of taper start point: ");
	scanf("%lf", &x);
	printf("Enter the Z dimension of taper start point: ");
	scanf("%lf", &z);
	printf("Enter the X dimension of taper end point: ");
	scanf("%lf", &fx);
	printf("Enter the Z dimension of taper end point: ");
	scanf("%lf", &fz);

	printf("Enter taper angle...(enter -1 if unknown): ");
	scanf("%lf", &ta);
	printf("Enter the corner radius: ");
	scanf("%lf", &r);

	if( ta == (double)-1 )
		{
		ta = atan( ( x - fx ) / ( z - fz ) ) * radians;
		printf("The taper angle = %lf degrees.\n", ta);
		}

	ma = (90 + ta) / 2;
	xd1 = r / tan( ma / radians );
	zd = xd1 * cos( ta / radians );
	xd2= xd1 * sin( ta / radians );
	bx = x + xd1;
	ex = x - xd2;
	bz = z;
	ez = z - zd;
	printf("\nThese are your program lines to add to your program: \n");
	printf("G01 X%lf Z%lf\n", bx, bz );
	printf("G02 X%lf Z%lf P%lf\n", ex, ez, r );
	printf("G01 X%lf Z%lf\n", fx, fz );

	getch();
	clrscr();

}


