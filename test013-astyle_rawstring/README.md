AStyle 3.1.8 has a bug in handling C++11 raw string literals. If a raw string
literal ends with a line `)";`, AStyle stops formatting subsequent code. See
the bug report:

- https://sourceforge.net/p/astyle/bugs/519/

The bug is already fixed in trunc, but it has not been released yet. Sadly, the
project looks dead (at least inactive for 1yr):

- https://sourceforge.net/p/astyle/code/HEAD/tree/
- https://sourceforge.net/p/astyle/bugs/stats/

So, the fix would not be released soon. Lickily, the bug can be worked around
by appending an arbitrary string that ends with `)";` to the end of the raw
string literal. Like this:

```c++
R"(
This is a raw string.
)" R"()";
```

Self-descriptive comment would be clearer and better. Looks uncool though:

```c++
R"(
This is a raw string.
)"
; // Dont't remove. This line makes AStyle <= 3.1.8 happy)";
```
