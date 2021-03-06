#ifndef Record_HPP
#define Record_HPP

#include <sys/types.h>
#include "mft.hpp"

typedef enum {
  STANDARD_INFORMATION  = 0x0010u,
  ATTRIBUTE_LIST        = 0x0020u,
  FILE_NAME             = 0x0030u,
  VOLUME_VERSION        = 0x0040u,
  OBJECT_ID             = 0x0040u,
  SECURITY_DESCRIPTOR   = 0x0050u,
  VOLUME_NAME           = 0x0060u,
  VOLUME_INFORMATION    = 0x0070u,
  DATA                  = 0x0080u,
  INDEX_ROOT            = 0x0090u,
  INDEX_ALLOCATION      = 0x00A0u,
  BITMAP                = 0x00B0u,
  SYMBOLIC_LINK         = 0x00C0u,
  REPARSE_POINT         = 0x00C0u,
  EA_INFORMATION        = 0x00D0u,
  EA                    = 0x00E0u,
  PROPERTY_SET          = 0x00F0u,
  LOGGED_UTILITY_STREAM = 0x0100u
} Record_Def;

typedef enum {
  RESIDENT    = 0x00u,
  NONRESIDENT = 0x01u,
} Attribute_Residency;

// Generic record
typedef struct __attribute__((packed)) {
  uint32_t type;
  uint32_t total_length;
  uint8_t form_code;
  uint8_t name_length;
  uint16_t name_offset;
  uint16_t flags;
  uint16_t instance;
} Record_Header;

typedef struct __attribute__((packed)) {
  Record_Header header;
  uint8_t body[1];
} Record_Generic;

typedef struct __attribute__((packed)) {
  Record_Header header;
  uint32_t value_length;
  uint32_t value_offset;
  uint8_t reserved[2];
  uint8_t body[1];
} Record_Resident;

typedef struct __attribute__((packed)) {
  Record_Header header;
  uint64_t vcn_low;
  uint64_t vcn_high;
  uint16_t data_offset;
  uint8_t reserved[6];
  uint64_t allocated_length;
  uint64_t real_length;
  uint64_t initial_length;
  uint8_t body[1];
} Record_Nonresident;

class Record {
protected:
  int fd;
  Record_Generic* data;
public:
  Record(MasterFileTable&);
  Record(Record&);
  ~Record();
  int getFD();
  off_t getBodyOffset();
  bool valid();
  void print();
};

#endif
