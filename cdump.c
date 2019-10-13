#include <stdio.h>
#include <unistd.h>

int option_respect_newline;

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
esc_out(int n)
{
   if (n)
      printf("\033[%d;40m", n);
   else
      printf("\033[0m");
}

void
esc(int colour)
{
   switch (colour) {
   case NONE:
      esc_out(0);  break;
   case RED:
      esc_out(31); break;
   case GREEN:
      esc_out(32); break;
   case YELLOW:
      esc_out(33); break;
   case WHITE:
      esc_out(37); break; }
}

void
class_change(int cls)
{
   static int prev_cls = NORMAL;

   if (cls == prev_cls) return;
   prev_cls = cls;

#if 0
   // DEBUG
#define SHOW_CLASS(x) case x: printf("{%s}", #x); break;
   switch (cls) {
   SHOW_CLASS(NORMAL);
   SHOW_CLASS(CTRL);
   SHOW_CLASS(PRINTABLE);
   SHOW_CLASS(META_CTRL);
   SHOW_CLASS(META_PRINTABLE); };
   return;
#endif

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

   if (option_respect_newline && c == '\n') {
      class_change(NORMAL);
      putchar(c); }
}

int
cdump(void)
{
   int c;

   while (c = getchar(), c != EOF)
      out(c);

   class_change(NORMAL);

   putchar('\n');
   return 0;
}

void
parse_option(int argc, char *argv[])
{
   int o;

   while (o = getopt(argc, argv, "r"), o != -1)
      switch (o) {
      case 'r':
         option_respect_newline = 1;
         break; }
}

int
main(int argc, char *argv[])
{
   parse_option(argc, argv);

   return cdump();
}
