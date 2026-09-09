# Crackme Name

- Platform: Linux
- Architecture: x86-64
- Language: C/C++
- Difficulty: level 1
- Source: [crackmes.one](https://crackmes.one/crackme/6a3ce36d39dd6183d62b9dfe)



## Objective

The program asks for a string and a number.
The goal is to determine how the program calculates the correct number
for a given string and to make a keygen.



## Initial Analysis

Running the program shows that it asks for a string followed by a number.

I tested different string lengths and noticed that inputs shorter than 5
or longer than 10 characters are rejected.



## Static Analysis

I opened the binary in Ghidra and located the main function responsible
for processing the input. The calculation function uses a table of integer constants located at
`int_table`. Inspecting this data in Ghidra gave the following values:

4, 79, 129, 171, 254, 123, 224, 204, 70, 53

The important part of the function is:
```c
  if (strobj[1] != 0) {
    index = 0;
    result = 0;
    do {
      character = (char *)(*strobj + index);
      tab_val = &int_table + index;
      index = index + 1;
      result = result + ((int)*character ^ *tab_val);
    } while (strobj[1] != index);
    return result;  
```


The decompiler initially made `strobj[1]` look like an array access.
However, inspecting the corresponding assembly showed that the value
being loaded from the second field of the `std::string` object was the
string length.

This clarified that `strobj` was being interpreted as a pointer to the
`std::string` object rather than a pointer directly to the characters.

## Understanding the Algorithm

The program initializes a result to zero.

For every character in the input:

1. Read the character at the current index.
2. Read the corresponding value from the table.
3. XOR the two values.
4. Add the result to the accumulated total.
5. Continue until the end of the string.

Example using the first character:

character[0] = 'A'
'A' = 65
int_table[0] = 4
65 XOR 4 = 69

result += 69

The next character uses `int_table[1]`, and the process continues
until every character in the input has been processed.

## Keygen

After understanding the validation function, I recreated the algorithm
in C.

The keygen:

1. Generates a random string between 5 and 10 characters.
2. Calculates the expected number using the recovered table.
3. Prints both values.

The source code can be found here:

[View the C source code](./keygen_got_started.c)



## Verification

I ran my keygen and used the generated string and number as input
to the original crackme.

The original program accepted the generated values, confirming that
the reconstructed algorithm was correct.
```bash
./keygen_got_started
String of characters(with no space): qUvOtZ4 
correct number(no spaces): 1001

./getting_started_keygen
Enter a string of characters (no spaces): 
qUvOtZ4 
Enter correct number (no spaces): 
1001
OMG! You did it! :3
```


## What I Learned

- How `std::string` is represented in the generated assembly.
- How to distinguish a string pointer from the string length.
- How Ghidra's decompiler can sometimes make pointer arithmetic confusing.
- How to follow a loop from C-like pseudocode back to assembly.
- How XOR operations can be used as part of a validation algorithm.
- How to reproduce a binary's logic in C.
