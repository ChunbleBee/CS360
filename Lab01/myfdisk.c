#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

char buffer[512];

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

typedef struct partition {
    u8 drive;             /* drive number FD=0, HD=0x80, etc. */

    u8  head;             /* starting head */
    u8  sector;           /* starting sector */
    u8  cylinder;         /* starting cylinder */

    u8  sys_type;         /* partition type: NTFS, LINUX, etc. */

    u8  end_head;         /* end head */
    u8  end_sector;       /* end sector */
    u8  end_cylinder;     /* end cylinder */

    u32 start_sector;     /* starting sector counting from 0 */
    u32 nr_sectors;       /* number of of sectors in partition */
} Partition;

int read_sector(int fd, int sector, char *buf)
{
  int n;
  lseek(fd, sector*512, SEEK_SET);
  n = read(fd, buf, 512);
  if (n <= 0){
    printf("read failed\n");
    return -1;
  }
  return n;
}

int write_sector(int fd, int sector, char *buf)
{
  int n;
  lseek(fd, sector*512, SEEK_SET); // advance to sector*512 bytes
  n = write(fd, buf, 512);         // write 512 bytes from buf[] to sector
  if (n != 512){
     printf("write failed\n");
     return -1;
  }
  return n;
}

int main() {
    int fd = open("vdisk", O_RDONLY);
    printf("File Disk: %d\n", fd);

    read_sector(fd, 0, buffer);

    Partition * part = (Partition *)&buffer[494];
    /*
    P01: Byte 446
    P02: 462
    p03: 478
    p04: 494
    */

    printf(
"  Drive:              %i\n\
  System Type:        %i\n\
  Sector Count:       %i\n\
  Staring Head:       %i\n\
  Ending Head:        %i\n\
  Starting Sector:    %i\n\
  Ending Sector:      %i\n\
  Starting Cylinder:  %i\n\
  Ending Cylinder:    %i\n",
    part->head,
    part->sys_type,
    part->nr_sectors,
    part->head,
    part->end_head,
    part->sector,
    part->end_sector,
    part->cylinder,
    part->end_cylinder);
}