#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "ntfs.h"

#define NTFS_PARTITION "/dev/sda2"

int main(void) {
  int fd;
  VBR* boot = calloc(1, sizeof(VBR));
  char* mft = calloc(1024, sizeof(char));

  if((fd = open(NTFS_PARTITION, O_RDWR)) < 0) {
    printf("Failed to open: " NTFS_PARTITION "\n");
    return -1;
  }

  load_vbr(fd, boot);
  load_mft(fd, mft, boot);

  printf("%ld", sizeof(VBR));
  printf("%s\n", boot->oem_id);
  printf("%016lX\n", *(uint64_t*) boot->ebpb.mft_cluster_number);
  for(int i = 0; i < 1024; i += 64) {
    for(int j = 0; j < 64 && (i + j) < 1024; j++) {
      printf("%c", mft[i + j]);
    }
    printf("\n");
  }

  free(mft);
  free(boot);

  return 0;
}
