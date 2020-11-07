# nush

### Northeastern University Shell

<hr>

#### Unix bash like shell written in C.

```
daniel@daniel:~/cs3650/ch01$ ./nush
nush$ ls
ast.c      hashmap.h      nush        svec.c      tests       tokenize.plist
ast.h      hashmap.o      nush.c      svec.h      tmp         valgrind.out
ast.o      hashmap.plist  nush.h      svec.o      tokenize.c
ast.plist  main.out       nush.o      svec.plist  tokenize.h
hashmap.c  Makefile       nush.plist  test.pl     tokenize.o
nush$ pwd
/home/daniel/cs3650/ch01
nush$ cd tmp
nush$ pwd
/home/daniel/cs3650/ch01/tmp
nush$ exit
```

## Usage

- Accepts an optional script file as the first command line arugment.
- Otherwise a prompt is show and command input is accepted, parsed, and executed.

### Supported Operators
Redirect input
```
nush$ head -n 2 < test.txt
```

Redirect output
```
nush$ echo one > test.txt
```

Pipe
```
nush$ sort test.txt | uniq
```

Background
```
nush$ sleep 10&
```

And
```
nush$ echo one && echo two
```

Or
```
nush$ echo one || echo two
```

Semicolon
```
nush$ echo one; cat test.txt
```
