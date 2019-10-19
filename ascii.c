#include <stdio.h>
int
main(void)
{
   for (int i = 0; i < 0x100; i++)
      putchar(i);

   return 0;
}
