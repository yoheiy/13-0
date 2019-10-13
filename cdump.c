#include <stdio.h>

enum {
   NORMAL,
   CTRL,
   PRINTABLE,
   META_CTRL,
   META_PRINTABLE };

enum {
   NONE,
   RED,
   GREEN,
   YELLOW,
   WHITE };

void
esc(int colour)
{
}

void
class_change(int cls)
{
   static int prev_cls = NORMAL;

   if (cls == prev_cls) return;

   switch (cls) {
   case NORMAL:
      esc(NONE);
      break;
   case CTRL:
      esc(RED);
      break;
   case PRINTABLE:
      esc(WHITE);
      break;
   case META_CTRL:
      esc(GREEN);
      break;
   case META_PRINTABLE:
      esc(YELLOW);
      break; }
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
