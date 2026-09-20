# ASM Crackme — Level 1 Writeup

- Platform: Linux
- Architecture: x86 (32-bit)
- Language: Assembly
- Difficulty: Level 1
- Source: Crackmes.one

## Objective

The program asks for a password and checks whether the supplied input is correct.

The goal is to reverse engineer the password validation logic.

## Analysis

Opening the binary in Ghidra and looking at the `_start` function gives:

```asm
08049000    POP        EBX
08049001    POP        EBX
08049002    POP        EBX

08049003    MOV        EAX,[password]
08049008    CMP        EAX,dword ptr [EBX]
0804900A    JZ         goodjob
0804900C    JMP        wrong
```

The first three `POP EBX` instructions consume three 4-byte values from the initial stack. The important part for the password check is that `EBX` ends up containing an address which is then dereferenced by:

```asm
CMP EAX,dword ptr [EBX]
```

The next instruction is:

```asm
MOV EAX,[password]
```

`EAX` is a 32-bit register, so this instruction loads 32 bits (4 bytes) starting from the address of `password`.

Looking at the `.data` section:

```text
0804a000    50 34 35 35
0804a004    77 30 72 64
```

Converting the bytes to ASCII gives:

```text
50 34 35 35 77 30 72 64
 P  4  5  5  w  0  r  d
```

So the complete string stored in memory is:

```text
P455w0rd
```

However, the program does **not** compare the complete string.

Because `EAX` is 32 bits wide, this instruction:

```asm
MOV EAX,[password]
```

loads only the first four bytes:

```text
P455
```

The comparison:

```asm
CMP EAX,dword ptr [EBX]
```

also compares only 4 bytes.

Therefore, the validation is effectively checking whether the first four input bytes match:

```text
P455
```

It is not performing a string comparison such as `strcmp()`.

## Success and Failure

If the comparison is equal:

```asm
JZ goodjob
```

the program executes the Linux `write` system call and prints:

```text
You Got This!
```

If the comparison fails, execution jumps to `wrong` and prints:

```text
Wrong!
```

The program then exits using the Linux `exit` system call.

## Important Observation

The stored value is:

```text
P455w0rd
```

but only the first four bytes are used during validation.

This is because the program loads and compares a `dword` (32 bits / 4 bytes), rather than comparing the entire string.

Therefore, the input that satisfies the check is:

```text
P455
```

## Conclusion

The crackme uses a very simple 4-byte comparison.

The important instructions are:

```asm
MOV EAX,[password]
CMP EAX,dword ptr [EBX]
JZ  goodjob
```

Since `EAX` is 32 bits wide, only the first four bytes of the stored password are loaded and compared.

The stored string is `P455w0rd`, but the validation only checks:

```text
P455
```

This makes `P455` the required input.

## Key Takeaways

- `EAX` is a 32-bit register, so it can hold 4 bytes.
- `dword` means 32 bits, or 4 bytes.
- `MOV EAX,[address]` loads 4 bytes from that memory address.
- `CMP` here is an integer/byte-level comparison, not a string comparison.
- Assembly does not inherently understand strings; the programmer determines how many bytes are accessed.
- When reversing a crackme, always check the **size of the operands** involved in a comparison.
