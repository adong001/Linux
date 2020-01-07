#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

void func1()
{
    printf("this is func1\n");
}

int func2(int* a,int* b)
{
    printf("this is func2\n");
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void func3(int a,int b)
{
    printf("this is func3\n");
    printf("a = %d\nb = %d",a,b);
}

int main()
{

    int a = 10,b = 20;
    func1();
    func3(a,b);
    func2(&a,&b);
    func3(a,b);
    func1();
    return 0;
}
