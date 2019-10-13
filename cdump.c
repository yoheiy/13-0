#include <stdio.h>

void
out(char c)
{
   putchar(c);
}

int
main(void)
{
   int c;

   while (c = getchar(), c != EOF)
      switch (c & 0x7f) {
      case 0 ... 31:
         out(c + '@');
         break;
      case 32:
         out('.');
         break;
      case 127:
         out('?');
         break;
      default:
         out(c);
         break; }

   putchar('\n');
   return 0;
}
