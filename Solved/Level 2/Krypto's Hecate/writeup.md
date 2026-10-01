# Hecate

- Platform: Linux
- Architecture: x86-64
- Language: C/C++
- Difficulty: Level 2
- Source: [crackmes.one](https://crackmes.one/)

## Initial analysis

I started by checking the binary with `file` and `strings`.

I was mainly looking for something obvious like a hardcoded password, serial, or useful strings that could point directly to the check. Nothing really useful showed up, so I opened the binary in Ghidra.

The program asks for:

```text
name:
serial:
```

The interesting part was what happened to the name after it was entered.

## Finding the hash

While going through the main function, I found this loop:

```c
uVar4 = 0x811C9DC5;

do {
    bVar1 = *pbVar14;
    pbVar14 = pbVar14 + 1;
    uVar4 = (uVar4 ^ bVar1) * 0x01000193;
} while (pbVar14 != pbVar9);
```

The two constants immediately stood out:

```text
0x811C9DC5
0x01000193
```

These are the standard constants for 32-bit FNV-1a.

The operation order also matched FNV-1a:

```text
hash = hash XOR byte
hash = hash * 0x01000193
```

So the name is hashed byte by byte using FNV-1a.

## Further processing

The resulting hash wasn't used directly as the serial.

The program also takes the length of the name and mixes it into the hash:

```c
uVar12 = (ushort)(sVar11 * 0x1234) ^ uVar4;
uVar4 = uVar12 * 3 + (uVar4 >> 0xb);
```

Here `sVar11` is the name length converted to a 16-bit value.

The first line takes the lower 16 bits of the multiplication result and XORs it with the hash.

The second line then uses the new `uVar12` together with the original hash shifted right by 11 bits.

## Building the serial

The program eventually takes three 16-bit values.

The first two come from the lower 16 bits of `uVar12` and `uVar4`:

```c
uVar12 & 0xFFFF
uVar4 & 0xFFFF
```

The third one is:

```c
(uVar12 & 0xFFFF) ^
(uVar4 & 0xFFFF) ^
0xC0DE
```

The three values are then passed to:

```c
"%04X-%04X-%04X"
```

So the expected serial has the format:

```text
XXXX-XXXX-XXXX
```

Each `XXXX` represents a 16-bit value printed as uppercase hexadecimal.

The entered serial is also converted to uppercase before the final comparison, and `memcmp()` is used to compare it with the generated serial.

If everything matches:

```text
Access granted.
```

Otherwise:

```text
Access denied.
```

## Keygen

Instead of trying to work directly with Ghidra's decompiled C, I recreated the relevant algorithm in a small C++ program.

```cpp
#include <iostream>
#include <string>
#include <cstdint>
#include <cstdio>

int main() {
    std::string name;

    std::cout << "name: ";
    std::getline(std::cin, name);

    // FNV-1a
    uint32_t hash = 0x811C9DC5;

    for (size_t i = 0; i < name.length(); i++) {
        hash = (hash ^ static_cast<uint8_t>(name[i])) * 0x01000193;
    }

    // Name length
    int16_t length = static_cast<int16_t>(name.length());

    uint32_t original_hash = hash;

    // Further processing
    uint32_t uVar12 =
        static_cast<uint16_t>(length * 0x1234) ^ original_hash;

    hash = uVar12 * 3 + (original_hash >> 11);

    // Three 16-bit values
    uint16_t part1 = uVar12 & 0xFFFF;
    uint16_t part2 = hash & 0xFFFF;

    uint16_t part3 =
        (uVar12 & 0xFFFF) ^
        (hash & 0xFFFF) ^
        0xC0DE;

    printf("%04X-%04X-%04X\n", part1, part2, part3);

    return 0;
}
```

I compiled the keygen and tested the generated serial against the original crackme. It was accepted.

## What I learned

This one took me considerably longer than the Level 1 crackmes. The main reason was that the decompiled code had a lot of C++/`std::string` related stuff that made the actual logic harder to see.

The main things I took away from this one:

- Recognizing FNV-1a from its constants and operation order.
- Not blindly trusting Ghidra's variable names.
- Following the data flow from the input instead.
- Understanding 16-bit vs 32-bit integer operations.
- Getting more comfortable with XOR, masks and right shifts.
- Understanding what `& 0xFFFF` is doing.
- Reconstructing the algorithm instead of trying to read the decompiler output as if it were the original source.
- Writing a small keygen to verify that my understanding was actually correct.

The overall flow ended up being:

```text
Name
  ↓
FNV-1a hash
  ↓
Mix hash with name length
  ↓
Generate three 16-bit values
  ↓
Format as XXXX-XXXX-XXXX
  ↓
Compare with entered serial
  ↓
Access granted / denied
```

This was definitely a step up from the Level 1 crackmes, but getting the keygen to work was a good confirmation that I had actually understood the check.
