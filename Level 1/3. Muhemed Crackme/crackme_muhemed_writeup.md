# Crackme muhemed

- Platform: Linux
- Architecture: x86-64
- Language: C/C++
- Difficulty: Level 1
- Source: [crackmes.one](https://crackmes.one/crackme/6a7b401905a9e80a90724367)

## Objective

The program asks for a password.

The goal is to find the password by looking at the assembly and understanding
how the program checks the user input.

## Initial Analysis

Running the program shows that it asks for a password:

```text
---------------------------------------------------------------------------------------
CRACKME 0.1v by muhemmed
---------------------------------------------------------------------------------------
enter the password:
```

If an incorrect password is entered, the program prints:

```text
nope you are bad boy
```

The program continues asking for a password in a loop until the correct
password is provided.

I then opened the binary in Ghidra and inspected the `main()` function.

The decompiled code shows two character arrays:

```c
char key[99];
char input[100];
```

`input` stores the password entered by the user, while `key` stores the
password that the program expects.

The key is initialized with:

```c
builtin_strncpy(key, "wvohXN8X7C14jrq1F*!j", 0x15);
```

The actual key is 20 characters long, while the remaining space in the
buffer is filled with null bytes (`'\0'`).

Later, the program compares the user input with the key:

```c
iVar1 = strcmp(input, key);

if (iVar1 == 0)
    break;
```

`strcmp()` returns `0` when the two strings are identical and a nonzero value
when they are different.

Therefore, the value stored in `key` is the value that needs to be entered
as the password.

## Looking at the Assembly

Instead of relying only on the decompiled C code, I looked at the assembly
instructions responsible for initializing `key`.

The first part is:

```asm
MOV RAX, 0x58384e58686f7677
MOV RDX, 0x3171726a34314337
```

`RAX` is a 64-bit register, so it can hold 64 bits, which is 8 bytes.

The first immediate value is:

```text
0x58384e58686f7677
```

Splitting it into individual bytes gives:

```text
58 38 4e 58 68 6f 76 77
```

x86-64 uses little-endian byte ordering, meaning the least significant byte
is stored first in memory. Therefore, the bytes appear in memory as:

```text
77 76 6f 68 58 4e 38 58
```

Interpreting these bytes as ASCII gives the first 8 characters of the key.

The same process can be applied to the second value:

```text
0x3171726a34314337
```

Splitting it into bytes and accounting for little-endian ordering gives the
next 8 characters of the key.

The assembly then stores these values into the memory locations belonging to
the `key` buffer. Another instruction initializes the remaining 4 characters
of the key.

In other words, the compiler is constructing the hardcoded key in the stack
memory using 64-bit and smaller integer values.

## Password Verification

The important part of the program is the call to:

```c
strcmp(input, key);
```

The function compares the string entered by the user with the string stored
in `key`.

After reconstructing the key from the assembly, I entered it into the
program.

The program accepted the password and printed:

```text
you are cracked program good boy
```

This confirms that the recovered key is the expected password.

## Conclusion

This crackme does not use encryption, hashing, or any password-generation
algorithm.

The expected password is hardcoded into the program and constructed directly
in memory when `main()` starts.

The main reversing step was recognizing that the hexadecimal values loaded
into the 64-bit registers represented ASCII characters when their bytes were
interpreted in little-endian order.

### Key Takeaways

- Used Ghidra to inspect the `main()` function.
- Identified the hardcoded password buffer.
- Identified `strcmp()` as the password verification mechanism.
- Learned how ASCII strings can appear as hexadecimal integer constants in
  assembly.
- Practiced splitting 64-bit values into bytes.
- Practiced accounting for little-endian byte ordering when recovering
  strings from assembly.
