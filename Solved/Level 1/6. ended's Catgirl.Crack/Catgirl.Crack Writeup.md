# Catgirl.Crack

- Platform: Linux
- Architecture: x86-64
- Language: C#
- Difficulty: Level 1
- Source: [crackmes.one](https://crackmes.one/)

## Analysis

The provided files included a native Linux executable along with several .NET files. Running the executable showed that it required .NET 10.

Since `CatgirlCrack.dll` was identified as a .NET assembly, I used ILSpy to decompile it instead of analyzing the native executable with Ghidra.

The important part was `Program.Main()`:

```csharp
string text = Console.ReadLine();

if (text.Equals("Mint", StringComparison.OrdinalIgnoreCase))
{
    Console.WriteLine("Mrrooww! Good Kitty! Mwwwaaah~!");
}
else if (text.Equals("meow", StringComparison.OrdinalIgnoreCase))
{
    // Wrong
}
else
{
    // Wrong
}
```

The program reads the password using `Console.ReadLine()` and compares it against two strings. The first comparison uses `StringComparison.OrdinalIgnoreCase`, meaning the comparison is case-insensitive.

The success condition therefore reveals the password directly.

## Key Takeaways

- `.NET` executables can often be decompiled back into readable C#.
- For a .NET crackme, identifying the `.dll` and inspecting it with a decompiler can be much faster than starting with assembly.
- When reversing a program, follow the data flow: **input → processing/comparison → success/failure**.
- `StringComparison.OrdinalIgnoreCase` means capitalization does not affect the comparison.