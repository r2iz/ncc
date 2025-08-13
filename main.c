#include "ncc.h"

char *read_file(char *path) {
    FILE *fp = fopen(path, "r");
    if (!fp) error("cannot open %s: %s", path, strerror(errno));

    if (fseek(fp, 0, SEEK_END) == -1)
        error("%s: fseek: %s", path, strerror(errno));

    size_t size = ftell(fp);
    if (fseek(fp, 0, SEEK_SET) == -1)
        error("%s: fseek: %s", path, strerror(errno));

    char *buf = calloc(1, size + 2);
    fread(buf, size, 1, fp);

    if (ferror(fp)) {
        error("%s: fread: %s", path, strerror(errno));
    }

    if (size == 0 || buf[size - 1] != '\n') buf[size++] = '\n';
    buf[size] = '\0';
    fclose(fp);
    return buf;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        error("Usage: %s <source file>", argv[0]);
        return 1;
    }
    user_input = read_file(argv[1]);
    if (!user_input) {
        error("Failed to read file: %s", argv[1]);
        return 1;
    }

    token = tokenize(user_input);
    Node *node = program();

    codegen(node);
    return 0;
}