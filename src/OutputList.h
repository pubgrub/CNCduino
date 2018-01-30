#ifndef OUTPUTLIST_H
#define OUTPUTLIST_H

class Output;

class OutputList {
  int outputs;
  Output* output[20];

public:
  OutputList();
  void registerOutput( Output* output);
  void update();
};

#include <Output.h>


#endif
