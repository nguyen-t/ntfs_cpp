#ifndef MBR_HPP
#define MBR_HPP

#include <cstdint>
#include <sys/types.h>

#define SECTOR_SIZE       512
#define MAX_PARTITIONS    4
#define MBR_SIGNATURE     0xAA55

typedef struct __attribute__((packed)) {
  uint8_t boot_status;
  uint8_t chs_start[3];
  uint8_t type;
  uint8_t chs_end[3];
  uint32_t lba;
  uint32_t size;
} Partition_Table;

typedef struct __attribute__((packed)) {
  uint8_t bootstrap_code[446];
  Partition_Table partitions[4];
  uint16_t boot_signature;
} MBR;

class MasterBootRecord {
private:
  int fd;
  MBR* data;
public:
  MasterBootRecord(const char*);
  ~MasterBootRecord();
  int getFD();
  off_t getPartitionStart(const int);
  uint8_t getPartitionType(const int);
  uint32_t getPartitionSize(const int);
  bool valid();
  void print();
};

#endif
