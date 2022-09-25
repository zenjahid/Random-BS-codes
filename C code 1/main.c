#include<stdio.h>

struct stu
{
    int roll;
    char nm[99];
};

int main(void)
{
    struct stu s[99],t;

    int n;

    //pt1

    scanf("%d",&n);

    for (int i = 0; i < n; i++)
    {
        scanf("%d %s",&s[i].roll,&s[i].nm);
    }

    //pt2

    printf("\n\n\nEven Ids\n");

    for (int i = 0; i < n; i++)
    {
        if (s[i].roll%2==0)
        {
            printf("Id = %d\nName = %s\n",s[i].roll,s[i].nm);
        }
        
    }

    printf("\n\n\nOdd Ids\n");

    for (int i = 0; i < n; i++)
    {
        if (s[i].roll%2==1)
        {
            printf("Id = %d\nName = %s\n",s[i].roll,s[i].nm);
        }
        
    }

    //pt3
    
    printf("\n\n\nIn asccending order\n");

    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n-1;j++)
        {
            if(s[j].roll>s[j+1].roll)
            {
                t=s[j];
                s[j]=s[j+1];
                s[j+1]=t;
            }
        }
    }
    

    for (int i = 0; i < n; i++)
    {
        printf("Id = %d\tName = %s\n",s[i].roll,s[i].nm);
    }

    //pt4
int k;

do
{
    scanf("%d",&k);
} while (0>k>n);

printf("Enter Kth student :");
scanf("%d %s",&s[k].roll,&s[k].nm);

printf("Enter K+2th student :");
scanf("%d %s",&s[k+2].roll,&s[k+2].nm);


  //pt5

FILE*fp;

//needs fixing//fixed

fp=fopen("log.txt","w");

if (fp == NULL)
    {
        printf("Could not open file");
        return 0;
    }
   
    for (int i = 0; i < n; i++)
    {
       
        fprintf(fp,"Id = %d\nName = %s\n",s[i].roll,s[i].nm);
    }
    fclose(fp);



//pt6
for (int i = k-1;i<n-1;i++)
{
    s[i]=s[i+1];
}

//pt7
fp=fopen("log.txt","w");

if (fp == NULL)
    {
        printf("Could not open file");
        return 0;
    }
   
    for (int i = 0; i < n-1; i++)
    {
       
        fprintf(fp,"Id = %d\nName = %s\n",s[i].roll,s[i].nm);
    }
    fclose(fp);

 return 0;   
}