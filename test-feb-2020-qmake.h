#ifndef TESTFEB2020QMAKE_H
#define TESTFEB2020QMAKE_H

#include <iostream>
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

const size_t MAXTHREADS(std::thread::hardware_concurrency());
static std::vector<std::thread> threads;
static std::queue<std::function<void()>> tasks;
static std::atomic<bool> continuecondition(true);

static std::mutex tasksmutex;
static std::condition_variable condition;

void generatefile(std::string, int);
void readfile(std::string);
void waitloop();
void pushtask(std::function<void()>);

void generatefile(std::string filename, int N){
  int i(0);
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

#endif // TESTFEB2020QMAKE_H
