Crackme name= level1_Gemcrackme  
Platform= Linux  
Architecture= x86-64  
Difficulty= level 1  
Tools used= Ghidra, Linux terminal, GNU compiler  

## 1. Initial Reconnaissance
The binary level1_gemcrackme was inspected in the Linux terminal using the file utility to confirm its architecture:  
```terminal
file level1_gemcrackme  
```  

```Output
level1_gemcrackme: ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked...
```
To make the target binary runnable, execute permissions were granted via chmod:  
```
chmod +x level1_gemcrackme
```  
## 2. Static Analysis in Ghidra
The binary was imported into Ghidra for decompilation and control-flow analysis.

Decompiled main Function
Looking at main, the program validates command-line arguments and passes argv[1] to check_key:  
```C
int main(int argc, char **argv)
{
    int iVar1;
    
    if (argc < 2) {
        printf("Usage: %s <license_key>\n", *argv);
        iVar1 = 1;
    }
    else {
        iVar1 = check_key(argv[1]);
        if (iVar1 == 0) {
            puts("Access Denied! Invalid Key.");
        }
        else {
            puts("Access Granted! License Key Validated.");
        }
        iVar1 = 0;
    }
    return iVar1;
}
```  


Decompiled check_key Function
Analyzing check_key revealed four key validation requirements:  
```c 
int check_key(char *input)
{
    int iVar1;
    size_t size;
    int sum;
    int i;
    
    size = strlen(input);
    if (size == 8) {
        sum = 0;
        for (i = 0; i < 8; i = i + 1) {
            sum = sum + input[i];
        }
        if (((sum == 0x2a0) && (*input == 'C')) && (input[7] == 'K')) {
            iVar1 = 1;
        }
        else {
            iVar1 = 0;
        }
    }
    else {
        iVar1 = 0;
    }
    return iVar1;
}
```
## 3. Reverse Engineering the Key Constraints
From the static analysis of check_key, the key generation constraints were extracted:

Length Check: strlen(input) == 8 (Key must be exactly 8 characters long).
First Character: *input == 'C' (Index 0 is 'C', ASCII value 67).
Last Character: input[7] == 'K' (Index 7 is 'K', ASCII value 75).
Total Sum Check: sum == 0x2a0 (0x2a0 in hexadecimal = 672 in decimal).

## 4. Verification & Patching Solutions
Solution 1: Valid Key Input
Executing the unpatched binary with the calculated valid key:

./level1_gemcrackme CXXXXXZK

Output:
Access Granted! License Key Validated.

Solution 2: Binary Patching (Bypassing Check)
To bypass the validation logic entirely so that any input string yields access:

1. In Ghidra's Listing window inside main, locate the conditional jump instruction following check_key:
00101264 85 c0    TEST  EAX, EAX
00101266 74 11    JZ    LAB_00101279

2. The JZ LAB_00101279 instruction branches to "Access Denied!" when check_key returns 0 (EAX == 0).
3. Patch the instruction using Ctrl + Shift + G by replacing JZ LAB_00101279 with NOP.
4. Ghidra replaces the 2-byte instruction (74 11) with two NOP instructions (90 90):
00101266 90 90    NOP

5. Export the patched program: File -> Export Program... -> Format: Original File, saved as level1_gemcrackme_patched.

Patch Verification:
chmod +x level1_gemcrackme_patched
./level1_gemcrackme_patched wrongkey

Output:
Access Granted! License Key Validated.

### 5. Flag / Conclusion
Valid Key: CXXXXXZK
Patch Logic: Patched 0x00101266 conditional branch JZ (74 11) to NOP (90 90), bypassing the key check entirely.
Key Calculation
Known character sum: 'C' (67) + 'K' (75) = 142
Required sum for middle 6 characters (indices 1 through 6): 672 - 142 = 530
Average ASCII value for middle 6 characters: 530 / 6 ≈ 88.33 (Close to 'X', ASCII 88)
Filling 5 slots with 'X' (88) and 1 slot with 'Z' (90): 88 + 88 + 88 + 88 + 88 + 90 = 530

Valid Key: CXXXXXZK
