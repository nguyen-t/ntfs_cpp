#include <cstdio>
#include <cstdint>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include "vbr.hpp"
#include "mft.hpp"

MasterFileTable::MasterFileTable(VolumeBootRecord& src) {
  off_t current = src.getMFTStart();

  this->fd = dup(src.getFD());
  this->data = new MFT;

  lseek(this->fd, current, SEEK_SET);
  read(this->fd, this->data, sizeof(MFT));
  lseek(this->fd, current, SEEK_SET);
}

MasterFileTable::MasterFileTable(MasterFileTable& src) {
  off_t current = lseek(src.getFD(), 0, SEEK_CUR) + sizeof(MFT);

  this->fd = dup(src.getFD());
  this->data = new MFT;
  lseek(this->fd, current, SEEK_SET);
  read(this->fd, this->data, sizeof(MFT));
  lseek(this->fd, current, SEEK_SET);
}

MasterFileTable::~MasterFileTable() {
  close(this->fd);
  delete this->data;
}

int MasterFileTable::getFD() {
  return this->fd;
}

off_t MasterFileTable::getRecordStart() {
  return this->data->record_offset + lseek(this->fd, 0, SEEK_CUR);
}

bool MasterFileTable::valid() {
  return *reinterpret_cast<uint32_t*>(this->data->magic_number) == *reinterpret_cast<uint32_t*>(MFT_MAGIC);
}

void MasterFileTable::print() {
  printf("Master File Table\n");
  printf("Magic number:            %.4s\n", this->data->magic_number);
  printf("Update sequence offset:  0x%04x\n", this->data->update_sequence_offset);
  printf("Fixup length:            %d\n", this->data->fixup_length);
  printf("Logfile sequence number: 0x%016lx\n", this->data->logfile_sequence_number);
  printf("Sequence number:         0x%04x\n", this->data->sequence_number);
  printf("Hard link count:         %d\n", this->data->hard_link_count);
  printf("Record offset:           0x%04x\n", this->data->record_offset);
  printf("Flags:                   0x%04x\n", this->data->flags);
  printf("Real size:               0x%08x\n", this->data->real_size);
  printf("Allocated size:          0x%08x\n", this->data->allocated_size);
  printf("Base reference:          0x%016lx\n", this->data->base_reference);
  printf("Next attribute ID:       0x%04x\n", this->data->next_attribute_id);
  printf("MFT no.:                 0x%08x\n", this->data->mft_no);
  printf("\n");
}

void MasterFileTable::address() {
  printf("%p\n", static_cast<void*>(this->data));
}
