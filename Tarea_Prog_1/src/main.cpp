// libraries
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <math.h>

// boost libraries
#include <boost/algorithm/string.hpp>

// .hh
//#include "../include/Cache.hh"
#include "../include/TraceElement.hh"

using namespace std;

//Cache functions
void printsize( int tag_size, int index_size, int offset_size){
  cout  <<"| Tag=" << tag_size <<" |  Index=" << index_size <<
  " | Offset=" << offset_size <<" |" <<'\n';
}

 get_tag(unsigned int DIR, int index_Plus_offset){
  unsigned int data_tag;
  data_tag=DIR>>index_Plus_offset;
  return(data_tag);
};



//main
int main(int argc, char** argv) {
  //size options
  long int t=0;  //Cache size [KB]
  int l=0; //Line size [B]
  int a=0; // Asociativity

  int sizet = 0;
  //Parameters for CLI
  if (argc < 7){
    cerr << "Error: Argumento incompleto \n" <<
    "Modo de uso: gunzip -c mcf.trace.gz | cache -t < # > -a < # > -l < # >"
    << endl;
    return EXIT_FAILURE;
  }

  sscanf(argv[2], "%d", &sizet);
  t = (long int) (round( (pow(2.0, 10.0)) ) * sizet) ;
  sscanf(argv[4], "%d", &a);
  sscanf(argv[6], "%d", &l);
  cout  <<"cache size [KB]:" << sizet <<", block/line size [B]:" << l <<
  ", asociativity:" << a<< '\n';


  //Architecture - Detect automatically?
  int arch=32;
  size_t byteNum = 10;


  //Trace elements - move to class object maybe?
  char instr;
  unsigned int dir;
  //unsigned int c;
  int IN_objct;
  char abc[256];
  string abc2;

  //trace element
  string* IN_linea = new string;

  //Cache elements - class methods
  int offset_size = (int)round(log2(l)); //base change
  int index_size = (int)round(log2( t/(l*a) ));
  int tag_size = arch - offset_size -index_size;
  int sets_number = t/(l*a);
  //prints directions
  printsize(tag_size,index_size,offset_size);

  //Event counters in cache move to class object maybe? .
  long int cache_hit=0;
  long int cache_miss=0;
  int data_ok=0;

  //Cache, asign memory
  int **cache = new int * [sets_number * sizeof(int *)];
  if(cache == NULL){
    std::cout << "Not enough memory for the set" << '\n';
    return EXIT_FAILURE;
  }

  // check if numbers make sense
  for(int i = 0; i < sets_number; i++){
    cache[i] = new int [a * sizeof(int)];
    if(cache[i] == NULL){
      return EXIT_FAILURE;
      std::cout << "Not enough memory for asociativity" << '\n';
    }

  }

  //intialize cache as zero
  std::cout << "sets number:" << sets_number << '\n';

  for(int i=0;i<sets_number; i++){
    for(int j=0 ; j<a ;j++){
      cache[i][j]=0;
    }
  }



  while(getline(cin,abc2)){
    // get instruction direction
    cout << "linea:" << abc2 << '\n'; //print read line
    //select_set=get_set(dir, N_sets_cache, offset_cache); //Obtengo el set al que va el dato.
    //select_tag=get_tag(dir, index_Plus_offset);  //Obtengo 	el tag del dato.;

  }

  //error handling
  if (cin.bad()) {
    cout << "IO error" << '\n';
  } else if (!cin.eof()) {
    cout << "Format error" << '\n';
  }

std::string text = "Let me split this into words";
std::vector<std::string> results;
boost::split(results, text, [](char c){return c == ' ';});


}
