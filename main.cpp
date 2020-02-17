#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <climits>

void generate(std::string, int);

int main(){
  int N;
  std::string filename;
  std::cout<<"Enter the filename(without extension) and the number of entries"<<std::endl;
  std::cin>>filename>>N;
  generate(filename, N);
  return 0;
}

void generate(std::string filename, int N){
  int i=0;
  int randnum;
  std::ofstream f;
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0,INT_MAX);

  f.open(filename,std::ios::out|std::ios::binary);
  if(!f.is_open()){
    std::cout<<"Error opening file "<<filename<<std::endl;
    return;
  }
  for(;i<N;++i){
    randnum=distribution(generator);
    std::cout<<"Writing "<<randnum<<" to "<<filename<<std::endl;
    f<<randnum;
  }
  std::cout<<"Closing "<<filename<<" file "<<std::endl;
  f.close();
  return;
}