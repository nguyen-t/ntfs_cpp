#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include "mbr.hpp"
#include "vbr.hpp"

VolumeBootRecord::VolumeBootRecord(MasterBootRecord& src, int pNum) {
  off_t current = src.getPartitionStart(pNum);

  this->fd = dup(src.getFD());
  this->data = new VBR;

  lseek(this->fd, current, SEEK_SET);
  read(this->fd, this->data, sizeof(VBR));
  lseek(this->fd, current, SEEK_SET);
}

VolumeBootRecord::~VolumeBootRecord() {
  close(this->fd);
  delete this->data;
}

int VolumeBootRecord::getFD() {
  return this->fd;
}

off_t VolumeBootRecord::getMFTStart() {
  uint16_t sector_size = this->data->bpb.bytes_per_sector;
  uint8_t cluster_size = this->data->bpb.sectors_per_cluster;
  uint64_t mft_cluster = this->data->ebpb.mft_cluster_number;
  off_t offset = this->data->bpb.hidden_sectors;

  return sector_size * (cluster_size * mft_cluster + offset);
}

bool VolumeBootRecord::valid() {
  return *reinterpret_cast<uint64_t*>(this->data->oem_id) == *reinterpret_cast<uint64_t*>(VBR_MAGIC);
}

void VolumeBootRecord::print() {
  printf("Volume Boot Record\n");
  printf("OEM ID:               %.8s\n", this->data->oem_id);
  printf("End of sector marker: 0x%04x\n", this->data->end_of_sector_marker);
  printf("BIOS parameter block\n");
  printf("  Bytes per sector:          %d\n", this->data->bpb.bytes_per_sector);
  printf("  Sectors per cluster:       %d\n", this->data->bpb.sectors_per_cluster);
  printf("  Sectors per track:         %d\n", this->data->bpb.sectors_per_track);
  printf("  Number of heads:           %d\n", this->data->bpb.number_of_heads);
  printf("  Media descriptor:          0x%02x\n", this->data->bpb.media_descriptor);
  printf("  Hidden sectors:            0x%08x\n", this->data->bpb.hidden_sectors);
  printf("Extended BIOS parameter block\n");
  printf("  Total sectors:             %ld\n", this->data->ebpb.total_sectors);
  printf("  MFT cluster number:        %ld\n", this->data->ebpb.mft_cluster_number);
  printf("  Mirror MFT cluster number: %ld\n", this->data->ebpb.mft_mirror_cluster_number);
  printf("  Volume serial number       0x%16lx\n", this->data->ebpb.volume_serial_number);
  printf("  Checksum:                  0x%08x\n", this->data->ebpb.checksum);
  printf("\n");
}
