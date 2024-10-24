int sys_status;

#define EXIT(n) __asm__ ( \
        "li a1, 0\n" \
        "mv %1, a2\n" \
        "ecall\n" \
        : "=r"(sys_status) \
        : "r"(n) \
    )

#define PUTS(str) __asm__ ( \
        "li a1, 1\n" \
        "mv %1, a2\n" \
        "ecall\n" \
        : "=r"(sys_status) \
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
    EXIT(1);
}
