#include<stdio.h>
#include<unistd.h>
int main()
{
  char ar[4]={"-\\|/"};
  char src[51]={0};
  int i;
  for(i = 0 ;i <=50;i++)
  {
    src[i]='=';
    printf("[%-50s][%d%%][%c]\r",src,i*2,ar[i%3]);
    fflush(stdout);
    usleep(100000);
  }
  return 0;  
}
