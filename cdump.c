#include <stdio.h>

enum {
   NORMAL,
   CTRL,
   PRINTABLE,
   META_CTRL,
   META_PRINTABLE };

void
class_change(int cls)
{
}

void
put(char c)
{
   putchar(c);
}

void
out(const char c)
{
   const char cp = c & 0x7f;
   const char cm = c & 0x80;

   switch (cp) {
   case 0 ... 32:
   case 127:
      class_change(cm ? META_CTRL : CTRL);
      break;
   default:
      class_change(cm ? META_PRINTABLE : PRINTABLE);
      break; }

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

   class_change(NORMAL);
   putchar('\n');
   return 0;
}
