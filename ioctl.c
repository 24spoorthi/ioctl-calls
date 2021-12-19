#include "chardev.h"
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
ioctl_set_msg(int fd,char* msg)
{
  int ret_val = ioctl(fd,IOCTL_SET_MSG,msg);
  if(ret_val < 0){
    printf("ioctl_set_msg failed: %d\n",ret_val);
    exit(-1);
  }
}
ioctl_get_msg(int fd)
{
  char msg[100];
  int ret_val = ioctl(fd, IOCTL_GET_MSG, msg);
  if(ret_val < 0){
    printf("ioctl_get_msg: %d\n",ret_val);
    exit(-1);
  }
  printf("get_msg message: %s\n",msg);
}
ioctl_get_nth_byte(int fd){
  char c;
  printf("get_nth_byte message:\n");
  int i = 0;
  do{
    c = ioctl(fd,IOCTL_GET_NTH_BYTE, i++);
    if(c<0){
      printf("ioctl_get_nth_byte failed at the %d'th byte:\n",i);
      exit(-1);
    }
    putchar(c);
  }while(c != 0);
  putchar('\n');
}
main()
{
  int fd,ret_val;
  char *str = "Oh Well today's headache is horrible, lol\n";
  char msg[25];
  gets(msg);
  fd = open("/dev/char_dev",0);
  if(fd < 0){
    printf("Couldn't open file: %d\n",fd);
    exit(-1);
  }
  ioctl_get_nth_byte(fd);
  ioctl_get_msg(fd);
  ioctl_set_msg(fd,msg);
  close(fd);
}
