#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <climits>

void generatefile(std::string, int);
void readfile(std::string, int);

int main(){
  int N;
  std::string filename;
  std::cout<<"Enter the filename(without extension) and the number of entries"<<std::endl;
  std::cin>>filename>>N;
  generatefile(filename,N);
  readfile(filename,N);
  return 0;
}

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
  std::cout<<"Writing N entries from the "<<filename<<std::endl;
  for(;i<N;++i){
    randnum=distribution(generator);
    std::cout<<randnum<<std::endl;
    f.write((char*)&randnum,sizeof(int));
  }
  std::cout<<"Closing "<<filename<<" file "<<std::endl;
  f.close();
  return;
}

void readfile(std::string filename, int N){
  int i(0);
  int number;
  std::ifstream f;
  std::cout<<"Reading N entries from the "<<filename<<std::endl;
  f.open(filename,std::ios::in|std::ios::binary);
  if(!f.is_open()){
    std::cout<<"Error opening file "<<filename<<std::endl;
    return;
  }
  for(;i<N;++i){
    f.read((char*)&number,sizeof(int));
    std::cout<<number<<std::endl;
  }
  std::cout<<"Closing "<<filename<<" file "<<std::endl;
  f.close();
  return;
}