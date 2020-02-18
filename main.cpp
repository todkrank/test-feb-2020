#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <climits>
#include <cstring>

void generatefile(std::string, int);
void readfile(std::string);

int main(int argc, char *argv[]){
  if(argc==1){
    std::cout<<"Enter the filename(without extension) and the number of entries"<<std::endl;
    std::string filename;//or passed as argv[1]
    int N;//or passed as argv[2]
    std::cin>>filename>>N;
    generatefile(filename,N);
    readfile(filename);
    return 0;
  }
  if(!strcmp(argv[1],"generate")){
    std::string f(argv[2]);//is char length check necessary?
    int N(atoi(argv[3]));//check for proper input?
    generatefile(f,N);
  }
  if(!strcmp(argv[1],"read")){
    std::string f(argv[2]);//is char length check necessary?
    readfile(f);
  }
  std::cout<<argc<<" parameters"<<std::endl;
  for(int i(0);i<argc;++i){
    std::cout<<argv[i]<<std::endl;
  }
  /*
  if(argc!=3){
    std::cout<<"Invalid command format\n\tgenerate <filename> <number of integers>\n\tread <filename>\n";
  }
  */
  
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
  std::cout<<"Writing N entries to \'"<<filename<<"\'"<<std::endl;
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
  std::cout<<"Reading N entries from \'"<<filename<<"\'"<<std::endl;
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