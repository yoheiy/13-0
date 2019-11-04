#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int option_debug;
int option_html;
int option_line_number;
int option_respect_newline;
int option_without_offset;
int option_width;
int option_xpm;
int option_u8;
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

   if (option_xpm)
      switch (cls) {
      case CTRL:
         putchar('c'); return;
      case PRINTABLE:
         putchar('p'); return;
      case META_CTRL:
         putchar('C'); return;
      case META_PRINTABLE:
         putchar('P'); return; }

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
   else if (option_xpm)
      ;
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
   else if (option_xpm) {
      if (c == '\n') {
         fputs("\",\n\"", stdout);
         for (int i = 0; i < option_width; i++)
            putchar('X');
         fputs("\",\n\"", stdout); } }
   else
      putchar(c);
}

void
show_offset(void)
{
   if (option_xpm) return;
   if (option_line_number)
      printf("%6d  ", global_line);
   if (!option_without_offset)
      printf("%6x  ", global_bytes);
}

int
u8width(char c)
{
   int i = 0;

   for (; c & 0x80; c <<= 1) i++;

   return i;
}

void
out(const char c)
{
   const char c7 = c & 0x7f;
   const char c8 = c & 0x80;

   if (option_u8 && c8) {
      unsigned char x = c;

      if ((x == 0xc0) || (x == 0xc1) || (x > 0xf4)) {
         class_change(META_PRINTABLE);
         put('X'); }
      else {
         class_change(META_CTRL);
         put(u8width(c) + '0'); }
      goto u8; }

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

u8:

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

void
output_xpm_header(void)
{
   fputs("a\n"
         "/* XPM */\n"
         "static char *cdump_xpm[] = {\n"
         "\"X X X X\",\n"
         "\"c c red\",\n"
         "\"p c white\",\n"
         "\"C c green\",\n"
         "\"P c yellow\",\n"
         "\"X c None\",\n"
         "\"", stdout);
}

void
output_xpm_footer(void)
{
   const int rest = option_width - global_bytes % option_width;

   if (global_bytes % option_width || !global_bytes) {
      for (int i = 0; i < rest; i++)
         putchar('X');
      put('\n'); }

   printf("};\n"
          ".\n"
          "3c\n\"%d %d 5 1\",\n.\n"
          "1,$p\n"
          "Q\n",
          option_width,
          2 * ((global_bytes - 1) / option_width + 1));
}

int
cdump(void)
{
   int c;

   if (option_xpm)
      output_xpm_header();
   while (c = getchar(), c != EOF)
      out(c);

   class_change(NORMAL);
   if (option_xpm)
      output_xpm_footer();
   else
      putchar('\n');
   return 0;
}

void
parse_option(int argc, char *argv[])
{
   int o;

   while (o = getopt(argc, argv, "Wdhlruw:x"), o != -1)
#define OPTION(x, y) case x: option_ ## y = 1; break;
#define OPTARG(x, y) case x: option_ ## y = atoi(optarg); break;
      switch (o) {
         OPTION('W', without_offset);
         OPTION('d', debug);
         OPTION('h', html);
         OPTION('l', line_number);
         OPTION('r', respect_newline);
         OPTION('u', u8);
         OPTION('x', xpm);
         OPTARG('w', width); }
   if (option_html && option_xpm) {
      option_xpm = 0;
      fputs("option -x ignored because of -h\n", stderr); }
   if (option_xpm && !option_width)
      option_width = 0x100;
}

int
main(int argc, char *argv[])
{
   parse_option(argc, argv);
   show_offset();

   return cdump();
}
