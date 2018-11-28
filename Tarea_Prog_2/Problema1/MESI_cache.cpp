/**
To consider:
- If the access No. mod 4 == 0 ---> a request to memory of CPU1
- Else                         ---> an access of CPU2
**/

#include<iostream>
#include<cmath>
#include<ctime>
#include<string>
#include<tuple>
using namespace std;

#define ARCH 32
#define CACHE_SZ_KB 16 //in KBytes
#define L1_ASOC 2
#define BLOCK 32    //
#define OFFSET1 13
#define OFFSET2 16

/**
** @brief FSM transitions for MESI handling, using one memory space at a time
**/

void MESI_Protocol( int RW1, int RW2, int mesi_st_cpu1, int mesi_st_cpu2,
                    int coherence_Inv1, int coherence_Inv2)
{
  switch(mesi_st_cpu1){
    case 0:
    if (RW2 == 1) {
      mesi_st_cpu1 = 3;
      mesi_st_cpu2 = 0;
      coherence_Inv1++;
    } else if (RW1 == 0) {
      mesi_st_cpu1 = 1;
    }
    break;
    case 1:
      if (RW2 == 0 && mesi_st_cpu2 == 3) {
        mesi_st_cpu1 = 2;
        mesi_st_cpu2 = 2;
      } else if (RW2 == 1) {
        mesi_st_cpu1 = 0;
        mesi_st_cpu2 = 3;
        coherence_Inv2++;
      } else if (RW1 == 1) {
        mesi_st_cpu1 = 3;
        mesi_st_cpu2 = 0;
        coherence_Inv1++;
      }
      break;

    case 2:
      if (RW1 == 1) {
        mesi_st_cpu1 = 0;
        mesi_st_cpu2 = 3;
        coherence_Inv2++;
      } else if (RW2 == 1) {
        mesi_st_cpu1 = 3;
        mesi_st_cpu2 = 0;
        coherence_Inv1++;
      }
      break;
    case 3:
      if (mesi_st_cpu2== 3){
        if (RW1 == 1) {
            mesi_st_cpu1 = 0;
        } else if (RW1 == 0){
            mesi_st_cpu1 = 1;
        } else if (RW2 == 1) {
            mesi_st_cpu2 = 0;
        } else {
            mesi_st_cpu2 = 1;
        }
      } else{
        switch (mesi_st_cpu2) {
          case 0:
            if (RW1 == 1) {
                mesi_st_cpu2 = 3;
                coherence_Inv2++;
                mesi_st_cpu1 = 0;
            } else if (RW2 == 0) {
                mesi_st_cpu2 = 1;
            }
            break;

          case 1:
            if (RW1 == 0 && mesi_st_cpu1 == 3) {
                mesi_st_cpu2 = 2;
                mesi_st_cpu1 = 2;
            } else if (RW1 == 1) {
                mesi_st_cpu2 = 0;
                mesi_st_cpu1 = 3;
                coherence_Inv1++;
            } else if (RW2 == 1) {
                mesi_st_cpu2 = 3;
                coherence_Inv2++;
                mesi_st_cpu1 = 0;
            }
            break;

          case 2:
            if (RW2 == 1) {
              mesi_st_cpu2 = 0;
              mesi_st_cpu1 = 3;
              coherence_Inv1++;
            } else if (RW1 == 1) {
              mesi_st_cpu2 = 3;
              mesi_st_cpu1 = 0;
              coherence_Inv2++;
            }
            break;

          case 3:
              break;
        }
      }
      break;
  }
};


/**
@brief Prints the cache sized
**/
void printsize( int tag_size, int index_size, int offset_size){
  cout  <<"| Tag=" << tag_size <<" |  Index=" << index_size <<
  " | Offset=" << offset_size <<" |" <<'\n';
};


string strWord(int index, string line){
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

void start_cache(long int **cache, int **cache_db, int a,
int sets_number){
  for(int i=0;i<sets_number; i++){
    for(int j=0 ; j<a ;j++){
      cache[i][j]=0;
      cache_db[i][j]=0;
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



int main(int argc, char const *argv[]){
  clock_t begin = clock();
  // The MESI Protocol states are defined as M=0, E=1, S=2, I=3.
  int RW1 = 0;
  int RW2 = 0;
  int mesi_st_cpu1 = 3;
  int mesi_st_cpu2 = 3;
  int coherence_Inv1 = 0;
  int coherence_Inv2 = 0;
  int miss_L1 = 0;
  int miss_L2 = 0;
  string abc;
  string abc2;
  int index_1 = (int)round(log2(BLOCK/(L1_ASOC*OFFSET1) ));
  int index_2 =
  int tag_size_1 = arch - OFFSET1 -index_size;
  int sets_1 = BLOCK/(BLOCK*L1_ASOC);

  //start cache with defalut values
//   start_cache(cache, cache_db, a, sets_number);
  while(getline(cin,abc2)){
    LS =  stoi(strWord(2, abc2)); //get 2nd word as an int
    Dir = stoul(strWord(3, abc2), nullptr, 16); //get third word as hex to
    sets_index = get_set(Dir, sets_number, offset_size);
    sets_tag = get_tag(Dir, index_size, offset_size);

  }



  clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    cout << "Execution time: "<< time_spent << '\n';
  return 0;
};
