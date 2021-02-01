#ifndef MFT_HPP
#define MFT_HPP

#include <stdint.h>
#include <sys/types.h>
#include "vbr.hpp"

#define MFT_MAGIC (char*) "FILE"

typedef struct __attribute__((packed)) {
  uint8_t magic_number[4];
  uint16_t update_sequence_offset;
  uint16_t fixup_length;
  uint64_t logfile_sequence_number;
  uint16_t sequence_number;
  uint16_t hard_link_count;
  uint16_t record_offset;
  uint16_t flags;
  uint32_t real_size;
  uint32_t allocated_size;
  uint64_t base_reference;
  uint16_t next_attribute_id;
  uint16_t boundary_align;
  uint32_t mft_no;
  uint8_t body[976];
} MFT;

class MasterFileTable {
private:
  int fd;
  MFT* data;
public:
  MasterFileTable(VolumeBootRecord&);
  MasterFileTable(MasterFileTable&);
  ~MasterFileTable();
  int getFD();
  off_t getRecordStart();
  bool valid();
  void print();
  void address();
};

#endif
