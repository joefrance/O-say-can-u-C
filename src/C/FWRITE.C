#include <stdio.h>

struct mystruct
{
  int i;
  char ch;
};

int main(void)
{
   FILE *stream;
   struct mystruct s;

	if ((stream = fopen("com1", "wb")) == NULL) /* open file TEST.$$$ */
	{
		fprintf(stderr, "Cannot open output file.\n");
		return 1;
	}
	s.i = 0;
	s.ch = 'A';
	fwrite(&s, sizeof(s), 1, stream); /* write struct s to file */
	fclose(stream); /* close file */
	return 0;
}
