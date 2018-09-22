// libraries
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

// .hh
//#include "../include/Cache.hh"
//#include "../include/TraceElement.hh"

using namespace std;

int main(int argc, char** argv) {
  //size options
  int t=0;  //Cache size [KB]
  int l=0; //Line size [B]
  int a=0; // Asociativity

/**  sscanf(argv[1], "%d", &t);
  sscanf(argv[2], "%d", &l);
  sscanf(argv[3], "%d", &a);
**/

  //Architecture
  int arch=32;
  size_t byteNum = 10;

  //Trace elements - move to class object maybe?
  char instr;
  unsigned int dir;
  unsigned int c;
  int IN_objct;

  //Event counters in cache move to class object maybe? .
  long int cache_hit=0;
  long int cache_miss=0;
  int data_ok=0;

  char *IN_linea = new char;

  while((c=getline(&IN_linea, &byteNum, stdin)) != -1){
    // get instruction direction
    IN_objct = sscanf(IN_linea, "%x %s", &dir, &instr);
  }

}
