#define MATRIX_ADDR_TESTING

#include "../../src/kaleidoscope/MatrixAddr.h"

#include <iostream>
#include <cassert>
#include <string>
#include <cxxabi.h>

template<typename MA__>
void logType() {
  std::cout << "   rows:" << (int)MA__::rows << std::endl;
  std::cout << "   cols:" << (int)MA__::cols << std::endl;
  std::cout << "   rowSize:" << (int)MA__::rowSize << std::endl;
}

template<typename MA__>
void testIndexedAccess() {
  MA__ ma2;

  ma2.setRow(1);
  ma2.setCol(3);
  assert(ma2 == MA__(1, 3));

  ma2.setRow(7);
  assert(ma2 == MA__(7, 3));

  ma2.setCol(5);
  assert(ma2 == MA__(7, 5));

  MA__ ma3(3, 5);

  assert(ma3.row() == 3);
  assert(ma3.col() == 5);
}

template<typename MA__>
std::string typenameString() {
  std::string nameString;
  char *name = 0;
  int status;
  name = abi::__cxa_demangle(typeid(MA__).name(), 0, 0, &status);
  if (name != 0) {
    nameString = name;
  }
  free(name);
  return nameString;
}

template<typename MA1__, typename MA2__>
void testRelation() {

  std::cout << "***************************************" << std::endl;
  std::cout << "MA1:" << typenameString<MA1__>() << std::endl;
  logType<MA1__>();

  std::cout << "MA2:" << typenameString<MA2__>() << std::endl;
  logType<MA2__>();

  MA1__ mAddr1(3, 5);
  MA2__ mAddr2(2, 5);
  MA1__ mAddr3;

  assert(mAddr1.isValid());
  assert(!mAddr3.isValid());

  assert(mAddr1 != mAddr2);

  MA2__ mAddr1Copy = mAddr1;
  assert(mAddr1Copy == mAddr1);

  MA2__ mAddr1Assigned;
  mAddr1Assigned = mAddr1;
  assert(mAddr1Assigned == mAddr1);

  assert(mAddr2 < mAddr1);
  assert(mAddr1 > mAddr2);

  assert(mAddr2 <= mAddr1);
  assert(mAddr1 >= mAddr2);

  assert(mAddr1 <= mAddr1);
  assert(mAddr2 >= mAddr2);
}

int main(int argc, char **argv) {
  typedef kaleidoscope::MatrixAddr<7, 14> MA1;
  typedef kaleidoscope::MatrixAddrCompressed<7, 14> MA2;
  typedef kaleidoscope::MatrixAddrCompressed<7, 8> MA3;

  testIndexedAccess<MA1>();
  testIndexedAccess<MA2>();

  testRelation<MA1, MA1>();
  testRelation<MA2, MA2>();
  testRelation<MA1, MA2>();
  testRelation<MA2, MA1>();

  testRelation<MA3, MA3>();
  testRelation<MA3, MA1>();
  testRelation<MA1, MA3>();
}
