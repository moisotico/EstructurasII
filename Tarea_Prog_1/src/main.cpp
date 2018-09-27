// libraries
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <math.h>
#include<ctime>
#include <tuple>
// .hh
//#include "../include/Cache.hh"
//#include "../include/TraceElement.hh"
using namespace std;

//Cache functions
/**
* @brief Cache function to print its size, and how much bits are needed to
* represent it's size.
**/
void printsize( int tag_size, int index_size, int offset_size){
  cout  <<"| Tag=" << tag_size <<" |  Index=" << index_size <<
  " | Offset=" << offset_size <<" |" <<'\n';
};

/**
* @brief Starts the cache values as 0.
**/
void start_cache(long int **cache, int **cache_db, int a,
int sets_number){
  for(int i=0;i<sets_number; i++){
    for(int j=0 ; j<a ;j++){
      cache[i][j]=0;
      cache_db[i][j]=0;
    }
  }
};

/**
* @brief Starts the rrpv values as 2^M - 2.
**/
void start_rrpv(int **rrpv_mat, int a, int sets_number, int M){
  if (a<=2){
    M = 1;
  }
  else{
    M = 2;
  }
  //set rrpv_mat elements as 2^M - 1
  int x =round(pow(2,M)-1);
  for(int i=0;i<sets_number; i++){
    for(int j=0 ; j<a ;j++){
      rrpv_mat[i][j]=x;
    }
  }
};

/**
* @brief Determine if it's a hit or a miss and add to respective counter.
**/
tuple<bool,int> hit_test(long int **cache,int select_set, int select_tag,
int a){
  int i;
  for(i=0; i < a; i++){
    if(cache[select_set][i] == select_tag){
      return make_tuple(true, i);
    }
  }
  return make_tuple(false, i);
};

/**
* @brief Knowing if it's a hit or a miss with @param is_in_cache, it adds to
*the respective counter (hit or miss).
**/
/**
void counter_add(bool is_in_cache, long int hit_counter, long int miss_counter){
  if (is_in_cache) {
    hit_counter++;
    std::cout << " hit_counter "<<hit_counter << '\n';
  }
  else{
    miss_counter++;
    std::cout << " miss_counter "<< miss_counter << '\n';
  }
}
**/
void check_SRRIP(long int **cache, int **rrpv_mat, bool is_in_cache,
int Cache_Elem,unsigned int sets_index, unsigned int sets_tag, int rrpv_M,
int a) {
  int  DistF =round(pow(2,rrpv_M)-1) ;
  int flag = 0;
/**
  if (cache[sets_index][Cache_Elem] = 0) {
    cache[sets_index][Cache_Elem] = sets_tag;  // fill an empty space in cache
    rpv_mat[sets_index][i] = 2;
    break;
  }
**/
  if (is_in_cache) {
    rrpv_mat[sets_index][Cache_Elem] = 0; //rrpv of block to 0
  } else{
    while(flag == 0){
      for(int i=0; i < a; i++){
        if(rrpv_mat[sets_index][i] >= DistF){
          cache[sets_index][i] = sets_tag;
          rrpv_mat[sets_index][i] = 2;
          flag = 1;
          //std::cout << "/* GETOUT!!! */" << '\n';
          break;
        }
      }
      for (int i = 0; i < a; i++) {
        rrpv_mat[sets_index][i]++;
        //std::cout << "Elem:"<< rrpv_mat[sets_index][i] << '\n';
      }
    }
  }
};

//Control functions
/**
* @brief Obtains the LS (Load or store values) and direction
* (chars in hexadecimal).
**/
string strWord(int index, string line) {
  int count = 0; // words counter
  string word; // string to store the index-th word
  for (int i = 0 ; i < line.length(); i++) { // iterate over all characters in 'line'
    if (line[i] == ' ') { // if this character is a space we might be done reading a word from 'line'
      if (line[i+1] != ' ') { // next character is not a space, so we are done reading a word
        count++;
        if (count == index) { // was this the word we were looking for?
          return word;
        }
      word = ""; // nope it wasn't .. so reset word and start over with the next one in 'line
      }
    }
    else { // not a space .. so append the character to 'word'
      word += line[i];
    }
  }
  return word;
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

/**********************************************/
/**********************************************/

//main
int main(int argc, char** argv) {
  //size options
  unsigned long int t=0;  //Cache size [KB]
  unsigned int l=0; //Line size [B]
  unsigned int a=0; // Asociativity
  int sizet = 0;
  int rrpv_M=0;

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
  int arch = 32;

  //Trace elements - move to class object maybe?
  unsigned int Dir;
  string abc;
  string abc2;
  unsigned int sets_index;
  unsigned int sets_tag;
  int LS;
  int Cache_Elem;
  bool is_in_cache;
  //trace element

  //Cache elements - class methods
  int offset_size = (int)round(log2(l)); //base change
  int index_size = (int)round(log2( t/(l*a) ));
  int tag_size = arch - offset_size -index_size;
  int sets_number = t/(l*a);
  //prints directions
  printsize(tag_size,index_size,offset_size);

  //Event counters in cache move to class object maybe?
  long int hit_counter=0;
  long int miss_counter=0;
  //int data_ok=0;

  //Cache, asign memory
  long int **cache = new long int *[sets_number * sizeof(long int *)];
  int **rrpv_mat = new int *[sets_number * sizeof(int *)];
  int **cache_db = new int *[sets_number * sizeof(int *)];

  if(cache == NULL){
    cout << "Not enough memory for the set" << '\n';
    return EXIT_FAILURE;
  }

  // check if matrix numbers can be done
  for(int i = 0; i < sets_number; i++){
    cache[i] = new long int [a * sizeof(long int)];
    cache_db[i] = new int [a * sizeof(int)];
    rrpv_mat[i] = new int [a * sizeof(int)];
    if(cache[i] == NULL){
      cout << "Not enough memory for asociativity" << '\n';
      return EXIT_FAILURE;
    }
  }

 //start cache with defalut values
  start_cache(cache, cache_db, a, sets_number);
  start_rrpv(rrpv_mat, a, sets_number, rrpv_M);

  clock_t start = clock();
  //get and split elements
  while(getline(cin,abc2)){
    //get elements
    LS =  stoi(strWord(2, abc2)); //get 2nd word as an int
    Dir = stoul(strWord(3, abc2), nullptr, 16); //get third word as hex to int
    sets_index = get_set(Dir, sets_number, offset_size);
    sets_tag = get_tag(Dir, index_size, offset_size);

    //tuple for cachehit
    tie(is_in_cache, Cache_Elem) = hit_test(cache, sets_index, sets_tag, a);
  //  counter_add(is_in_cache, hit_counter, miss_counter);
    /**
    cout  << "Is it in cache: "<< is_in_cache<<'\n'
          << "Cache Element: " << Cache_Elem <<'\n'
          << "Index:"<< sets_index <<'\n'
          <<"Element: "<<cache[Cache_Elem][sets_index]<<'\n';
    **/
    if (is_in_cache) {
      hit_counter++;
      //std::cout << " hit_counter "<<hit_counter << '\n';
    }
    else{
      miss_counter++;
      //std::cout << " miss_counter "<< miss_counter << '\n';
    }

    //method SRRRIP
    check_SRRIP(cache,rrpv_mat, is_in_cache, Cache_Elem, sets_index,
    sets_tag, rrpv_M, a);
    //method memory request
  }

  clock_t end = clock();

  //error handling
  if (cin.bad()) {
    cout << "IO error" << '\n';
  } else if (!cin.eof()) {
    cout << "Format error" << '\n';
  }

  //Determine elapsed time
  double simTime = (end - start) * 1.0/ CLOCKS_PER_SEC;
  cout<< "Lapse time: " << simTime << " s" << endl;
  std::cout << "Cache entries =" <<(miss_counter + hit_counter) << '\n';

  if(miss_counter + hit_counter != 0){
    double miss_rate = 1.0 * miss_counter / (miss_counter + hit_counter);
    double hit_rate = 1.0 * hit_counter / (miss_counter + hit_counter);
    cout << "Hit rate: " << hit_rate << endl;
    cout << "Miss rate: " << miss_rate << endl;
  }

  //free dynamic memory
  for( int i = 0; i < sets_number; i++){
    delete cache[i];
    delete cache_db[i];
    delete rrpv_mat[i];
  }

  delete cache;
  delete cache_db;
  delete rrpv_mat;
}
