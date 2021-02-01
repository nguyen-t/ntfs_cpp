#include <cstdio>
#include <cstddef>
#include <cstdint>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include "mft.hpp"
#include "rtype.hpp"
#include "record.hpp"

void print_header(Record_Generic&);
void print_resident(Record_Generic&);
void print_nonresident(Record_Generic&);
void print_0010h(Record_Generic&);
void print_0030h(Record_Generic&);

Record::Record(MasterFileTable& src) {
  size_t size = 0;
  off_t current = src.getRecordStart();

  this->fd = dup(src.getFD());

  lseek(this->fd, current, SEEK_SET);
  lseek(this->fd, offsetof(Record_Header, total_length), SEEK_CUR);
  read(this->fd, &size, sizeof(uint32_t));

  this->data = reinterpret_cast<Record_Generic*>(new uint8_t[size]);

  lseek(this->fd, current, SEEK_SET);
  read(this->fd, this->data, size);
  lseek(this->fd, current, SEEK_SET);
}

Record::Record(Record& src) {
  size_t size = 0;
  off_t current = lseek(src.fd, 0, SEEK_CUR) + src.data->header.total_length;

  this->fd = dup(src.getFD());

  lseek(this->fd, current, SEEK_SET);
  lseek(this->fd, offsetof(Record_Header, total_length), SEEK_CUR);
  read(this->fd, &size, sizeof(uint32_t));

  this->data = reinterpret_cast<Record_Generic*>(new uint8_t[size]);

  lseek(this->fd, current, SEEK_SET);
  read(this->fd, this->data, size);
  lseek(this->fd, current, SEEK_SET);
}

Record::~Record() {
  close(this->fd);
  delete[] this->data;
}

int Record::getFD() {
  return this->fd;
}

off_t Record::getBodyOffset() {
  return lseek(this->fd, 0, SEEK_CUR) + offsetof(Record_Generic, body);
}

bool Record::valid() {
  return this->data->header.total_length > 0;
}

void Record::print() {
  print_header(*this->data);

  switch(this->data->header.form_code) {
  case RESIDENT:
    print_resident(*this->data);
    break;

  case NONRESIDENT:
    print_nonresident(*this->data);
    break;
  }

  switch(this->data->header.type) {
    case STANDARD_INFORMATION:
      print_0010h(*this->data);
      break;

    case ATTRIBUTE_LIST:
      break;

    case FILE_NAME:
      print_0030h(*this->data);
      break;

    case OBJECT_ID:
      break;

    case VOLUME_NAME:
      break;

    case VOLUME_INFORMATION:
      break;

    case DATA:
      break;

    case INDEX_ROOT:
      break;

    case INDEX_ALLOCATION:
      break;

    case BITMAP:
      break;

    case REPARSE_POINT:
      break;
  }
  printf("\n");
}

void print_header(Record_Generic& rec) {
  printf("Record Header\n");
  printf("Type:         0x%04x\n", rec.header.type);
  printf("Total length: %d\n", rec.header.total_length);
  printf("Form code:    %s\n", (rec.header.form_code) ? "Nonresident" : "resident");
  printf("Name length:  %d\n", rec.header.name_length);
  printf("Flags:        0x%04x\n", rec.header.flags);
  printf("Instance no.: 0x%04x\n", rec.header.instance);
}

void print_resident(Record_Generic& rec) {
  Record_Resident& res = reinterpret_cast<Record_Resident&>(rec);
  printf("Resident Header\n");
  printf("    Value length: %d\n", res.value_length);
  printf("    Value offset: 0x%08x\n", res.value_offset);
}

void print_nonresident(Record_Generic& rec) {
  Record_Nonresident& nres = reinterpret_cast<Record_Nonresident&>(rec);
  printf("Nonresident Header\n");
  printf("    VCN low:          0x%016lx\n", nres.vcn_low);
  printf("    VCN high:         0x%016lx\n", nres.vcn_high);
  printf("    Data offset:      0x%04x\n", nres.data_offset);
  printf("    Allocated length: %ld\n", nres.allocated_length);
  printf("    Real length:      %ld\n", nres.real_length);
}

void print_0010h(Record_Generic& rec) {
  Record_Resident& res = reinterpret_cast<Record_Resident&>(rec);
  Type_0010h& ref = reinterpret_cast<Type_0010h&>(res.reserved);
  printf("STANDARD_INFORMATION Type\n");
  printf("    Owner ID:    0x%08x\n", ref.owner_id);
  printf("    Security ID: 0x%08x\n", ref.security_id);
}

void print_0030h(Record_Generic& rec) {
  Record_Resident& res = reinterpret_cast<Record_Resident&>(rec);
  Type_0030h& ref = reinterpret_cast<Type_0030h&>(res.reserved);
  printf("FILE_NAME Type\n");
  printf("    Parent directory\n");
  printf("        Segment low:     0x%08x\n", ref.parent_dir.segment_low);
  printf("        Segment high:    0x%08x\n", ref.parent_dir.segment_high);
  printf("        Sequence number: 0x%04x\n", ref.parent_dir.segment_low);
  printf("    Flags:       %02x\n", ref.file_name_flags);
  printf("    Name length: %d\n", ref.file_name_length);
  printf("    Name:        ");
  for(int i = 0; i < ref.file_name_length; i++) {
    printf("%c", ref.name[i] & 0xFFu);
  }
  printf("\n");
}
