# NAME

cdump - a dump program which displays characters with different colours

# SYNOPSIS

`cdump` [options] `<`*file*

# DESCRIPTION

`cdump` reads standard input and displays it with colours.
It always shows one byte with one coloured character.

# OPTIONS

  - `-r` respect newline
  - `-d` debug mode
  - `-w`*width* wrap at *width* chars

# EXIT STATUS

cdump always returns with exit status zero.

# EXAMPLE

```
% hexdump -C sample.png
00000000  89 50 4e 47 0d 0a 1a 0a  00 00 00 0d 49 48 44 52  |.PNG........IHDR|
00000010  00 00 00 08 00 00 00 08  01 00 00 00 00 ec 74 83  |..............t.|
00000020  26 00 00 00 15 49 44 41  54 08 99 63 d8 cf 30 9f  |&....IDAT..c..0.|
00000030  a1 9f a1 1d 08 fb 81 ac  fd 00 27 50 04 e9 33 a6  |..........'P..3.|
00000040  a4 25 00 00 00 00 49 45  4e 44 ae 42 60 82        |.%....IEND.B`.|
0000004e
```
<style>
pre { background-color: black; color: grey; }
.ctrl { color: red; }
.printable { color: white; }
.metactrl { color: green; }
.metaprintable { color: yellow; }
</style>
<pre><code>
% cdump -hw16 &lt;sample.png
     0  <span class='metactrl'>I</span><span class='printable'>PNG</span><span class='ctrl'>MJZJ@@@M</span><span class='printable'>IHDR</span><br>    10  <span class='ctrl'>@@@H@@@HA@@@@</span><span class='metaprintable'>l</span><span class='printable'>t</span><span class='metactrl'>C</span><br>    20  <span class='printable'>&amp;</span><span class='ctrl'>@@@U</span><span class='printable'>IDAT</span><span class='ctrl'>H</span><span class='metactrl'>Y</span><span class='printable'>c</span><span class='metaprintable'>XO</span><span class='printable'>0</span><span class='metactrl'>_</span><br>    30  <span class='metaprintable'>!</span><span class='metactrl'>_</span><span class='metaprintable'>!</span><span class='ctrl'>]H</span><span class='metaprintable'>{</span><span class='metactrl'>A</span><span class='metaprintable'>,}</span><span class='ctrl'>@</span><span class='printable'>'P</span><span class='ctrl'>D</span><span class='metaprintable'>i</span><span class='printable'>3</span><span class='metaprintable'>&amp;</span><br>    40  <span class='metaprintable'>$</span><span class='printable'>%</span><span class='ctrl'>@@@@</span><span class='printable'>IEND</span><span class='metaprintable'>.</span><span class='printable'>B`</span><span class='metactrl'>B</span>
</code></pre>
