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

/*
Master Boot Record Partition Info Tables:
P01: Byte - 446, Sector - 0
P02: 462
p03: 478
p04: 494

Extended Boot Record Partition Info Tables:
P01: Byte 446, Sector - First Sector of Partition
P02: 462
P03: 478
P04: 494
*/

void print_partition_info(Partition * part, int sector) {
    printf("  \
Drive:              %i\n  \
System Type:        %x\n  \
Sector Count:       %i\n  \
Starting Sector:    %i\n  \
Ending Sector:      %i\n  \n",
    part->drive,
    part->sys_type,
    part->nr_sectors,
    part->start_sector + sector,
    part->start_sector + part->nr_sectors + sector - 1);
}

void list_partitions(int fd, int sector, char *buffer) {
    read_sector(fd, sector, buffer);
    //printf("RECURSED!!!\n");
    
    for (int i = 0; i < 4; i++){
        Partition * part = (Partition *)&buffer[0x1BE + (i*16)];
        //printf("nr_sectors: %d\n", part->nr_sectors);
        if (part->nr_sectors > 0) {
            print_partition_info(part, sector);

            if (part->sys_type == 5){
                list_partitions(fd, sector + part->start_sector, buffer);
                read_sector(fd, sector, buffer);
            }
        }
    }
}

int main(int argc, char *argv[], char *env[]) {
    int fd = open("vdisk", O_RDONLY);
    printf("File Disk: %d\n", fd);
    list_partitions(fd, 0, buffer);
}

