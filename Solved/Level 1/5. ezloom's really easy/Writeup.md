# Crackme Writeup

- **Platform:** Linux
- **Architecture:** x86-64
- **Difficulty:** Level 1
- **Tool:** Ghidra
- **Source:** [crackmes.one](https://crackmes.one/)

## Analysis

Opened the binary directly in Ghidra and located the `main` function.

The program first prompts for a password and reads the input using `scanf`:

```asm
LEA RAX,[RBP + -0x40]
MOV RSI,RAX
...
CALL __isoc99_scanf
```

The input is then compared using `strcmp`:

```asm
LEA RAX,[RBP + -0x40]
LEA RDX,[s_iloveicecream]
MOV RSI,RDX
MOV RDI,RAX
CALL strcmp
```

Using the x86-64 calling convention:

- `RDI` = first argument
- `RSI` = second argument

So this is effectively:

```c
strcmp(input, "iloveicecream");
```

`strcmp` returns `0` when the strings match. The following instructions check this:

```asm
TEST EAX,EAX
JNZ  LAB_0010123e
```

If the result is zero, the program prints:

```text
I love ice cream too!
```

## Result

The password was found by tracing the arguments passed to `strcmp`.

```text
iloveicecream
```