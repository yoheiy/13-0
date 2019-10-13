#include <stdio.h>


void
put(char c)
{
   putchar(c);
}

void
out(char c)
{
   switch (c & 0x7f) {
   case 0 ... 31:
      put(c + '@');
      break;
   case 32:
      put('.');
      break;
   case 127:
      put('?');
      break;
   default:
      put(c);
      break; }
}

int
main(void)
{
   int c;

   while (c = getchar(), c != EOF)
      out(c);

   putchar('\n');
   return 0;
}
