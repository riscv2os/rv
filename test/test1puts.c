int temp;

#define PUTS(str) __asm__ ( \
        "li a1, 1\n" \
        "mv %1, a2\n" \
        "ecall\n" \
        : "=r"(temp) \
        : "r"(str) \
    )

int sum(int n) {
    int s = 0;
    for (int i=1; i<=n; i++)
        s+=i;
    return s;
}

int main() {
    int s = sum(10);
    PUTS("hello");
    temp = s;
}
