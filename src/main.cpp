#include <cstdio>
#include "ntfs.hpp"

int main(int argc, char** argv) {
  if(argc != 2) {
    perror("Usage ./main [DEVICE]\n");
  }

  MasterBootRecord mbr(argv[1]);
  VolumeBootRecord vbr(mbr, 2);
  MasterFileTable mft(vbr);

  while(mft.valid()) {
    mft.print();
    mft = MasterFileTable(mft);
    mft.print();
  }
  
  while(true);
  return 0;
}
