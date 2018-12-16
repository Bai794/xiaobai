#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n,i,a,sum=0;
    printf("input n\n");
    scanf("%ld",&n);
    for(i=1;;i++){
       a=i*i*i;
       sum+=a;
       if(sum>=n){
        printf("%d",i-1);
        break;
       }

    }

}
