#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <unistd.h>

#define SECTOR_SIZE 512
 
extern int sky_read_mac(char *wifi_mac);
 
