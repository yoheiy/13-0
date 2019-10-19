#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int option_debug;
int option_html;
int option_line_number;
int option_respect_newline;
int option_without_offset;
int option_width;
int global_bytes;
int global_col;
int global_line = 1;

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
html(int cls)
{
   const char *s;

   switch (cls) {
   case CTRL:
      s = "ctrl";
      break;
   case PRINTABLE:
      s = "printable";
      break;
   case META_CTRL:
      s = "metactrl";
      break;
   case META_PRINTABLE:
      s = "metaprintable";
      break;
   default:
      return; }
   printf("<span class='%s'>", s);
}

void
html_close(void)
{
   fputs("</span>", stdout);
}

void
class_change(int cls)
{
   static int prev_cls = NORMAL;

   if (cls == prev_cls) return;

   if (option_debug) { // DEBUG
#define SHOW_CLASS(x) case x: printf("{%s}", #x); break;
      switch (cls) {
         SHOW_CLASS(NORMAL);
         SHOW_CLASS(CTRL);
         SHOW_CLASS(PRINTABLE);
         SHOW_CLASS(META_CTRL);
         SHOW_CLASS(META_PRINTABLE); };
      return; }

   if (option_html) {
      if (prev_cls != NORMAL)
         html_close();
      html(cls); }
   else
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

   prev_cls = cls;
}

void
put(char c)
{
   if (option_html)
#define ENTITY(x, y) case x: fputs(y, stdout); break;
      switch (c) {
         ENTITY('&', "&amp;");
         ENTITY('<', "&lt;");
         ENTITY('>', "&gt;");
         ENTITY('\n', "<br>");
      default:
         putchar(c);
         break; }
   else
      putchar(c);
}

void
show_offset(void)
{
   if (option_line_number)
      printf("%6d  ", global_line);
   if (!option_without_offset)
      printf("%6x  ", global_bytes);
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

   global_bytes++;
   global_col++;
   if (c == '\n')
      global_line++;

   if ((option_respect_newline && c == '\n') ||
       (option_width && global_col == option_width)) {
      class_change(NORMAL);
      global_col = 0;
      put('\n');
      show_offset(); }
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

   while (o = getopt(argc, argv, "Wdhlrw:"), o != -1)
#define OPTION(x, y) case x: option_ ## y = 1; break;
#define OPTARG(x, y) case x: option_ ## y = atoi(optarg); break;
      switch (o) {
         OPTION('W', without_offset);
         OPTION('d', debug);
         OPTION('h', html);
         OPTION('l', line_number);
         OPTION('r', respect_newline);
         OPTARG('w', width); }
}

int
main(int argc, char *argv[])
{
   parse_option(argc, argv);
   show_offset();

   return cdump();
}
