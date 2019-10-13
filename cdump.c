#include <stdio.h>

int
main(void)
{
   int c;

   while (c = getchar(), c != EOF)
      putchar(c);

   return 0;
}
