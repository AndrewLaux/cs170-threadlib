#include <stdio.h>

int routine(int a) {
    a = a * 2;
    return a;
};

void exit_rtn(void) {


}

int main() {
    
    int max = 64;
    char array[max];
    printf("The addres pointed to by array[max - 4] is %x\n", &array[max - 4]);
    int* mfour = (int*)&array[max-4];
    printf(" With an int pointer %x\n", mfour);
    long exit_addr = (long)exit_rtn;
    printf("The address of exit_rtn is %x\n", exit_addr);
    *mfour = (int)exit_addr;
    printf("The contents of array[max - 4] is now %x\n", *mfour);

    return 0;

}