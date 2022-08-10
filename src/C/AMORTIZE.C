#include<stdio.h>
#include<math.h>

float FV, PV, PP, IP, i, n;
double payment, interest;
int j;

main()
{
clrscr();

printf("Enter the amount of purchase:");
scanf("%f",&PV);

FV = PV;

printf("Enter the annual percentage rate(Example: For 18.5%% A.P.R. enter .185): ");
scanf("%f",&i);
printf("Enter the desired number of monthly payments: ");
scanf("%f",&n);

payment = (double)(PV / ( ( 1 - (float) pow( (double) ( 1 + i/12), (double) -n ) ) / ( i / 12 ) ) );
interest = payment * (double) n - (double) PV;
printf("\nYour monthly payment will be %lf.\nYou will pay a total of %f interest to borrow $%f for %f month(s).\n", payment, interest, PV, n );
printf("\nPress any key to see an amortization table...");
if( getch() == 27 )
	{
	clrscr();
	exit( 1 );
	}
clrscr();
drawheader();

for (j=0; j<(int)n+1; j++)
	{
	printf("%3i     %15lf  %15lf  %15lf\n",j, PP, IP, PV );
	IP = PV * i / 12;
	PP = payment - IP;
	PV -= PP;

	if( j>0 )
		if( j % (int) 18 == 0 )
			{
			printf("Press any key to see more...");
			getch();
			clrscr();
			drawheader();
			}
	}
printf("Press any key to exit...");
getch();
clrscr();
exit(1);
}

drawheader()
{
printf("Amortization table for %f @ %f%% A.P.R. for %f months.\nMonthly payment is %lf.\n", FV, i * 100, n, payment);
printf("Month   Principal Paid  Interest Paid    Remaining Loan\n");
printf("-------------------------------------------------------\n");
return 0;
}
