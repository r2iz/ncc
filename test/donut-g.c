char palette[13];

int m(int a, int b) { return (a * b + 5000) / 10000; }

void a(int* c, int* s, int d, int t) {
    int k;
    int l;
    k = m(*c, d) - m(*s, t);
    l = m(*s, d) + m(*c, t);
    *c = k;
    *s = l;
}

int main() {
    palette[0] = '.';
    palette[1] = ',';
    palette[2] = '-';
    palette[3] = '~';
    palette[4] = ':';
    palette[5] = ';';
    palette[6] = '=';
    palette[7] = '!';
    palette[8] = '*';
    palette[9] = '#';
    palette[10] = '$';
    palette[11] = '@';
    palette[12] = 0;

    int z[1760];
    char b[1760];
    printf("\033[2J");
    int s;
    s = 10000;
    int q;
    q = s;
    int r;
    r = 0;
    int u;
    u = s;
    int v;
    v = 0;

    for (;; a(&q, &r, s - 8, 400), a(&u, &v, s - 2, 200)) {
        memset(b, 32, 1760);
        memset(z, 0, 1760 * sizeof(q));
        int l;
        l = 0;
        int p;
        p = s;

        int i;
        i = 0;
        for (; i < 88; i = i + 1, a(&p, &l, 9974 + i % 2, 714)) {
            int w;
            w = 0;
            int e;
            e = s;

            int j;
            j = 0;
            for (; j < 314; j = j + 1, a(&e, &w, s - 2, 200)) {
                int f;
                f = p + 2 * s;
                int g;
                g = s * s / (m(m(w, f), r) + m(l, q) + 5 * s);
                int t;
                t = m(m(w, q), f) - m(l, r);
                int x;
                x = 40 + 30 * m(g, m(m(e, u), f) - m(t, v)) / s;
                int y;
                y = 12 + 15 * m(g, m(m(e, v), f) + m(t, u)) / s;
                int o;
                o = x + 80 * y;
                int N;
                N = 8 *
                    (m(m(l, r) - m(m(w, q), p), u) - m(m(w, r), p) - m(l, q) -
                     m(m(e, v), p)) /
                    s;

                if (y > 0) {
                    if (g > z[o]) {
                        if (22 > y) {
                            if (x > 0) {
                                if (80 > x) {
                                    z[o] = g;
                                    if (N >= 1) {
                                        b[o] = palette[N];
                                    } else {
                                        b[o] = palette[0];
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        printf("\033[H");
        int k;
        k = 0;
        for (; k < 1761; k = k + 1) {
            if (k % 80) {
                putchar(b[k]);
            } else {
                putchar(10);
            }
        }
        usleep(5 * s);
    }
    return 0;
}