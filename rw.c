#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define reader_num 5
#define writer_num 2

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t db = PTHREAD_MUTEX_INITIALIZER;
int rc = 0;
int reading_time = 3, writing_time = 12,using_time = 18;

void read_data_base(int reader_id)
{
    printf("number %d reader is reading data base!\n",reader_id);
    sleep(reading_time * (reader_id + 1));
}

void use_data_read(int reader_id)
{
    printf("  number %d reader is using data!\n",reader_id);
    sleep(using_time * (reader_id + 1));

}
void write_data_base(int writer_id)
{
    printf(" number %d writer is writing data base!\n",writer_id);
    sleep(writing_time);
}

void think_up_data(int writer_id)
{
    printf("    number %d writer is thinking up data!\n",writer_id);
    sleep(using_time);
}

void reading(int *reader_id)
{
    while(1){
        pthread_mutex_lock(&mutex);
        rc++;
        if(rc == 1) pthread_mutex_lock(&db);
        pthread_mutex_unlock(&mutex);
        read_data_base(*reader_id);
        pthread_mutex_lock(&mutex);
        rc--;
        if(rc == 0) pthread_mutex_unlock(&db);
        pthread_mutex_unlock(&mutex);
        use_data_read(*reader_id);
    }
}

void writing(int *writer_id)
{
    while(1){
        think_up_data(*writer_id);
        pthread_mutex_lock(&db);
        write_data_base(*writer_id);
        pthread_mutex_unlock(&db);
    }
}

int main()
{
    int num;
    pthread_t *reader = (pthread_t*)malloc(sizeof(pthread_t)*reader_num);
    int *reader_id = (int*)malloc(sizeof(int)*reader_num);
    for(num=0;num<reader_num;num++){
        reader_id[num] = num;
        pthread_create(&reader[num],NULL,reading,(void*)(&reader_id[num]));
    }
    pthread_t *writer = (pthread_t*)malloc(sizeof(pthread_t)*writer_num);
    int *writer_id = (int*)malloc(sizeof(int)*writer_num);
    for(num=0;num<writer_num;num++){
        writer_id[num] = num;
        pthread_create(&writer[num],NULL,writing,(void*)(&writer_id[num]));
    }
    for(num=0;num<reader_num;num++){
        pthread_join(reader[num],NULL);
    }
    for(num=0;num<writer_num;num++){
        pthread_join(writer[num],NULL);
    }
    return 0;
}
