#include <OutputList.h>

OutputList::OutputList() {
  outputs = 0;
}

void OutputList::registerOutput( Output *o) {
  output[ outputs] = o;
  outputs++;
}

void OutputList::update() {
  for( int i = 0; i < outputs; i++) {
    output[ i]->update();
  }
}
