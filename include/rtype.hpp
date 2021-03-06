#ifndef RTYPE_HPP
#define RTYPE_HPP

#include "record.hpp"

typedef struct __attribute__((packed)) {
  uint32_t segment_low;
  uint16_t segment_high;
  uint16_t sequence_number;
} File_Reference;

typedef struct __attribute__((packed)) {
  uint8_t reserved[0x30u];
  uint32_t owner_id;
  uint32_t security_id;
} Type_0010h;

typedef struct __attribute__((packed)) {
  File_Reference parent_dir;
  uint8_t reserved[0x38u];
  uint8_t file_name_length;
  uint8_t file_name_flags;
  uint16_t name[1];
} Type_0030h;

class Record0030h : public Record {
public:
  Record0030h(MasterFileTable&);
  Record0030h(Record&);
  void print();
};

#endif
