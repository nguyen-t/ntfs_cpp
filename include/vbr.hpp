#ifndef VBR_HPP
#define VBR_HPP

#include <cstdint>
#include <sys/types.h>
#include "mbr.hpp"

#define VBR_MAGIC     (char*) "NTFS    "
#define VBR_SIGNATURE 0xAA55

// NTFS Boot Sector structure
typedef struct __attribute__((packed)) {
  uint8_t jump_instruction[3];
  uint8_t oem_id[8];
  struct __attribute__((packed)) bpb {
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t unused0[3];
    uint8_t unused1[2];
    uint8_t media_descriptor;
    uint8_t unused2[2];
    uint16_t sectors_per_track;
    uint16_t number_of_heads;
    uint32_t hidden_sectors;
    uint8_t unused3[4];
  } bpb;
  struct __attribute__((packed)) ebpb {
    uint8_t unused4[4];
    uint64_t total_sectors;
    uint64_t mft_cluster_number;
    uint64_t mft_mirror_cluster_number;
    union {
      uint8_t bytes_per_file_record_segment;
      uint8_t clusters_per_file_record_segment;
    };
    uint8_t unused5[3];
    union {
      uint8_t bytes_per_index_buffer;
      uint8_t clusters_per_index_buffer;
    };
    uint8_t unused6[3];
    uint64_t volume_serial_number;
    uint32_t checksum;
  } ebpb;
  uint8_t bootstrap_code[426];
  uint16_t end_of_sector_marker;
} VBR;

class VolumeBootRecord {
private:
  int fd;
  VBR* data;
public:
  VolumeBootRecord(MasterBootRecord&, int);
  ~VolumeBootRecord();
  int getFD();
  off_t getMFTStart();
  bool valid();
  void print();
};

#endif
