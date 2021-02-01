#include <cstdio>
#include <cstdint>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include "mbr.hpp"

MasterBootRecord::MasterBootRecord(const char* device) {
  off_t current = 0;

  this->fd = open(device, O_RDWR);
  this->data = new MBR;

  lseek(this->fd, current, SEEK_SET);
  read(this->fd, this->data, sizeof(MBR));
  lseek(this->fd, current, SEEK_SET);
}

MasterBootRecord::~MasterBootRecord() {
  close(this->fd);
  delete this->data;
}

int MasterBootRecord::getFD() {
  return this->fd;
}

off_t MasterBootRecord::getPartitionStart(const int pNum) {
  return this->data->partitions[pNum - 1].lba * SECTOR_SIZE;
}

uint8_t MasterBootRecord::getPartitionType(const int pNum) {
  return this->data->partitions[pNum - 1].type;
}

uint32_t MasterBootRecord::getPartitionSize(const int pNum) {
  return this->data->partitions[pNum - 1].size;
}

bool MasterBootRecord::valid() {
  return this->data->boot_signature == MBR_SIGNATURE;
}

void MasterBootRecord::print() {
  printf("Master Boot Record\n");
  printf("Partition signature: 0x%04x\n", this->data->boot_signature);
  for(int i = 0; i < 4; i++) {
    printf("Partition %d\n", i + 1);
    printf("  Partition type ID:  0x%02x\n", this->data->partitions[i].type);
    printf("  Partition bootable: %s\n", (this->data->partitions[i].boot_status) ? "Yes" : "No");
    printf("  LBA start sector:   0x%08x\n", this->data->partitions[i].lba);
    printf("  LBA no. of sectors: 0x%08x\n", this->data->partitions[i].size);
  }
  printf("\n");
}
