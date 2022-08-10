#include <math.h>

float a, b, c, A, B, C;
double res1, res2, res3;

main()
{
loop:
clrscr();

printf("\n\n\n 1. Calculate an angle.\n");
printf(" 2. Find the length of the adjacent side.\n");
printf(" 3. Find the length of the opposite side. \n");
printf(" 4. Find the length of the hypotenuse. \n");
printf(" 5. Chamfer a tapered corner. \n");
printf(" 6. Radius a tapered corner. \n\n");
printf(" Please enter a selection: ");

getch();

}

drawtriangle()
{
printf("             b\n\n");
printf("   ||===================//\n");
printf("   || C=90        A  //\n);
printf("   ||             //\n");
printf("a  ||          //\n");
printf("   ||       //    c\n");
printf("   || B  //\n");
printf("   || //\n");
printf("   //\n");

return 0;
}