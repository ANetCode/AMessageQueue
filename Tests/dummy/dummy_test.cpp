#include <stdio.h>

void empty_test(void);
int init_test(void) {
    return 0;
}

int cleanup_test(void){
    return 0;
}

int main(int argc, char const *argv[]) {
    printf("hello dummy test\n");
    return 0;
}