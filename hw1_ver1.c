#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 5

int buf[BUF_SIZE]; // the buffer
int len = 0;
int head = 0;
int tail = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // needed to add/remove data from the buffer
pthread_cond_t can_produce = PTHREAD_COND_INITIALIZER; // signaled when items are removed
pthread_cond_t can_consume = PTHREAD_COND_INITIALIZER; // signaled when items are added

void compareFiles(FILE *fp1, FILE *fp2);


typedef struct argument{
    File* file;
	int tid;
	int size;
	int num_thread;
} argument;

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

// consume random numbers
void* consumer(void *arg) {

	argument targ = *(argument *)arg;
    File out = targ.file;
    int id = targ.id;

#ifdef OVERFLOW
        //show that the buffer won't overflow if the consumer
        // is slow (i.e. the producer will wait)
        sleep(rand() % 3);
#endif 

        char chunk[50];
        pthread_mutex_lock(&mutex);

        while(len == 0) { // empty
            // wait for new items to be appended to the buffer
            pthread_cond_wait(&can_consume, &mutex);
        }

        // grab data
        int i;
        for(i=0; i<50; i++){
            chunk[i] = (char) buff[tail];
            tail = (tail + 1)%BUF_SIZE;
		    len--;    
            printf("q%ld, index %d: Consumed: %d\n", id, tail, buf[tail]);
        }
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
