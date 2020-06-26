#ifndef MFT_H
#define MFT_H

#include <stdint.h>
#include <sys/types.h>
#include "vbr.h"

#define MFT_SIZE 1024

typedef union {
  // TODO: Add MFT info
  uint8_t raw[MFT_SIZE];
} MFT;

ssize_t load_mft(int, MFT*, VBR*, off_t);
ssize_t load_mft_mirror(int, MFT*, VBR*, off_t);

#endif
