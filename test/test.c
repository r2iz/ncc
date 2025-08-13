int assert(int expected, int actual, const char *code) {
    if (expected == actual) {
        printf("Test passed: %s => %d\n", code, actual);
        return 0;
    } else {
        printf("Test failed: %s => expected %d, got %d\n", code, expected,
               actual);
        exit(1);
    }
}

int x;
int xarr[5];
int x4[4];
char g;
char garr[3];

int ret3() { return 3; }
int ret5() { return 5; }
int retsum(int x, int y) { return x + y; }
int add2(int a, int b) { return a + b; }
int fib(int n) {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}

int main() {
    if (0 != 0) return 1;
    if (42 != 42) return 2;
    if (21 != (5 + 20 - 4)) return 3;
    if (41 != (12 + 34 - 5)) return 4;
    if (47 != (5 + 6 * 7)) return 5;
    if (15 != (5 * (9 - 6))) return 6;
    if (4 != ((3 + 5) / 2)) return 7;
    if (10 != (-10 + 20)) return 8;
    if (10 != (- -10)) return 9;
    if (10 != (- -+10)) return 10;

    if (0 != (0 == 1)) return 11;
    if (1 != (42 == 42)) return 12;
    if (1 != (0 != 1)) return 13;
    if (0 != (42 != 42)) return 14;
    if (1 != (0 < 1)) return 15;
    if (0 != (1 < 1)) return 16;
    if (0 != (2 < 1)) return 17;
    if (1 != (0 <= 1)) return 18;
    if (1 != (1 <= 1)) return 19;
    if (0 != (2 <= 1)) return 20;
    if (1 != (1 > 0)) return 21;
    if (0 != (1 > 1)) return 22;
    if (0 != (1 > 2)) return 23;
    if (1 != (1 >= 0)) return 24;
    if (1 != (1 >= 1)) return 25;
    if (0 != (1 >= 2)) return 26;

    int a;
    a = 3;
    if (a != 3) return 27;
    int b;
    int c;
    b = 3;
    c = 5;
    if (b + c != 8) return 28;

    if (0) return 29;
    if (1 - 1) return 30;
    if (1) {
        if (2 != 2) return 31;
    }
    if (2 - 1) {
        if (2 != 2) return 32;
    }

    int i = 0;
    while (i < 10) i = i + 1;
    if (i != 10) return 33;

    int j = 0;
    for (i = 0; i <= 10; i = i + 1) j = i + j;
    if (j != 55) return 34;

    int arr[2];
    arr[0] = 3;
    arr[1] = 5;
    if (arr[0] != 3) return 35;
    if (arr[1] != 5) return 36;

    int *p;
    a = 7;
    p = &a;
    if (*p != 7) return 37;

    char ch;
    ch = 'a';
    if (ch != 97) return 38;

    char ca[2];
    ca[0] = 'x';
    ca[1] = 'y';
    if (ca[0] != 120) return 39;
    if (ca[1] != 121) return 40;

    return 0;
}
