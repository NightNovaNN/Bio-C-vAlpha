BioC

BioC is a modern, safe, developer-friendly language that transpiles directly into clean, portable C.
It aims to make C development simpler, less error-prone, and far more expressive—without sacrificing performance or compatibility.

BioC acts as the TypeScript of C:
you write high-level, comfortable code, and the BioC compiler converts it into normal ANSI C that works anywhere.

✨ Features
• Modern Syntax

BioC introduces clean, readable syntax for functions, variables, and I/O while keeping full low-level control.

int sum(int a, int b) -> int {
    return a + b;
}

var x: int = 4;
var y: int = 7;

echo(sum(x, y));

• File Output Made Simple

Built-in syntax for writing to files safely and concisely.

out[result.txt]: "Result is %d", sum(x, y);


Compiles into standard C:

FILE *_fp = fopen("result.txt", "w");
fprintf(_fp, "Result is %d", sum(x, y));
fclose(_fp);

• Inline C Support

When needed, you can drop into raw C code directly.

inline_c {
    printf("Inline C OK!\n");
}

• 100% Portable Output

BioC transpiles to simple and predictable C—no weird macros, no runtime, no overhead.

🔧 Example Transpilation
BioC Source
int sum(int a, int b) -> int {
    return a + b;
}

var x: int = 4;
var y: int = 7;

echo(sum(x, y));

out[result.txt]: "Result is %d", sum(x, y);

inline_c {
    printf("Inline C OK!\n");
}

Generated C Output
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int sum(int a, int b) {
    return a + b;
}

int main() {
    int x = 4;
    int y = 7;
    printf("%d\n", sum(x, y));

    { 
        FILE *_fp = fopen("result.txt", "w");
        fprintf(_fp, "Result is %d", sum(x, y));
        fclose(_fp);
    }

    /* inline C start */
    printf("Inline C OK!\n");
    /* inline C end */

    return 0;
}

🚀 Why BioC?

Write C-compatible programs faster

More readable syntax

Safer defaults

Zero runtime, zero overhead

Works everywhere C works

Perfect for systems programming, embedded, and high-performance workloads
