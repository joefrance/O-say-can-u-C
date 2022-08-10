#include <stdio.h>

main()
{
	int steps[8] = { 0,2,4,5,7,9,11,12 };
	char *notes[26] = {  "",
								"A ",
								"A#",
								"B ",
								"C ",
								"C#",
								"D ",
								"D#",
								"E ",
								"F ",
								"F#",
								"G ",
								"G#",
								"A ",
								"A#",
								"B ",
								"C ",
								"C#",
								"D ",
								"D#",
								"E ",
								"F ",
								"F#",
								"G ",
								"G#",
								"A " };
	int i,j,k;
	for (i = 1;i<13;i++)
	{
		for( j=0;j<8;j++)
			printf("%s ", notes[i + steps[j] ] );

		printf("\n");

	}
	getch();

}