#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "ntfs.h"

int main(int argc, char** argv) {
  int fd;
  off_t ntfs_start, mft_start, mirror_start;
  MBR* mbr    = malloc(1 * sizeof(MBR));
  VBR* vbr    = malloc(1 * sizeof(VBR));
  MFT* mft    = malloc(1 * sizeof(MFT));
  MFT* mirror = malloc(1 * sizeof(MFT));

  if(argc != 2) {
    perror("USAGE: ./main [DEVICE]\n");
    free(mbr);
    free(vbr);
    free(mft);
    free(mirror);
    return -1;
  }

  if(!(mbr && vbr && mft && mirror)) {
    perror("Failed to allocate memory\n");
    free(mbr);
    free(vbr);
    free(mft);
    free(mirror);
    return -1;
  }

  if((fd = open(argv[1], O_RDWR)) < 0) {
    perror("Failed to open device\n");
    free(mbr);
    free(vbr);
    free(mft);
    free(mirror);
    return -1;
  }

  if(mbr_read(fd, mbr) < 0) {
    perror("Failed to read MBR\n");
    free(mbr);
    free(vbr);
    free(mft);
    free(mirror);
    close(fd);
    return -1;
  }

  if(!mbr_check(mbr)) {
    perror("Failed MBR validation check\n");
    free(mbr);
    free(vbr);
    free(mft);
    free(mirror);
    close(fd);
    return -1;
  }

  if((ntfs_start = mbr_partition_offset(mbr, NTFS_PARTITION_ID)) < 0) {
    perror("Failed to find NTFS partition\n");
    free(mbr);
    free(vbr);
    free(mft);
    free(mirror);
    close(fd);
    return -1;
  }

  if(vbr_read(fd, vbr, ntfs_start) < 0) {
    perror("Failed to read VBR\n");
    free(mbr);
    free(vbr);
    free(mft);
    free(mirror);
    close(fd);
    return -1;
  }

  if((mft_start = vbr_mft_offset(vbr, ntfs_start)) < 0) {
    perror("Failed to read VBR\n");
    free(mbr);
    free(vbr);
    free(mft);
    free(mirror);
    close(fd);
    return -1;
  }

  if((mirror_start = vbr_mirror_offset(vbr, ntfs_start)) < 0) {
    perror("Failed to read VBR\n");
    free(mbr);
    free(vbr);
    free(mft);
    free(mirror);
    close(fd);
    return -1;
  }

  if(mft_read(fd, mft, mft_start) < 0) {
    perror("Failed to read MFT\n");
    free(mbr);
    free(vbr);
    free(mft);
    free(mirror);
    close(fd);
    return -1;
  }

  if(mft_mirror_read(fd, mirror, mirror_start) < 0) {
    perror("Failed to read MFT mirror\n");
    free(mbr);
    free(vbr);
    free(mft);
    free(mirror);
    close(fd);
    return -1;
  }

  free(mbr);
  free(vbr);
  free(mft);
  free(mirror);
  close(fd);

  return 0;
}
