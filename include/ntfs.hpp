#ifndef NTFS_HPP
#define NTFS_HPP

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#else
  #error Only little endian is supported
#endif

#include "mbr.hpp"
#include "vbr.hpp"
#include "mft.hpp"
#include "record.hpp"

#define NTFS_PARTITION_ID 0x07u

#endif
