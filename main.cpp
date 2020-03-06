#include "test-feb-2020-qmake.h"

int main(int argc, char *argv[]){
  std::string command;

  for(size_t i(0);i<MAXTHREADS;++i){
    threads.push_back(std::thread(waitloop));
  }

  std::cout<<"Launched with "<<argc<<" parameters"<<std::endl;
  if(argc!=1){//console output access is asynchoronous, outputs gibberish
    std::cout<<"Launch without parameters, then enter commands.\n"<<
               "The list of available commands:\n"<<
               "generate <filename> <N entries>\n"<<
               "read <filename>\n"<<
               "exit\n";
  }
  while(true){
    std::cout<<"Command: ";
    std::cin>>command;

    if(!strcmp(command.c_str(),"generate")){
      std::cout<<"Generating..."<<std::endl;
      std::string filename;
      int N(0);
      std::cout<<"Filename: ";
      std::cin>>filename>>N;//input check! .eof() || .bad() + cin.ignore();
      std::function<void()> f_lambda=[filename,N](){generatefile(filename,N);};
      pushtask(f_lambda);
    }
    if(!strcmp(command.c_str(),"read")){
      std::cout<<"Reading..."<<std::endl;
      std::string filename;
      std::cin>>filename;//input check! .eof() || .bad() + cin.ignore();
      std::function<void()> f_lambda=[filename](){readfile(filename);};
      pushtask(f_lambda);
    }
    if(!strcmp(command.c_str(),"exit")){
      std::cout<<"Exiting..."<<std::endl;
      continuecondition=false;
      condition.notify_all();
      break;
    }
    std::cout<<"Command not recognised\n";//will trigger ALWAYS

  }
//add a check that excludes the same filenames during the same session

  for(size_t i(0);i<MAXTHREADS;++i){
    threads.at(i).join();
  }

  return 0;
}
