#include <stdio.h>
#include <stdlib.h>

int main()
{
   float a,b;
   char op,x;
  STAR: printf("Please enter the expression:\n");
    scanf("%f %c%f",&a,&op,&b );
   switch(op)
   {
   case'+':
    printf("%f + %f = %f\n",a,b,a+b);break;
   case'-':
    printf("%f - %f = %f\n",a,b,a-b);break;
   case'*':
    printf("%f * %f = %f\n",a,b,a*b);break;
   case'/':
       if(b==0) printf("Division by zero!\n");
       else printf("%f  %f = %f\n",a,b,a/b);break;
   default:
    printf("Unknown operator!\n");
   }
   printf("Do you want to continue(Y/N or y/n)?");
   scanf(" %c",&x);
   if(x=='y'||x=='Y') goto STAR;
        else return;

   }
