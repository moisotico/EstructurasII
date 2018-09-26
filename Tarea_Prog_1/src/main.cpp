// libraries
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <math.h>
//#include <vector>
#include<ctime>


// .hh
//#include "../include/Cache.hh"
//#include "../include/TraceElement.hh"

using namespace std;

//using namespace boost;

//Cache functions
  //print cache size
void printsize( int tag_size, int index_size, int offset_size){
  cout  <<"| Tag=" << tag_size <<" |  Index=" << index_size <<
  " | Offset=" << offset_size <<" |" <<'\n';
};
  // counts hits in cache
  bool hit_test(long int **cache,int select_set, int select_tag, int a){
    for(int i=0; i < a; i++){
      if(cache[select_set][i] == select_tag)
        return(true);
    }
    return(false);
  };

  //counters
  void counter_add(bool is_in_cache, long int hit_counter,
    long int miss_counter){
    if (is_in_cache) {
      hit_counter++;
    }
    else{
      miss_counter++;
    }
  }

  void srrip_method(){
  std::cout << "aaa" << '\n';
  };

//Control functions
  //obtain a specific word
string strWord(int index, string line) {
  int count = 0; // number of read words
  string word; // the resulting word
  for (int i = 0 ; i < line.length(); i++) { // iterate over all characters in 'line'
    if (line[i] == ' ') { // if this character is a space we might be done reading a word from 'line'
      if (line[i+1] != ' ') { // next character is not a space, so we are done reading a word
        count++; // increase number of read words
        if (count == index) { // was this the word we were looking for?
            return word; // yes it was, so return it
        }
        word =""; // nope it wasn't .. so reset word and start over with the next one in 'line'
      }
    }
    else { // not a space .. so append the character to 'word'
      word += line[i];
    }
  }
};

  //obtain set of Trace element
unsigned int get_set(unsigned int DIR, int sets_number, int offset_size){
  unsigned int data_set;
  data_set= DIR>>offset_size;
  return (data_set%sets_number);
};

  //obtain tag of Trace element
unsigned int get_tag(unsigned int DIR, int index_size, int offset_size){
  unsigned int data_tag;
  int sum = index_size + offset_size;
  data_tag=DIR>>sum;
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
  unsigned int Dir;
  //unsigned int c;
  int IN_objct;
  string abc;
  string abc2;
  //vector<string> TracElem;
  unsigned int sets_index;
  unsigned int sets_tag;
  int LS;
  bool is_in_cache;

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
  long int hit_counter=0;
  long int miss_counter=0;
  int data_ok=0;

  //Cache, asign memory
  long int **cache = new long int * [sets_number * sizeof(long int *)];
  int **cache_db = new int * [sets_number * sizeof(int *)];
  if(cache == NULL){
    std::cout << "Not enough memory for the set" << '\n';
    return EXIT_FAILURE;
  }

  // check if matrix numbers can be done
  for(int i = 0; i < sets_number; i++){
    cache[i] = new long int [a * sizeof(long int)];
    cache_db[i] = new int [a * sizeof(int)];
    if(cache[i] == NULL){
      std::cout << "Not enough memory for asociativity" << '\n';
      return EXIT_FAILURE;
    }

  }

  //intialize cache as zero
  std::cout << "sets number:" << sets_number << '\n';

  for(int i=0;i<sets_number; i++){
    for(int j=0 ; j<a ;j++){
      cache[i][j]=0;
      cache_db[i][j]=0;
    }
  }


  clock_t start = clock();
  //get and split elements
  while(getline(cin,abc2)){
    //get elements
    LS =  stoi(strWord(2, abc2)); //get 2nd word as an int
    Dir = stoul(strWord(3, abc2), nullptr, 16); //get third word as hex to int
    sets_index = get_set(Dir, sets_number, offset_size);
    sets_tag = get_tag(Dir, index_size, offset_size);
    //check cache
    //R_o_w =
    is_in_cache = hit_test( cache, sets_index, sets_tag, a);
    cout <<"boolInCache:" <<is_in_cache << '\n';
    counter_add(is_in_cache, hit_counter, miss_counter);
  }

/*
  cout << "Index"<< hex << sets_index << '\n';
  cout << "Tag: " << hex <<select_tag << '\n';
  cout << "Direction: "<< Dir << '\n';
*/
  clock_t end = clock();

  //error handling
  if (cin.bad()) {
    cout << "IO error" << '\n';
  } else if (!cin.eof()) {
    cout << "Format error" << '\n';
  }

  //Determine elapsed time
  double simTime = (end - start) * 1.0/ CLOCKS_PER_SEC;
  cerr << "Lapse time: " << simTime << " s" << endl;
}
