#include <stdio.h>
#include <stdlib.h>

int maiassn() {
    int n,i,*ptr,sum=0;
    printf("Enter number of elements:");
    scanf("%d",&n);

    ptr = (int*) malloc(n*sizeof (int));
    if(ptr == NULL){
        printf("error");
        exit(0);
    }
    printf("enter elements: ");
    for(i = 0; i < n; ++i) {
        scanf("%d",ptr+i);
        sum+=*(ptr+i);
    }
    printf("Address of previously allocated memory:\n");
    for (int j = 0; j < n; ++j) {
        printf("%pc\n",ptr+i);
    }

    float avg = (float)sum/(float)n;
    printf("Sum= %d",sum);
    printf("\nAverage= %f",avg);
    return 0;
}
