#include<stdlib.h>
#include<stdio.h>
#define MAX_THREADS 8
#include<pthread.h>
pthread_mutex_t lock;
class Singleton{
  Singleton(){};
  Singleton(const Singleton&)=delete;
  Singleton&operator=(const Singleton&)=delete;
  static Singleton* instance;
public:
  static Singleton*getInstance();
};
Singleton*Singleton::instance=nullptr;
Singleton*Singleton::getInstance(){
    if(instance)return instance;
    pthread_mutex_lock(&lock);
    if(instance){
      pthread_mutex_unlock(&lock);
      return instance; 
    }
#pragma optimize( "", off )//turn off memory reorder
    Singleton::instance=new Singleton();
#pragma optimize( "", on )
    pthread_mutex_unlock(&lock);
    return instance;
}
void*thrFunc(void*arg){
  Singleton*s=(Singleton*)arg;
  s=Singleton::getInstance();
  printf("address: %0x\n",s);
}
int main(){
  pthread_t threads[MAX_THREADS];
  pthread_mutex_init(&lock,NULL);
  for(int i=0;i<MAX_THREADS;++i)
    if(pthread_create(&threads[i],NULL,thrFunc,NULL)){
      fprintf(stderr,"create thread error");
      exit(1);
    }
}
