#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int starts_with(const char *s, const char *p) {
    return strncmp(s, p, strlen(p)) == 0;
}

void trim(char *s) {
    int len = strlen(s);
    while (len > 0 && isspace(s[len - 1])) s[--len] = 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("usage: bioc <file.bc>\n");
        return 1;
    }

    FILE *in = fopen(argv[1], "r");
    if (!in) {
        printf("error: cannot open file\n");
        return 1;
    }

    FILE *out = fopen("out.c", "w");

    // AUTO-INCLUDED HEADERS
    fprintf(out, "#include <stdio.h>\n");
    fprintf(out, "#include <stdlib.h>\n");
    fprintf(out, "#include <string.h>\n");
    fprintf(out, "#include <stdbool.h>\n\n");

    // We postpone writing main() until we see user code
    fprintf(out, "/* --- BioC transpiled code --- */\n\n");

    char line[2048];
    int main_started = 0;

    while (fgets(line, sizeof(line), in)) {
        trim(line);
        if (strlen(line) == 0) continue;

        // CUSTOM HEADER IMPORT
        if (starts_with(line, "add ")) {
            char header[256];
            sscanf(line, "add %255[^;];", header);
            fprintf(out, "#include \"%s\"\n", header);
            continue;
        }

        // ---------------- FUNCTION DECLARATION ----------------
if (strstr(line, ") -> ")) {
    char rettype[64], name[128], args[512];

    // Pattern:   int sum(int a, int b) -> int {
    // Extract pieces
    sscanf(line, "%63s %127[^ (](%511[^)]) -> %63s", rettype, name, args, rettype);

    // Output:  int sum(int a, int b) {
    fprintf(out, "%s %s(%s) {\n", rettype, name, args);

    // Copy function body lines
    while (fgets(line, sizeof(line), in)) {
        if (strstr(line, "}")) break;
        fprintf(out, "%s", line);
    }

    fprintf(out, "}\n\n");
    continue;
}

        // FILE OUTPUT
        if (starts_with(line, "out[")) {
            char file[256], format[1024];

            sscanf(line, "out[%255[^]]]: %1023[^;];", file, format);

            trim(file);
            trim(format);

            fprintf(out,
                "{ FILE *_fp = fopen(\"%s\", \"w\"); fprintf(_fp, %s); fclose(_fp); }\n",
                file, format
            );

            continue;
        }

        // START MAIN IF NOT YET
        if (!main_started) {
            fprintf(out, "int main() {\n");
            main_started = 1;
        }

        // INLINE C
        if (starts_with(line, "inline_c {")) {
            fprintf(out, "/* inline C start */\n");
            while (fgets(line, sizeof(line), in)) {
                if (strstr(line, "}")) break;
                fprintf(out, "%s", line);
            }
            fprintf(out, "/* inline C end */\n");
            continue;
        }

        // VARIABLE DECLARATION
        if (starts_with(line, "var ")) {
            char name[128], type[128], expr[1024];

            sscanf(line, "var %127[^:]: %127[^=]= %1023[^;];",
                   name, type, expr);
            trim(name);
            trim(type);
            trim(expr);

            char ctype[64] = "int";

            if (strcmp(type, "int") == 0) strcpy(ctype, "int");
            else if (strcmp(type, "uint") == 0) strcpy(ctype, "unsigned int");
            else if (strcmp(type, "float") == 0) strcpy(ctype, "float");
            else if (strcmp(type, "uflt") == 0) strcpy(ctype, "double");
            else if (strcmp(type, "lfloat") == 0) strcpy(ctype, "double");
            else if (strcmp(type, "llfloat") == 0) strcpy(ctype, "long double");
            else if (strcmp(type, "lli") == 0) strcpy(ctype, "long long");
            else if (strcmp(type, "string") == 0) strcpy(ctype, "char*");
            else if (strcmp(type, "bool") == 0) strcpy(ctype, "bool");
            else strcpy(ctype, type);

            fprintf(out, "%s %s = %s;\n", ctype, name, expr);
            continue;
        }

        // ---------------- echo(x); ----------------
if (starts_with(line, "echo(")) {
    char inside[1024];

    // Capture everything between echo(  and  );
    int start = strlen("echo(");
    int end = strlen(line) - 2; // remove ");"
    
    if (end < start) continue;

    strncpy(inside, line + start, end - start);
    inside[end - start] = 0;

    trim(inside);

    // string?
    if (inside[0] == '"' || inside[0] == '\'')
        fprintf(out, "printf(\"%%s\\n\", %s);\n", inside);
    else
        fprintf(out, "printf(\"%%d\\n\", %s);\n", inside);

    continue;
}

        // fallback raw
        fprintf(out, "%s\n", line);
    }

    if (main_started)
        fprintf(out, "return 0;\n}\n");

    fclose(in);
    fclose(out);

    return 0;
}
