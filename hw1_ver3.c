#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<math.h>
#include<string.h>
#include<unistd.h>
#define leng 50
#define BUF_SIZE 5



typedef struct re_key {
     int e_cpy[leng];
     int d_cpy[leng];
    char ms[leng];
}re_key;



re_key rsa(re_key inp, char check);
int prime(int pr);
re_key encryption_key(int t, int x ,int y);
int cd(int a ,int t);
re_key encrypt(int e[],int len,int m[],int n);
re_key decrypt(int d[], re_key inp, int n);
void compareFiles(FILE *fp1, FILE *fp2);


int buf[BUF_SIZE]; // the buffer
int len = 0;
int head = 0;
int tail = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // needed to add/remove data from the buffer
pthread_cond_t can_produce = PTHREAD_COND_INITIALIZER; // signaled when items are removed
pthread_cond_t can_consume = PTHREAD_COND_INITIALIZER; // signaled when items are added




typedef struct argument{
    File* file;
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
    int x ;
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
    while(inp.e_cpy[i] != -1) //inp.e_cpy[i] = en
    {
        ct = inp.d_cpy[i]; // inp.d_cpy[i] = temp
        k = 1;
        for(j = 0; j < key; j++)
        {
            k = k * ct;
            k = k % n;
        }
        pt = k + 96;
        m[i] = pt;
        i++;
    }

    m[i] = -1;
//  printf("\n\nTHE DECRYPTED MESSAGE IS\n");
    for(i = 0; m[i] != -1; i++)
    {
        //printf("%c", (char)m[i]);
        dec.e_cpy[i] =m[i];
    }
    dec.e_cpy[i] =-1;
//  printf("\n");

    return dec;
}




// produce random numbers
void* producer(void *arg) {

	argument targ = *(argument *)arg;
    File file = targ.file;
    int id = targ.id;
    int size = targ.size;

    int chunk_num = size / 50;
    if(size % 50 != 0){
        chunk_num++;
    }
    int chunk_per_thread = chunk_num /targ.num_thread;

    
	int si = (chunk_per_thread * 50) * targ.tid; 
	int ei = min(si + (chunk_per_thread * 50), targ.size);	
    
#ifdef UNDERFLOW
        // used to show that if the producer is somewhat "slow"
        // the consumer will not fail (i.e. it'll just wait
        // for new items to consume)
        sleep(rand() % 3);
#endif

    char chunk[50];  // array of bytes, not pointers-to-bytes
    size_t bytesRead = 0;

    if(file!=null){
        // read up to sizeof(buffer) bytes
        fseek(fp, si, SEEK_SET);
        int end = min(si-ei+1, 50);
        
	    for(int i = si; i < ei; i+=end){
            fread(chunk, 1, sizeof(chunk), file);
          //  convert_chunck_to_enc(chunk);

                re_key rtkk;
                 re_key rtkkk;
                
            int j ;
                char mss[leng]=chunk;
                memcpy(rtkk.ms,mss ,leng*sizeof(int));
                rtkkk=rsa(rtkk,'p');
                printf("here is ");
                for(j=0;rtkkk.e_cpy[j]!=-1;j++){
                    printf("%c",rtkkk.e_cpy[j]);
                }
                printf("\n");
            pthread_mutex_lock(&mutex);

            for(int j=0; j<end; j++){
                while(len == BUF_SIZE) { // full
                    // wait until some elements are consumed
                    pthread_cond_wait(&can_produce, &mutex);
                }

                // in real life it may be some data fetched from
                // sensors, the web, or just some I/O
                printf("p%ld, index %d: Produced: %d\n", id, head, t);

                // append data to the buffer
                buf[head] = chunk[j];
                //++buffer->len;
                head = (head + 1)%BUF_SIZE;
                len++;
                // signal the fact that new items may be consumed
            }
            if(end!=50){
                int k;
                for(k=end; k<50; k++){
                    // append data to the buffer
                    buf[head] = " ";
                    //++buffer->len;
                    head = (head + 1)%BUF_SIZE;
                    len++;
                }
            }
            pthread_cond_signal(&can_consume);
            pthread_mutex_unlock(&mutex);
        }   

    }

	pthread_exit(NULL);
    // never reached
    return NULL;
}

void* consumer(void *arg) {

	argument targ = *(argument *)arg;
    FILE* out = targ.file;
    int id = targ.tid;
    int size = targ.size;
    char filestr[2*size];

#ifdef OVERFLOW
        //show that the buffer won't overflow if the consumer
        // is slow (i.e. the producer will wait)
        sleep(rand() % 3);
#endif 

        pthread_mutex_lock(&mutex);

        while(len == 0) { // empty
            // wait for new items to be appended to the buffer
            pthread_cond_wait(&can_consume, &mutex);
        }

        // grab data
        int i;
        re_key res = buf[tail];
        tail = (tail + 1)%BUF_SIZE;
		len--;    
        printf("q%ld, index %d: Consumed: %d\n", id, tail, buf[tail]);

        re_key decrypted = rsa(res, 'c');
        for(i=0; i<50; i++){
            if(decrypted.e_cpy[i] != NULL)
                filestr[decrypted.index + i] = decrypted.e_cpy[i];
        }



        int chunk_num = size / 50;
        if(size % 50 != 0){
            chunk_num++;
        }
        int chunk_per_thread = chunk_num /targ.num_thread;

        
	    int si = (chunk_per_thread * 50) * targ.tid; 
	    int ei = min(si + (chunk_per_thread * 50), targ.size;

      // signal the fact that new items may be produced
        pthread_cond_signal(&can_produce);
        pthread_mutex_unlock(&mutex);

		fwrite(chunk, 1, sizeof(chunk), out);

    // never reached
    return NULL;
}

int main(int argc, char *argv[]) {
    
    struct timeval start, end;
	pthread_t* threads;
	argument *arguments;
    int i;
	time_t t;

	if(argc != 3){
		printf("Correct way to execute this program is:\n");
		printf("./your-encryption-program number-of-threads input-file-path.\n");
		return 1;
	}

   
	int num_thread = atoi(argv[1]);
    char* address = argv[2];

    FILE *file = NULL;
    file = fopen(address, "rb");

    fseek(file, 0L, SEEK_END); 
  
    // calculating the size of the file 
    int size = ftell(file); 

    // seek back
    fseek(file, 0L, SEEK_SET);
  
    int tmp = num_thread /2;

	threads = (pthread_t *)malloc(sizeof(pthread_t) * num_thread);
	arguments = (argument *)malloc(sizeof(argument) * num_thread);

    gettimeofday(&start, NULL);

	for(int i = 0; i < tmp; i++){
		arguments[i].tid = i;
		arguments[i].file = file;
		arguments[i].size = size;
		arguments[i].num_thread = num_thread;
	
		pthread_create(&threads[i], NULL, producer, (void *)&arguments[i]);
	}

    FILE *file = NULL;
    out = fopen("output.txt", "w");

	for(int i = 0; i < tmp; i++){
		arguments[i].tid = i;
		arguments[i].file = out;
		arguments[i].size = size;
		arguments[i].num_thread = num_thread;
	
		pthread_create(&threads[i], NULL, consumer, (void *)&arguments[i]);
	}
	
	for(i=0;i < num_thread; i++){
		void *retVal;
		pthread_join(threads[i],&retVal);
	}		


    gettimeofday(&end, NULL);

    double diff = (end.tv_sec - start.tv_sec) * 1000000.0 +
        (end.tv_usec - start.tv_usec);

    printf("Duration: %8.5fms\n", diff / 1000);
	
	free((void*) threads);
    FILE *fp1 = fopen("file.txt", "r"); 
    FILE *fp2 = fopen("output.txt", "r"); 
    compareFiles(fp1, fp2);
    fclose(fp1);
    fclose(fp2);
    return 0;

}

void compareFiles(FILE *fp1, FILE *fp2) 
{ 
    // fetching character of two file 
    // in two variable ch1 and ch2 
    char ch1 = getc(fp1); 
    char ch2 = getc(fp2); 
  
    // error keeps track of number of errors 
    // pos keeps track of position of errors 
    // line keeps track of error line 
    int error = 0, pos = 0, line = 1; 
  
    // iterate loop till end of file 
    while (ch1 != EOF && ch2 != EOF) 
    { 
        pos++; 
  
        // if both variable encounters new 
        // line then line variable is incremented 
        // and pos variable is set to 0 
        if (ch1 == '\n' && ch2 == '\n') 
        { 
            line++; 
            pos = 0; 
        } 
  
        // if fetched data is not equal then 
        // error is incremented 
        if (ch1 != ch2) 
        { 
            error++; 
            printf("Line Number : %d \tError"
               " Position : %d \n", line, pos); 
        } 
  
        // fetching character until end of file 
        ch1 = getc(fp1); 
        ch2 = getc(fp2); 
    } 
  
    printf("Total Errors : %d\t", error); 
} 
