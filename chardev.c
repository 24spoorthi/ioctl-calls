#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<asm/uaccess.h>
#include "chardev.h"
MODULE_AUTHOR("Spoo");
MODULE_LICENSE("GPL");
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode*, struct file*);
static int device_release(struct inode*, struct file*);
static ssize_t device_read(struct file*, char*,size_t,loff_t*);
static ssize_t device_write(struct file*,const char*,size_t,loff_t*);
static long device_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param);

#define SUCCESS 0
#define BUF_LEN 80

static int Major;
static int Device_Open = 0;
static char msg[BUF_LEN];
static char *msg_Ptr;
static struct file_operations fops = {
  .read = device_read,
  .write = device_write,
  .open = device_open,
  .unlocked_ioctl = device_ioctl,
  .release = device_release
};

int init_module(void){
  Major = register_chrdev(MAJOR_NUM,DEVICE_NAME,&fops);
  if(Major < 0){
    printk(KERN_ALERT "Registerstion unsuccessful with %d",Major);
    return Major;
  }
  printk(KERN_INFO "I was assigned major number %d. To talk to\n", MAJOR_NUM);
  printk(KERN_INFO "the driver, create a dev file with\n");
  printk(KERN_INFO "'mknod /dev/%s c %d 0'\n",DEVICE_NAME, MAJOR_NUM);
  printk(KERN_INFO "Remove file when done\n");
  return SUCCESS;
}
void cleanup_module(void){
  unregister_chrdev(Major, DEVICE_NAME);
  printk(KERN_INFO "Cleanup called\n");
}
static int device_open(struct inode *inode,struct file *file){
#ifdef DEBUG
  printk(KERN_INFO "device_open(%p)\n",file);
#endif
  //  static int counter = 0;
  if (Device_Open){
    printk(KERN_INFO "Device already open: %d\n",Device_Open);
    return -EBUSY;
  }
  Device_Open++;
  //  sprintf(msg, "Hello world for the %d time\n",counter++);
  printk(KERN_INFO "opened the char dev\n");
  msg_Ptr = msg;
  try_module_get(THIS_MODULE);
  return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file){
#ifdef DEBUG
  printk(KERN_INFO "deivce_release(%p,%p)\n",inode,file);
#endif
  Device_Open--;
  printk(KERN_INFO "device release called\n");
  module_put(THIS_MODULE);
  return 0;
}
static ssize_t device_read(struct file *filp, char __user *buffer, size_t length, loff_t * offset){
#ifdef DEBUG
  printk(KERN_INFO "device_read(%p,%p,%p)\n",filp,buffer,length);
#endif
  int bytes_r = 0;
  if(*msg_Ptr == 0)
    return 0;
  while(length && *msg_Ptr){
    put_user(*(msg_Ptr++), buffer++);
    length--;
    bytes_r++;
  }
#ifdef DEBUG
  printk(KERN_INFO "Read %d bytes, %d left\n",bytes_r,length);
#endif
  return bytes_r;
}
static ssize_t device_write(struct file* filp,const char* buff,size_t len,loff_t* off){
#ifdef DEBUG
  printk(KERN_INFO "device_write(%p,%p,%p)\n",filp,buffer,len);
#endif
  int i;
  for(i = 0 ; i < len && i < BUF_LEN ; i++)
    get_user(msg[i],buff+i);
  msg_Ptr = msg;
  return i;
}

static long device_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
  int i;
  char *temp,ch;
  switch(ioctl_num)
  {
    case IOCTL_SET_MSG:
      temp = (char*) ioctl_param;
      get_user(ch,temp);
      for(i = 0 ; ch && i < BUF_LEN; i++,temp++)
	get_user(ch,temp);
      device_write(file,(char*)ioctl_param, i, 0);
      break;
    case IOCTL_GET_MSG:
      i = device_read(file,(char*)ioctl_param, 99,0);
      put_user('\0',(char*)ioctl_param+i);
      break;
    case IOCTL_GET_NTH_BYTE:
      return msg[ioctl_param];
      break;
  }
  return SUCCESS;
}
	
