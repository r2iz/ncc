#!/bin/bash
cat <<EOF | gcc -xc -c -o tmp2.o -
int ret3() { return 3; }
int ret5() { return 5; }
int retsum(int x, int y) { return x + y; }
EOF


assert() {
  expected="$1"
  code="$2"
  tmpfile="./test/tmp_test.c"
  echo "$code" > "$tmpfile"
  ./ncc "$tmpfile" > tmp.s
  gcc -static -o tmp tmp.s tmp2.o
  ./tmp
  actual="$?"
  rm -f "$tmpfile"

  if [ "$actual" = "$expected" ]; then
    echo "$code => $actual"
  else
    echo "$code => $expected expected, but got $actual"
    exit 1
  fi
}


assert 0 'int main() { return 0; }'
assert 42 'int main() { return 42; }'
assert 21 'int main() { return 5+20-4; }'
assert 41 'int main() { return  12 + 34 - 5 ; }'
assert 47 'int main() { return 5+6*7; }'
assert 15 'int main() { return 5*(9-6); }'
assert 4 'int main() { return (3+5)/2; }'
assert 10 'int main() { return -10+20; }'
assert 10 'int main() { return - -10; }'
assert 10 'int main() { return - - +10; }'
assert 1  'int main() { return 5 % 2; }'
assert 61 'int main() { return 161 % 100; }'

assert 0 'int main() { return 0==1; }'
assert 1 'int main() { return 42==42; }'
assert 1 'int main() { return 0!=1; }'
assert 0 'int main() { return 42!=42; }'

assert 1 'int main() { return 0<1; }'
assert 0 'int main() { return 1<1; }'
assert 0 'int main() { return 2<1; }'
assert 1 'int main() { return 0<=1; }'
assert 1 'int main() { return 1<=1; }'
assert 0 'int main() { return 2<=1; }'

assert 1 'int main() { return 1>0; }'
assert 0 'int main() { return 1>1; }'
assert 0 'int main() { return 1>2; }'
assert 1 'int main() { return 1>=0; }'
assert 1 'int main() { return 1>=1; }'
assert 0 'int main() { return 1>=2; }'

assert 3 'int main() { int a; a=3; return a; }'
assert 8 'int main() { int a; int z; a=3; z=5; return a+z; }'

assert 1 'int main() { return 1; 2; 3; }'
assert 2 'int main() { 1; return 2; 3; }'
assert 3 'int main() { 1; 2; return 3; }'

assert 3 'int main() { int foo; foo=3; return foo; }'
assert 8 'int main() { int foo123; int bar; foo123=3; bar=5; return foo123+bar; }'

assert 3 'int main() { if (0) return 2; return 3; }'
assert 3 'int main() { if (1-1) return 2; return 3; }'
assert 2 'int main() { if (1) return 2; return 3; }'
assert 2 'int main() { if (2-1) return 2; return 3; }'

assert 3 'int main() { {1; {2;} return 3;} }'

assert 10 'int main() { int i; i=0; while(i<10) i=i+1; return i; }'
assert 55 'int main() { int i; int j; i=0; j=0; while(i<=10) {j=i+j; i=i+1;} return j; }'

assert 55 'int main() { int i; int j; i=0; j=0; for (i=0; i<=10; i=i+1) j=i+j; return j; }'
assert 3 'int main() { for (;;) return 3; return 5; }'

assert 3 'int main() { return ret3(); }'
assert 5 'int main() { return ret5(); }'
assert 8 'int main() { return retsum(3, 5); }'
assert 8 'int add2(int a, int b) { return a + b; } int main() { return add2(3, 5); }'
assert 21 'int fib(int n) { if (n <= 1) return n; return fib(n-1) + fib(n-2); } int main() { return fib(8); }'

assert 3 'int main() { int a; int b; a=3; b=&a; return *b; }'
assert 3 'int main() { int *p; int a; a=3; p=&a; return *p; }'
assert 8 'int main() { int *p; int *q; int a; int b; a=3; b=5; p=&a; q=&b; return *p + *q; }'

assert 4 'int main() { int x; return sizeof(x); }'

assert 8 'int main() { int a[2]; return sizeof(a); }'
assert 4 'int main() { int a[1]; a[0] = 4; return a[0]; }'
assert 4 'int main() { int a[1]; a[0] = 4; return *a; }'

assert 0 'int x; int main() { return x; }'
assert 3 'int x; int main() { x=3; return x; }'
assert 0 'int x[4]; int main() { x[0]=0; x[1]=1; x[2]=2; x[3]=3; return x[0]; }'
assert 1 'int x[4]; int main() { x[0]=0; x[1]=1; x[2]=2; x[3]=3; return x[1]; }'
assert 2 'int x[4]; int main() { x[0]=0; x[1]=1; x[2]=2; x[3]=3; return x[2]; }'
assert 3 'int x[4]; int main() { x[0]=0; x[1]=1; x[2]=2; x[3]=3; return x[3]; }'

assert 4 'int x; int main() { return sizeof(x); }'
assert 20 'int x[5]; int main() { return sizeof(x); }'

assert 97 "int main() { return 'a'; }"
assert 65 "int main() { return 'A'; }"
assert 48 "int main() { return '0'; }"
assert 10 "int main() { return '\\n'; }"
assert 9 "int main() { return '\\t'; }"
assert 0 "int main() { return '\\0'; }"
assert 39 "int main() { return '\\''; }"
assert 92 "int main() { return '\\\\'; }"

assert 97 "int main() { char a; a='a'; return a; }"
assert 65 "int main() { char a; char b; a='A'; b='B'; return a; }"
assert 66 "int main() { char a; char b; a='A'; b='B'; return b; }"
assert 131 "int main() { char a; char b; a='A'; b='B'; return a+b; }"

assert 1 "int main() { char x; return sizeof(x); }"
assert 1 "int main() { return sizeof(char); }"

assert 5 "int main() { char a[5]; return sizeof(a); }"
assert 97 "int main() { char a[2]; a[0]='a'; a[1]='b'; return a[0]; }"
assert 98 "int main() { char a[2]; a[0]='a'; a[1]='b'; return a[1]; }"

assert 97 "int main() { char a; char *p; a='a'; p=&a; return *p; }"
assert 98 "int main() { char a; char b; char *p; a='a'; b='b'; p=&b; return *p; }"

assert 0 "char g; int main() { return g; }"
assert 120 "char g; int main() { g='x'; return g; }"
assert 3 "char g[3]; int main() { g[0]='a'; g[1]='b'; g[2]='c'; return sizeof(g); }"

assert 0 "// comment
int main() { return 0; }"

assert 97 'int main() { return "abc"[0]; }'
assert 98 'int main() { return "abc"[1]; }'
assert 99 'int main() { return "abc"[2]; }'
assert 0 'int main() { return "abc"[3]; }'
assert 4 'int main() { return sizeof("abc"); }'

assert 0 'void foo() {} int main() { foo(); return 0; }'

echo OK
