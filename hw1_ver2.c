#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<math.h>
#include<string.h>
#include<unistd.h>
#define leng 50
#define BUF_SIZE 5

#define min(a,b) ((a) < (b) ? a : b)

typedef struct re_key {
    int e_cpy[leng];
    int d_cpy[leng];
    char ms[leng];
    int index;
}re_key;


re_key rsa(re_key inp, char check);
int prime(int pr);
re_key encryption_key(int t, int x ,int y);
int cd(int a ,int t);
re_key encrypt(int e[],int len,int m[],int n);
re_key decrypt(int d[], re_key inp, int n);
void compareFiles(FILE *fp1, FILE *fp2);


re_key buf[BUF_SIZE]; // the buffer
int len = 0;
int head = 0;
int tail = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // needed to add/remove data from the buffer
pthread_cond_t can_produce = PTHREAD_COND_INITIALIZER; // signaled when items are removed
pthread_cond_t can_consume = PTHREAD_COND_INITIALIZER; // signaled when items are added




typedef struct argument{
    FILE* file;
	int tid;
	int size;
	int num_thread;
} argument;





re_key rsa(re_key inp, char check){
    int i , n;
    int m[leng];
    //long int msg[50];
    int x , y;
    
      

     //long int* res= (int*)malloc(50*sizeof(int));
    x = 19; //first prime number
    y = 11; //second prime number
    //printf("\nENTER MESSAGE OR STRING TO ENCRYPT\n");
    //scanf("%[^\n]s",msg);
 
    //printf("%d",len_inp);

    
    n = x * y;
    int t ; 
    t = (x-1) * (y-1);
    //encryption_key(t,x,y);
    //printf("\nPOSSIBLE VALUES OF e AND d ARE\n");
    //for(i = 0; i < j-1; i++)
        //printf("\n%ld\t%ld", e[i], d[i]);
            
    re_key func_value = encryption_key( t, x ,y);
     if(check =='p'){ 
        for(i = 0; inp.ms[i] !=0; i++){
         m[i] = inp.ms[i];
        //msg[i]=inp.ms[i];  
        //printf(" im msg %c " ,(char)msg[i]); 
        

        }

        
         return encrypt(func_value.e_cpy,i,m,n);

}
    else if(check =='c'){
            return decrypt(func_value.d_cpy,inp,n);      
}
    }


    
int prime(int pr)
{
    int i;
    int j ;
    j = sqrt(pr);
    for(i = 2; i <= j; i++)
    {
        if(pr % i == 0)
            return 0;
    }
    return 1;
}


//function to generate encryption key
re_key encryption_key(int t, int x ,int y)
{
    re_key retky;
    int e[leng],d[leng];
    int i,k,flag;
    k = 0;
    for(i = 2; i < t; i++)
    {
        if(t % i == 0)
            continue;
        flag = prime(i);
        if(flag == 1 && i != x && i != y)
        {
            e[k] = i;
            flag = cd(e[k] , t);
            if(flag > 0)
            {
                d[k] = flag;
                k++;
            }
            if(k == 99)
                break;
        }
    
    }


    memcpy(retky.e_cpy , e , k*sizeof(int));
    memcpy(retky.d_cpy , d,k*sizeof(int));


    return retky;
}


int cd(int a ,int t)
{
    int k = 1;
    while(1)
    {
        k = k + t;
        if(k % a == 0)
            return(k / a);
    }
}



//function to encrypt the message
re_key encrypt(int e[],int len,int m[],int n)
{
    re_key rtk;
    
    int*  en=(int*)malloc(leng*sizeof(int));
    int pt, ct, key = e[0], k;
    int i = 0;
    int j ;
    int  temp[50];
    
    while(i != len)
    {
        pt = m[i];
        pt = pt - 96;
        k = 1;
        for(j = 0; j < key; j++)
        {
            k = k * pt;
            k = k % n;
        }
        temp[i] = k;
        ct = k + 96;
        en[i] = ct;
        i++;
    }
    en[i] = -1;
    rtk.e_cpy[i]=-1;
    rtk.d_cpy[i]=-1;
    //printf("\n\nTHE ENCRYPTED MESSAGE IS\n");
    //  for(i = 0; en[i] != -1; i++)
    //  printf("%c", (char)en[i]);
    //printf("\n");

    memcpy( rtk.e_cpy , en, leng*sizeof(int));
   
    memcpy( rtk.d_cpy ,temp,leng*sizeof(int));
//for(x=0; rtk.d_cpy[x]!=-1;x++){
  //  printf(" im in e %c ",(char)rtk.d_cpy[x]);
//}
//  printf("\n");

    return rtk;
}


//function to decrypt the message
re_key decrypt(int d[], re_key inp, int n){
    re_key dec;
    
    int pt, ct, key = d[0], k;
    int i = 0;
    int j;
    int* m= (int*)malloc(leng*sizeof(int)); 
    while(inp.e_cpy[i] 