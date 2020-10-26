module;
import <cstdio>;

// import <iostream>;
// For some reason <iostream> does not work with clang++/libc++ in llvm 11.
// Compiler says "error: explicit specialization of 'char_traits<char>' must
// be imported from one of the following modules before it is required:
// std.__string std.iosfwd". importing <iosfwd> did not fix the issue.

export module io;

export void println(double value)
{
    std::printf("%g\n", value);
}
