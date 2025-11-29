# BioC

BioC is a modern, safe, developer-friendly language that transpiles directly into clean, portable ANSI C.  
It provides a comfortable syntax, safer patterns, and powerful high-level features while preserving the performance and compatibility of traditional C.

BioC acts as a “TypeScript for C”:  
you write expressive BioC code, and the compiler generates predictable plain C.

---

## Features

### Modern, Clean Syntax
BioC introduces a readable style for defining functions, variables, and logic.

```bioc
int sum(int a, int b) -> int {
    return a + b;
}

var x: int = 4;
var y: int = 7;

echo(sum(x, y));
```

### Built-in File Output
BioC provides dedicated syntax for writing to files safely and concisely.

```bioc
out[result.txt]: "Result is %d", sum(x, y);
```

Transpiles to:

```c
FILE *_fp = fopen("result.txt", "w");
fprintf(_fp, "Result is %d", sum(x, y));
fclose(_fp);
```

### Inline C Support
Drop directly into standard C whenever necessary.

```bioc
inline_c {
    printf("Inline C OK!\n");
}
```

### Zero Runtime Overhead
BioC does not add a runtime layer.  
Generated C code is simple, portable, and compatible with any compiler.

---

## Example

### BioC Source

```bioc
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
```

### Generated C Output

```c
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
```

---

