#include <stdio.h>


void
put(char c)
{
   putchar(c);
}

void
out(const char c)
{
   const char cp = c & 0x7f;
   switch (cp) {
   case 0 ... 31:
      put(cp + '@');
      break;
   case 32:
      put('.');
      break;
   case 127:
      put('?');
      break;
   default:
      put(cp);
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
