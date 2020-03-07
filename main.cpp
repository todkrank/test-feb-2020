#include "test-feb-2020-qmake.h"

int main(void){
  std::string command;
  std::string filename;
  const size_t MAX_FILENAME_LENGTH(255);
  int N(0); //make it size_t and change function declr/definitions in the header corresp.
  for(size_t i(0);i<MAXTHREADS;++i){
    threads.push_back(std::thread(waitloop));
  }

  //console output access is asynchoronous, outputs gibberish
  std::cout<<"The list of available commands:\n"<<
             "generate <filename> <N entries>\n"<<
             "read <filename>\n"<<
             "exit\n";

  while(true){
    std::cin>>command;
    if(!strcmp(command.c_str(),"exit")){
      std::cout<<"Exiting..."<<std::endl;
      continuecondition=false;
      condition.notify_all();
      break;
    }
    if(!strcmp(command.c_str(),"read")){
      if(!(std::cin>>filename)){//if the input is invalid (cin returns bad() or eof()
        std::cout<<"Invalid filename. Repeat command:"<<std::endl;
        continue;
      }
      if(filename.length()>MAX_FILENAME_LENGTH){
        std::cout<<"Filename too long. Repeat command:"<<std::endl;
        continue;
      }
      std::cout<<"Reading "<<filename<<std::endl;
      std::function<void()> f_lambda=[filename](){readfile(filename);};
      pushtask(f_lambda);
    }
    if(!strcmp(command.c_str(),"generate")){
      if(!(std::cin>>filename)){//if the input is invalid (cin returns bad() or eof()
        std::cout<<"Invalid filename. Repeat command:"<<std::endl;
        continue;
      }
      if(filename.length()>MAX_FILENAME_LENGTH){
        std::cout<<"Filename too long. Repeat command:"<<std::endl;
        continue;
      }


      if(!std::cin>>N){//is N unsigned already?
        std::cout<<"Invalid number. Repeat command:"<<std::endl;
        continue;
      }
      std::cout<<"Generating "<<filename<<" with "<<N<<" numbers"<<std::endl;
      std::function<void()> f_lambda=[filename,N](){generatefile(filename,N);};
      pushtask(f_lambda);
    }
    else {
      std::cout<<"Command not recognised\n";
    }
  }
//add a check that excludes the same filenames during the same session

  for(size_t i(0);i<MAXTHREADS;++i){
    threads.at(i).join();
  }

  return 0;
}
