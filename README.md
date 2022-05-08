# BrainFuck to C

### ⚠️ CodeWars Spoiler Alert

This project is a solution to CodeWars Kata [Brainfuck Translator](https://www.codewars.com/kata/58924f2ca8c628f21a0001a1).

## Execution
Compile using `make`. Use the `run` bash script to execute.
```
USAGE:
    make  o | options   -- display options
    make  d | debug     -- debug build
    make  b | build     -- optimised binary build
    make  c | compiler  -- brainfuck compiler build
    make cl | clean     -- clean ./bin/ directory
    make cf | cleanf    -- forever clean ./bin/

USAGE: run "[Brainfuck code]"
```

## Example
#### Command 1
```bash
$ run "[.[]]"
```
#### Output 1
```c
if (*p) do {
  putchar(*p);
} while (*p);

bfc: [ exited with code 0 ]
```

#### Command 2
```bash
$ run "the bf code to print hello world: ++++++++++[>+++++++>++++++++++>+++>+++++++++<<<<-]>++.>+.+++++++..+++.>++.>---.<<.+++.------.--------.>+."
```

#### Output 2
```c
*p += 10;
if (*p) do {
  p += 1;
  *p += 7;
  p += 1;
  *p += 10;
  p += 1;
  *p += 3;
  p += 1;
  *p += 9;
  p -= 4;
  *p -= 1;
} while (*p);
p += 1;
*p += 2;
putchar(*p);
p += 1;
*p += 1;
putchar(*p);
*p += 7;
putchar(*p);
putchar(*p);
*p += 3;
putchar(*p);
p += 1;
*p += 2;
putchar(*p);
p += 1;
*p -= 3;
putchar(*p);
p -= 2;
putchar(*p);
*p += 3;
putchar(*p);
*p -= 6;
putchar(*p);
*p -= 8;
putchar(*p);
p += 1;
*p += 1;
putchar(*p);

bfc: [ exited with code 0 ]
```
