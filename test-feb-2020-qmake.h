#ifndef TESTFEB2020QMAKE_H
#define TESTFEB2020QMAKE_H

#include <iostream>
#include <cstdio>
#include <fstream>
#include <random>
#include <string>
#include <cstring>
#include <climits>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <queue>
#include <vector>
#include <functional>
#include <mutex>

// Locks, threads, conditions - concurrency
const size_t MAXTHREADS(std::thread::hardware_concurrency());
static std::vector<std::thread> threads;
static std::queue<std::function<void()>> tasks;
static std::atomic<bool> continuecondition(true);

static std::mutex tasksmutex;
static std::mutex coutmutex;
static std::condition_variable condition;

// File operations
void generatefile(std::string, size_t);
void generatefilec(const char*, size_t);
void readfile(std::string);
void readfilec(const char *);

// Threadpool functions
void waitloop();
void pushtask(std::function<void()>);

// Statistical functions, step-by-step
double mean(const double, const double, const double);
double variance(const double, const double, const double, const double);

void generatefile(std::string filename, size_t N){
  size_t i(0);
  int randnum;
  std::ofstream f;
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(INT_MIN,INT_MAX);

  f.open(filename,std::ios::out|std::ios::binary);
  if(!f.is_open()){
    std::cout<<"Error opening file "<<filename<<std::endl;
    return;
  }
  std::cout<<"Writing "<<N<<" entries to \'"<<filename<<"\'"<<std::endl;
  for(;i<N;++i){
    randnum=distribution(generator);
    std::cout<<randnum<<std::endl;
    f.write((char*)&randnum,sizeof(int));
  }
  std::cout<<"Closing "<<filename<<" file "<<std::endl;
  f.close();
  return;
}

void readfile(std::string filename){
  int number;
  std::ifstream f;
  std::cout<<"Reading from \'"<<filename<<"\'"<<std::endl;
  f.open(filename,std::ios::in|std::ios::binary);
  if(!f.is_open()){
    std::cout<<"Error opening file "<<filename<<std::endl;
    return;
  }
  while(f.read((char*)&number,sizeof(int))){
    std::cout<<number<<std::endl;
  }
  std::cout<<"Closing "<<filename<<" file "<<std::endl;
  f.close();
  return;
}

void waitloop(){
  std::function<void()> task;
  while(continuecondition){
    {
      std::unique_lock<std::mutex> lock(tasksmutex);

      condition.wait(lock,[]{return !(tasks.empty()&&continuecondition);});
      if(!continuecondition)break;
      task=tasks.front();
      tasks.pop();
    }
    task();
  }
  return;
}

void pushtask(std::function<void()> newtask){
  {
    std::unique_lock<std::mutex> lock(tasksmutex);
    tasks.push(newtask);
  }
  condition.notify_one();
  return;
}

void generatefilec(const char* filename, size_t N){
  size_t i(0);
  int randnum;
  FILE *f;
  f=fopen(filename,"wb");
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(INT_MIN,INT_MAX);

  if(!f){//is this okay? or f==nullptr?
    std::cout<<"Error opening file "<<filename<<std::endl;
    return;
  }
  std::cout<<"Writing "<<N<<" entries to \'"<<filename<<"\'"<<std::endl;
  for(;i<N;++i){
    randnum=distribution(generator);
    std::cout<<randnum<<std::endl;
    fwrite(&randnum,sizeof(int),1,f);
  }
  std::cout<<"Closing "<<filename<<" file "<<std::endl;
  fclose(f);
  return;
}

void readfilec(const char *filename){
  int number;
  FILE* f;
  std::cout<<"Reading from \'"<<filename<<"\'"<<std::endl;
  f=fopen(filename,"rb");
  if(!f){
    std::cout<<"Error opening file "<<filename<<std::endl;
    return;
  }
  while(fread(&number,sizeof(int),1,f)){
    std::cout<<number<<std::endl;
  }
  std::cout<<"Closing "<<filename<<" file "<<std::endl;
  fclose(f);
  return;
}

double mean(double an, double n, double prevmean){
  double m;
  m=((prevmean*(n-1)+an)/n);
  return m;
}

double variance(double an, double n, double prevvariance, double prevmean){
  double v;
  v=(n-1)/n*(prevvariance+(an-prevmean)*(an-prevmean)/n);
  return v;
}

void filestats(const char* filename){
  int i(0);
  int number;
  double m(0), pm(0), v(0), pv(0);
  FILE* f;
  std::cout<<"Reading from \'"<<filename<<"\'"<<std::endl;
  f=fopen(filename,"rb");
  if(!f){
    std::cout<<"Error opening file "<<filename<<std::endl;
    return;
  }
  while(fread(&number,sizeof(int),1,f)){
    ++i;
//    std::cout<<number<<std::endl;
    m=mean(number,i,pm);
    v=variance(number,i,pv,pm);
    pm=m;
    pv=v;
    if(!(i%100))
    {
      std::unique_lock<std::mutex> lock(coutmutex);//preventing concurrent access to console output
      std::cout<<filename<<"\t"<<
                 "mean: "<<m<<
                 "\tvariance: "<<v<<
                 " ("<<i<<" numbers)"<<std::endl;
    }
  }


  {
    std::unique_lock<std::mutex> lock(coutmutex);//preventing concurrent access to console output
    std::cout<<filename<<"\t"<<
               "mean: "<<m<<
               "\tvariance: "<<v<<
               " ("<<i<<" numbers)"<<std::endl;
  }
  std::cout<<"Closing "<<filename<<" file "<<std::endl;
  fclose(f);
  return;
}

#endif // TESTFEB2020QMAKE_H
