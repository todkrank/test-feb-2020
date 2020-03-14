#include "test-feb-2020-qmake.h"
//1.put filestats .h in order
//2.generate - random/hand input; bin/text/(toml?), add one more parameter - flags?
//3.create hand input txt file with 2 4 4 4 5 5 7 9 numbers (mean 5, var 4, stdev 2)
//4.add delays into filestats to make testing easier
int main(void){
  std::string command;
  std::string filename;
  const size_t MAX_FILENAME_LENGTH(255);
  size_t N(0);
  for(size_t i(0);i<MAXTHREADS;++i){
    threads.push_back(std::thread(waitloop));
  }

  //console output access is asynchoronous, outputs gibberish without mutex
  std::cout<<"The list of available commands:\n"<<
             "generate <filename> <N entries>\n"<<
             "read <filename>\n"<<
             "filestats <filename>\n"<<
             "exit\n";

  while(true){
    //1.replace generatefile with c-version
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
      std::function<void()> f_lambda=[filename](){readfilec(filename.c_str());};
      pushtask(f_lambda);
      continue;
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
      if(!(std::cin>>N)){
        std::cout<<"Invalid number. Repeat command:"<<std::endl;
        continue;
      }

      std::function<void()> f_lambda=[filename,N](){generatefilec(filename.c_str(),N);};
      pushtask(f_lambda);
      continue;
    }
    if(!strcmp(command.c_str(),"filestats")){
      if(!(std::cin>>filename)){//if the input is invalid (cin returns bad() or eof()
        std::cout<<"Invalid filename. Repeat command:"<<std::endl;
        continue;
      }
      if(filename.length()>MAX_FILENAME_LENGTH){
        std::cout<<"Filename too long. Repeat command:"<<std::endl;
        continue;
      }
      std::cout<<"Reading "<<filename<<std::endl;
      std::function<void()> f_lambda=[filename](){filestats(filename.c_str());};
      pushtask(f_lambda);
      continue;
    }
    std::cout<<"Command not recognised\n";
  }
//add a check that excludes the same filenames during the same session
//incapsulate code in if's? is it possible

  for(size_t i(0);i<MAXTHREADS;++i){
    threads.at(i).join();
  }

  return 0;
}
