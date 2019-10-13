#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int option_respect_newline;
int option_debug;
int option_width;
int global_col;

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

   if (option_debug) { // DEBUG
#define SHOW_CLASS(x) case x: printf("{%s}", #x); break;
      switch (cls) {
         SHOW_CLASS(NORMAL);
         SHOW_CLASS(CTRL);
         SHOW_CLASS(PRINTABLE);
         SHOW_CLASS(META_CTRL);
         SHOW_CLASS(META_PRINTABLE); };
      return; }

   switch (cls) {
   case NORMAL:
      esc(NONE);   break;
   case CTRL:
      esc(RED);    break;
   case PRINTABLE:
      esc(WHITE);  break;
   case META_CTRL:
      esc(GREEN);  break;
   case META_PRINTABLE:
      esc(YELLOW); break; }
}

void
put(char c)
{
   putchar(c);
}

void
out(const char c)
{
   const char c7 = c & 0x7f;
   const char c8 = c & 0x80;

   switch (c7) {
   case 0 ... 32:
   case 127:
      class_change(c8 ? META_CTRL : CTRL);
      break;
   default:
      class_change(c8 ? META_PRINTABLE : PRINTABLE);
      break; }

   switch (c7) {
   case 0 ... 31:
      put(c7 + '@'); break;
   case 32:
      put('.'); break;
   case 127:
      put('?'); break;
   default:
      put(c7);  break; }

   global_col++;

   if (option_respect_newline && c == '\n') {
      class_change(NORMAL);
      global_col = 0;
      putchar(c); }

   if (option_width && global_col == option_width) {
      class_change(NORMAL);
      global_col = 0;
      putchar('\n'); }
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

   while (o = getopt(argc, argv, "drw:"), o != -1)
      switch (o) {
      case 'd':
         option_debug = 1;
         break;
      case 'r':
         option_respect_newline = 1;
         break;
      case 'w':
         option_width = atoi(optarg);
         break; }
}

int
main(int argc, char *argv[])
{
   parse_option(argc, argv);

   return cdump();
}
