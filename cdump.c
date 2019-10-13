#include <stdio.h>

int
main(void)
{
   int c;

   while (c = getchar(), c != EOF)
      switch (c & 0x7f) {
      case 0 ... 31:
         putchar(c + '@');
         break;
      case 32:
         putchar('.');
         break;
      case 127:
         putchar('?');
         break;
      default:
         putchar(c);
         break; }

   putchar('\n');
   return 0;
}
