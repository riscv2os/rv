char name[]="ccc";
int age = 55;
char buffer[100];

int add(int a, int b) {
    return a+b;
}

int sum(int n) {
    int s = 0;
    for (int i=1; i<=n; i++)
        s+=i;
    return s;
}

int main() {
    int s = sum(10);
}
