/*
 * CS 5348.001 - PROJECT.2 
 * TEAM - 17
 * SAI MALLIKARJUNA RAO, BOLLAM - sxb200136
 * SAHITH, ANUSURI - sxa200163
 * MINH, TANG - mgt180001
 *
 *------------------------------------------------------------------
 *
 *      ******  PART - 1  ******
 *
 *------------------------------------------------------------------
 * 
 *    **** CONTRIBUTIONS ****
 *
 *------------------------------------------------------------------
 *
 * All the team members worked collectively to implement each command.
 * Each one of us got familiar with the read, write, open, lseek system calls by implementing small programs on our own and
 * used that knowledge to understand the working of the three commands to be implemented.
 * For implementing the initfs command we went through the class notes and the slides, understood the multiple steps involved in the 
   file system initialization process such as initializing the data blocks, inodes, adding and getting the free data blocks and verifing the 
   file system initialization without the ninode value.
 *
 *-------------------------------------------------------------------
 *
 * In the main function, the filename has been hard coded to "sample3.txt". 
 * When the program is compiled and executed, it creates a file in the directory where the mod-v6.c file exists(present working directory) 
   if the file does not exists and initializes the v6 file system in that location with that file name.
 * If given a pathname, it creates or reads the file from that location.
 * If the file exists, it reads the FSIZE and ISIZE values from the super block.
 * To test the program the hard coded input in the main function can be changed to any filename or pathname of the user choice.
 * To execute the mod-v6.c file, follow the below mentioned procedure:
 * 1.cc mod-v6.c
 * 2../a.out
 *
 * ------------------------------------------------------------------
 *
 * A normal text file of the sort "sample.txt" has been formatted to function as a V6 file system by writing and reading the information
   from that file block by block where the block size is assumed to be of 1024 bytes.
 * Below is a C program which implements the following three commands in the above V6 file system.
   1. Openfs
   2. Initfs
   3. q

   OPENFS
   ------
 * The pathname/filename is taken as the input for implementing the openfs command.
 * We call the open system call with the provided pathname and the type of access as parameters
 * The open call returns an integer value which is the file descriptor which points to the file table
   entry which in-turn points to inode table entry.
 * As we know the values for fd=0,1,2 are reserved and -1 is returned if the file does not exist.
 * So we check the return value of the open system call to check whether the file exists or not.
 * If the file does not exist in the given pathname, we then create a new file in the given pathname location and implement a v6 file system 
   in the file by calling the initfs command for initializing the file system in the given pathname location.
 * If the file already exists in the given location we then read the super block and print the FSIZE and ISIZE values. It also reads
 * the root inode and prints the first two entries present in the block number of addr[0] of the root inode
 * If the file exists but has not been initialized we ask for the FSIZE and ISIZE values to initiaize the file system.
 
   INITFS
   ------
 * The initfs command takes the v6 file system size and the number of blocks devoted to inodes as the parameters.
 * We write the values of fsize and isize into the super block and copy the super block into the file system.
 * We add all the free blocks to the free block array using addFreeBlock function.
 * Following the above steps, the inodes are initialized with MSB of flags field as '0' which indicates that the inodes are not allocated.
 * We then get a free data block using getFreeBlock function to store the first two entries of the root directory.
 * We initialize the root inode with MSB in the flags field as '1' indicating that it has been allocated and also the 14th and 13th bit to "10"
   indicating that the file type is a directory.
 * In the free block derived using getFreeBlock function, we store the two entries of root and its parent which is the root itself.
 * The block number is then stored in the addr[0] of the root inode.
 * The root inode is then copied into the first 64 bytes of the block number 2 after skipping the bootloader and super block.
 * We then call the verify function with the file descriptor as the parameter to check if the file system initialization is successful or not.
 * In verify function, we check the MSB of flags field for each inode starting from the root inode to know whether it's allocated or not.

   Q
   ----
 * We use the close system call which takes the file descriptor as the parameter.
 * It closes the file which the file descriptor is pointing to.
 * Then we quit the program using the exit system call.
 * --------------------------------------------------------------------------------------------------------------
 *
 *       ******  PART - 2  ******
 *
 *---------------------------------------------------------------------------------------------------------------
 * 
 *    **** CONTRIBUTIONS ****
 *
 *---------------------------------------------------------------------------------------------------------------
 *
 * Sai Mallikarjuna Rao Bollam : cpin, cpout functions
 *
 * Sahith Anusuri : rm, mkdir functions
 *
 * Minh Tang : cd, q, main functions
 *
 *----------------------------------------------------------------------------------------------------------------
 *
 * As part of the part-2 of the project-2, 6 commands have been implemented built upon the work done in part 1 of the project 2.
 * 
 * 1. CPIN
 * --------
 * The contents present in the external file are read in chunks of size 1024 bytes each and copied into the V6 file block by block.
 * The block numbers in which the contents are placed are stored in the addr array of an unallocated inode.
 * The size field of the inode is incremented everytime a block of data is copied from the external file.
 * The most significant bit is set to 1 indicating that the inode is allocated.
 * The 14th and 13th bit are set to 0 indicating that it is a file and 12th, 11th bit are set to 0 indicating that it's a small file.
 * The inode number and file name are stored in the first data block of the root directory.
 * The inode contents are written in it's respective location in the file system.
 *-------------------------------------------------------------------------------------------------------------
 * 
 * 2. CPOUT
 * ---------
 * The contents present in the v6 file are read block by block and copied into the external file.
 * The v6 file is fetched by comparing the entries in the first data block of the present working directory and the corresponding 
 * inode number is obtained whenever there is a match with the given v6 file name.
 * The data block numbers of that file are obtained by accessing the addr array of that inode.
 * -------------------------------------------------------------------------------------------------------------
 * 
 * 3. RM
 * ------
 * The inode number of the file is fetched by comparing the entries in the present working directory with the given file name.
 * Whenever there is a match we access the data blocks using the addr array and add those blocks to the free array.
 * We unallocate the inode by setting it's most significant bit to 0 and write it back into it's location in the file system.
 * The inode of that particular file is set to 0 and that entry is replaced with the last directory/file entry of the first block of the 
 * present working directory and the size of the present working directory is decremented by 32 bytes(size of the directory or file entry).
 * -----------------------------------------------------------------------------------------------------------------------------------------
 *
 * 4. MKDIR
 *  --------
 * An unallocated inode is fetched by checking the flags field and a free block is fetched to store the two entries of any directory which
 * point to the directory itslef and its parent.
 * The first entry will be that of the new directory and the second entry will be the present working directory in which the directory
 * is being created.
 * These two entries are written in the free block fetched and the block number is stored in the addr array of the unallocated inode fetched.
 * The size field of the fetched inode is increased to 64 bytes(2 entries). 
 * The MSB of the flags field of the inode will be set to 1 and the 14,13th bit to 10 indicating that it is a directory, 12th & 11th bit to
 * 00 indicating that it's a small file.
 * The inode is then written back into it's respective location in the file system.
 * In the present working directory a new directory entry is added with the inode as the unallocated inode fetched and the corresponding file name
 * as the name of the new directory being created.
 * The size of the present working directory is increased by 32 bytes as a new entry is added to it.
 *------------------------------------------------------------------------------------------------------------------------------
 *
 * 5. CD
 *   -----
 * The input directory is searched among the entries stored in the first data block of the present working directory. 
 * When there is a match the inode number of that particular entry is set as the current inode number and the directory name is set
 * as the present working directory.
 *--------------------------------------------------------------------------------------------------------------------------
 *
 * 6. Q
 *   ---
 * We use the close system call which takes the file descriptor as the parameter.
 * It closes the file which the file descriptor is pointing to.
 * Then we quit the program using the exit system call.


*/
#include<fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>
#include<errno.h>
#include <unistd.h>
#include <sys/types.h>
extern int errno;

//super_block struct

typedef struct {
int isize;
int fsize;
int nfree;
unsigned int free[251];
char flock;
char ilock;
char fmod;
unsigned int time;
} superblock_type; // Block size is 1024 bytes , used are 1023 bytes

//i-node structure

typedef struct {
unsigned short flags;
unsigned short nlinks;
unsigned int uid;
unsigned int gid;
unsigned int size0;
unsigned int size1;
unsigned int addr[9];
unsigned int actime;
unsigned int modtime;
} inode_type; // 64 bytes in size

//dir_type struct

typedef struct {
unsigned int inode;
char filename[28];
} dir_type; // 32 bytes long

//global variables
int block_size=1024;//size of the block is 1024 bytes
int inode_size=64;//size of the inode is 64 bytes
int fd;//file descriptor
superblock_type sb;//super block instance
inode_type root_inode;//root_inode
int current_inumber;//stores the inode number of the present working directory
int n1;//size of the file system
int n2;//number of blocks for inodes
int inode_count;//no:of inodes
char pwd[100];//present working directory...for the most part it is the root unless the user gives the "cd dir" command.

// implementing the 'q' command
void q(int fd){
  close(fd);//closing the file 
   printf("file has been closed successfully\n");
   exit(0);
}

//implementing the 'openfs' command
int  openfs(char *pathname){
   fd=open(pathname,O_RDWR);//opening the file
    //if the file does not exist
    if(fd==-1){
      printf("The file does not exists so created a new file in the given pathname\n");
    	fd=open(pathname, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR);//creating the file
    	//asking the user the values of n1 & n2
    	printf("Enter the size of the file system size in number of blocks :\n");
    	scanf("%d", &n1);
    	printf("Enter the number of blocks devoted for inodes :\n");
    	scanf("%d", &n2);
    	if(n1-n2-2<1){//checking if the values of n1 and n2 are valid
    	  printf("Invalid values of n1 and n2\n");
    	  return -1;
    	}
	return 0;
  }
	//if the file exists
  	else{
  	printf("File has been opened successfully\n");
	lseek(fd, block_size, SEEK_SET);
	read(fd, &sb, sizeof(sb));//read the super block and print fsize and isize values
	 if(sb.fsize==0 || sb.isize==0){//if the file exists but the file system has not been initialised
	   printf("The file exists but has not been initialised\n");
	   printf("Enter the size of the file system size in number of blocks :\n");
	   scanf("%d", &n1);
	   printf("Enter the number of blocks devoted to inodes :\n");
	   scanf("%d", &n2);   
	   return 0;
	 }

	//reading the fsize, isize and the root directory entries
	inode_type temp2;
	dir_type first;
	dir_type second;
	lseek(fd, 2*block_size, SEEK_SET);//skip to the block where the root inode is present
	read(fd, &temp2, 64);
	lseek(fd, (temp2.addr[0])*block_size, SEEK_SET);//skip to the block where the root directory entries are present
	read(fd, &first, 32);
	read(fd, &second, 32);
	printf("The value of fsize is %d\n", sb.fsize);
	printf("The value of isize is %d\n",sb.isize);
	inode_count=sb.isize*16;
	current_inumber=1;
	strcpy(pwd,"/");
	printf("Inode of the first entry for the root directory is  %d\n", first.inode);
	printf("Filename of the first entry for the root directory is  %s\n", first.filename);
	printf("Inode of the second entry for the root directory is  %d\n", second.inode);
	printf("Filename of the second entry for the root directory is  %s\n", second.filename);
	return 1;
  }
}

//initializing the inodes
void initialize_inode(int k){
  inode_type inode;
  inode.flags=0;//flags field as zero as they are not allocated
  int inode_bn=2+(k*inode_size/block_size);//calculating the block number of the inode
  int offset=((inode_size*(k-1))%block_size);//calculating the position with respect to the first inode in that block
  lseek(fd, inode_bn*block_size+offset, SEEK_SET);
  write(fd, &inode, sizeof(inode));//writing the inode into it's respective block
}

//adding free blocks to the free array
void addFreeBlock(int free_block){
  if(sb.nfree==251){//if the free array is full
    lseek(fd, free_block*block_size, SEEK_SET);
    write(fd, &sb.nfree, sizeof(sb.nfree));// copy the nfree value into the free block
    lseek(fd, free_block*block_size+4, SEEK_SET);
    write(fd, &sb.free, sizeof(sb.free));//copy the free array into the free block
    sb.nfree=0;
  }
  sb.free[sb.nfree]=free_block;//copy the free block into free[nfree]
  sb.nfree++;//increment nfree
}

//to verify the initialization of the file system
void verify(int fd){
 inode_type temp;
 unsigned short a=0,test;
 a|=1<<15;
 int i=0;
 printf("Checking the allocation status of each inode:\n");
 for(i; i<inode_count; i++){
  lseek(fd, (2*block_size)+((i)*inode_size), SEEK_SET);
  read(fd, &temp, sizeof(temp));
  test=temp.flags&a;//to check if the MSB of the flags field is set to 1 or 0
  if(test==32768){
    printf("Inode %d is allocated\n",i+1);

  }
  else{
   printf("Inode %d is unallocated\n",i+1);
  }
 }
}

//initializing the file system
void initfs(int n1, int n2){
     int g;
     int blockdata[256];
     for(g=0;g<256;g++){
      blockdata[g]=0;
     }
     int j;
     for(j=1; j<n1; j++){
       lseek(fd, j*block_size, SEEK_SET);
       write(fd, &blockdata, block_size);//write the 0's to all the data blocks indicating that they are currently not being used
     }
     //intializing the nfree and free array values to '0' before adding any free data blocks
     sb.nfree=0;
     int i;
     for(i=0;i<251;i++){
        sb.free[i]=0;
     }
     sb.fsize = n1;
    sb.isize=n2;
     //initializing the inodes with flags field as '0' and writing them into their respective positions in the inode blocks
     inode_count=(n2*block_size)/inode_size;
     int k;
     for(k=2;k<=inode_count;k++){
     	initialize_inode(k);
     }
     sb.flock='z';
     sb.ilock='z';
     sb.fmod='z';
     sb.time=0;
     int free_block;
     for(free_block=2+n2; free_block<n1;free_block++){
       addFreeBlock(free_block);
     }
     
     //initializing the root inode 
     root_inode.flags=0;
     root_inode.flags|=1<<15;//root-inode is allocated
     root_inode.flags|=1<<14;//file type pointed by this inode is a directory
     root_inode.nlinks=1;//as there is only link pointing to the root which is the root itself
     root_inode.uid=0;
     root_inode.gid=0;
     root_inode.size0=0;
     root_inode.size1=0;//root directory is empty as there are no other entries in the root directory
     root_inode.actime=0;
     root_inode.modtime=0;
     //initializing the addr[] values with zero before writing the entries into the root directory
     int y;
     for(y=0; y<9; y++){
        root_inode.addr[y]=0;

     }

     //getting a free block to store the entries of the root directory
     int root_inode_block=getFreeBlock();
    // printf("root block is %d\n", root_inode_block);
     dir_type dir_entry[32];//the block is sliced into 32 slices of 32 bytes each 
     dir_entry[0].inode=1;//inode value for the root is '1'
     strcpy(dir_entry[0].filename,".");//points to the root directory
     dir_entry[1].inode=1;
     strcpy(dir_entry[1].filename,"..");//points to the parent of the root directory which is the root itself
     lseek(fd, root_inode_block*block_size, SEEK_SET);
     write(fd, &dir_entry, sizeof(dir_entry));//writing the 2 entries into the free block obtained
     root_inode.addr[0]=root_inode_block;//copying the block number into addr[0] of the root inode
     lseek(fd, 2*block_size, SEEK_SET);
     write(fd, &root_inode, sizeof(root_inode));//writing the root inode into the first 64 bytes of the block number 2
     
     root_inode.size1=2*sizeof(dir_type);
     printf("Size of the root inode is %d\n", root_inode.size1);
     lseek(fd, 2*block_size, SEEK_SET);
     write(fd, &root_inode, sizeof(root_inode));

     //storing the inode number of the present working directory
     current_inumber=1;
     strcpy(pwd,"/");//right after the initialization the root is the present working directory
     verify(fd);//TO VERIFY THE INITIALIZATION OF THE V6 FILE SYSTEM
     printf("File system has been initialized in the given pathname\n");

     //write the super block into the file system after skipping the bootloader block
     lseek(fd, block_size, SEEK_SET);
     write(fd, &sb, sizeof(sb));
}

//getting a free block from the free array
int getFreeBlock(){
 sb.nfree--;//decrement the nfree value
 //printf("nfree value is %d \n", sb.nfree);
if(sb.nfree>0){
  if(sb.free[sb.nfree]==0){//indicates that the file system is full
  printf("File system is full\n");
  return -1;
  }
  else{
   return sb.free[sb.nfree];//return the block number if the file number is not full
  }
 }
 else if(sb.nfree==0){//if nfree is zero after decrementing 
   int b_number=sb.free[sb.nfree];
   lseek(fd, b_number*block_size, SEEK_SET);
   read(fd, &sb.nfree, sizeof(sb.nfree));//read the first word in that block which gives us the nfree value
   lseek(fd, b_number*block_size + sizeof(sb.nfree), SEEK_SET);
   read(fd, &sb.free, sizeof(sb.free));//read the next nfree number of bytes in that block to get the free block numbers
   return b_number;//return the free block number
 }
 else
  return -1;
}

//returns an inode number which has not been allocated
int getFreeInode(){
  int i=1;
  int a =1<<15;
  int j;
  inode_type test_inode;
  for(i;i<inode_count;i++){
    lseek(fd, ((2*block_size)+(i*64)), SEEK_SET);
    read(fd, &test_inode ,sizeof(test_inode));
    j=(a & test_inode.flags);
    if(j == 0){//comparing the MSB of the flags field
      return i+1;
      break;
    }
   }
  return -1;
}

//to fetch the inode contents using the inode number
inode_type getInode(int inumber){
  inode_type temp_inode;
  lseek(fd,((2*block_size)+(inumber-1)*64) ,SEEK_SET);
  read(fd, &temp_inode, sizeof(temp_inode));
  return temp_inode;
}

//unallocating the inode
void addFreeInode(int inumber){
  inode_type freed_inode;
  freed_inode.flags=0;
  freed_inode.uid=0;
  freed_inode.gid=0;
  freed_inode.size0=0;
  freed_inode.size1=0;
  freed_inode.nlinks=0;
  freed_inode.actime=0;
  freed_inode.modtime=0;
  lseek(fd, 2*block_size+(inumber-1)*64, SEEK_SET);
  write(fd, &freed_inode, sizeof(freed_inode));
}

//implementing the cpin command
void cpin(char *extFile, char *v6File){
  int ext, b_number;
  int j=0;
 ext=open(extFile, O_CREAT|O_RDWR, S_IWUSR|S_IRUSR);//opening the external file
  if(ext==-1){
    printf("No file descriptors available to open the file\n");
    return;
  }
  int fil_inode=getFreeInode();//fetching an unallocated inode to point towards the new v6file to be created
  if(fil_inode==-1){
   printf("There are no free inodes to be allocated to the new file\n");
   return;
  }
  inode_type v6fil;
  v6fil.flags=0;
  v6fil.flags|=1<<15;
  v6fil.nlinks=1;
  v6fil.uid=0;
  v6fil.gid=0;
  v6fil.size0=0;
  v6fil.size1=0;
  v6fil.actime=time(NULL);
  v6fil.modtime=time(NULL);

  int bytes_read = block_size;
  int i=0;
  char data_buffer[1024];
  while(bytes_read==block_size){//whenever the bytes read is less than 1024, it indicates that we reached the end of the file
    bytes_read=read(ext, &data_buffer, block_size);//reading the data from the file
    v6fil.size1+=bytes_read;
    b_number=getFreeBlock();//fetching the free block to store the 1024 bytes of the external file
    if(b_number==-1){
      printf("File system is full-- no free blocks available to store the contents\n");
      for(j;j<i;j++){
         addFreeBlock(v6fil.addr[j]);
      }
      return;
    }
    v6fil.addr[i++]=b_number;//copying the free block number into the addr array of the unallocated inode
    lseek(fd, b_number*1024, SEEK_SET);
    write(fd, &data_buffer, bytes_read);//copying the data into the free block
 } 
  
  int inode_bnumber=(fil_inode*inode_size)/block_size+2;
  int offset = (fil_inode*inode_size)%block_size-64;
  lseek(fd, (block_size*inode_bnumber)+offset, SEEK_SET);
  write(fd, &v6fil, sizeof(v6fil));//writing back the originally unallocated inode with the new changes

  inode_type current_inode = getInode(current_inumber);//updating the pwd inode
  b_number = current_inode.addr[0];
  dir_type newFile;
  newFile.inode = fil_inode ;
  strcpy(newFile.filename, v6File);

  lseek(fd, (block_size*b_number)+current_inode.size1,SEEK_SET);
  write(fd, &newFile,sizeof(dir_type));//adding a new entry corresponding to the newly created file

  current_inode.size1 += sizeof(dir_type);//increasing the size of the pwd

  int inode_bnumber1=(current_inumber*inode_size)/block_size+2;
  int offset1 = (current_inumber*inode_size)%block_size-64;
  lseek(fd, (block_size*inode_bnumber1)+offset1, SEEK_SET);
  write(fd, &current_inode, sizeof(current_inode));//writing back the changes made to the pwd inode
}

//implementing the cpout command
void cpout(char *extFile, char *v6file){
  int v6fil,b_number,x,i;
  inode_type v6_file;
  char buffer[1024] = {0};
  v6fil=open(extFile,O_RDWR|O_CREAT,0600);//opening the external file
  if(v6fil == -1)
  {
    printf("\nError while opening the file\n");
    return;
  }
  inode_type current_inode = getInode(current_inumber);//fetching the pwd inode
  b_number = current_inode.addr[0];
  dir_type dir[32];
   int test;
  lseek(fd, block_size*b_number,SEEK_SET);
  read(fd, &dir, sizeof(dir));
  for(i = 0; i < 32; i++)
  {
    if(strcmp(v6file,dir[i].filename)==0){//checking the directory entries of the pwd
      v6_file = getInode(dir[i].inode);
       test=v6_file.flags & 1<<15;   
      if(test==32768){
        for(x = 0; x<v6_file.size1/block_size; x++)
        {
          b_number= v6_file.addr[x];

          lseek(fd, block_size*b_number,SEEK_SET);
          read(fd, &buffer, block_size);
          write(v6fil, &buffer, block_size);//copying the data block by block and writing it to the external file
        }
        b_number = v6_file.addr[x];
        
        lseek(fd, block_size*b_number,SEEK_SET);
        read(fd, &buffer, v6_file.size1%block_size);
        write(v6fil, &buffer,v6_file.size1%block_size);

        v6_file.actime=time(NULL);
        lseek(fd,((2*block_size) + (dir[i].inode-1)*64) ,SEEK_SET);
        write(fd, &v6_file, inode_size);//writing back the changes made to the inode of the v6 file
	return;
      }
      else{
        printf("\n%s\n","The given input for the v6file parameter is not a valid input");
        return;
      }
    }
  }
  printf("The v6file does not exist\n");
  return;
}

//implementing the mkdir command
void makedir(char* dirName)
{
  int b_number = getFreeBlock(); //free block to store the directory entries
 int i_number = getFreeInode();// inode number for the directory
 if(i_number==-1|| b_number==-1){
  if(i_number==-1){
   printf("There are no inodes available to be allotted to the new directory.\n");
   return;}
 else{
   printf("The file system is full so cannot create a new directory.\n"); 
   return;}
}
 else{
 printf("Inode allocated for the directory %s is : %d\n", dirName, i_number);
 dir_type dir[2];
  dir[0].inode = i_number;
  strcpy(dir[0].filename,dirName);

  dir[1].inode = current_inumber;
  strcpy(dir[1].filename, pwd);

   lseek(fd,b_number*block_size ,SEEK_SET);//storing the 2 entries in the free data block
   write(fd, &dir, 2*sizeof(dir_type));
 
 //initializing the inode for the new directory
  inode_type dir_inode;
  dir_inode.flags = 1<<14 | 1<<15; // setting 14th and 15th bit to 1, 15: allocated and 14: directory
  dir_inode.nlinks = 1;
  dir_inode.uid = 0;
  dir_inode.gid = 0;
  dir_inode.size0=0;
  dir_inode.size1 = 2*sizeof(dir_type);
  dir_inode.addr[0] = b_number;//storing the data block in the addr array
  dir_inode.actime = time(NULL);
  dir_inode.modtime = time(NULL);

  //writing the inode struct into the location of the originally unallocated inode
  int inode_bnumber=(i_number*inode_size)/block_size+2;
  int offset = (i_number*inode_size)%block_size-64;
  lseek(fd, (block_size*inode_bnumber)+offset, SEEK_SET);
  write(fd, &dir_inode, sizeof(dir_inode));

  //fetching the inode of the present working directory
  inode_type current_inode = getInode(current_inumber);
  dir_type newDir;
  newDir.inode = i_number;
  strcpy(newDir.filename,dirName);

  //adding the entry of the new directory
  lseek(fd, current_inode.addr[0]*1024+current_inode.size1,SEEK_SET);
  write(fd, &newDir, sizeof(newDir));
  current_inode.size1 += sizeof(dir_type);
  
  //writing back the current inode into it's location after updating the new entry
  int inode_bnumber1=(current_inumber*inode_size)/block_size+2;
  int offset1 = (current_inumber*inode_size)%block_size-64;
  lseek(fd, (block_size*inode_bnumber1)+offset1, SEEK_SET);
  write(fd, &current_inode, sizeof(current_inode));
  return;
}
}

//implementing the cd command
void changedir(char* dirName)
{
  inode_type current_inode = getInode(current_inumber);
  if(strcmp(dirName,"..")==0){//when the input is to change to the parent directory
    if(current_inumber==1){
     printf("As the pwd is the root cd .. has no effect\n");
      return;
      }
    else{
      lseek(fd, current_inode.addr[0]*block_size ,SEEK_SET);
      dir_type self;
      read(fd, &self, sizeof(self));
      dir_type parent;
      lseek(fd, 0, SEEK_CUR);
      read(fd,&parent, sizeof(parent));
      current_inumber=parent.inode;//setting the current inumber as the inode of the parent directory
      strcat(self.filename,"/");
      char newpath[100]="";
      strncpy(newpath, &pwd[0], strlen(pwd)-strlen(self.filename));
      strcpy(pwd,newpath);
      printf("The current inode number is %d\n", current_inumber);
      printf("The new present working directory is %s\n", pwd);
     return;
 }
 }
 int b_number = current_inode.addr[0];//fetching the current directory inode
  dir_type dir[32];
  int i;
  int temp_inumber;
  inode_type dir_inode;
  //readFromBlockOffset(blockNumber,0,directory,curINode.size);
  lseek(fd, block_size*b_number, SEEK_SET);
  read(fd, &dir, sizeof(dir));

  for(i = 0; i < 32; i++) {
    if(strcmp(dirName,dir[i].filename)==0){
      dir_inode = getInode(dir[i].inode);
      if(dir_inode.flags ==( 1<<14 | 1<<15)){//checking the flags field if it's a directory and wether it has been allocated or not
        temp_inumber=dir[i].inode;
	break;
      }
    }
   }
    if(temp_inumber==0){
        printf("Directory not found in the path\n");
        return ;
    }
     dir_inode.actime=time(NULL);
     int x1=2+(temp_inumber*64/1024);
     int x1_offset=(temp_inumber*64)%block_size-64;
     lseek(fd, x1*block_size+x1_offset, SEEK_SET);
     write(fd, &dir_inode, sizeof(dir_inode)); //writing back the changes in the inode of the new directory
     current_inumber=temp_inumber;//setting the new directory inode number current inode number 
     strcat(pwd, dirName);
     strcat(pwd, "/");//updating the present working directory
     printf("The current inode number for the directory %s is %d\n", dirName, current_inumber);
     printf("The current working directory is %s\n", pwd);
     return;
}

//implementing the rm command
void rm(char* filename){
  int b_number,x,i;
  inode_type file_inode;
  inode_type current_inode = getInode(current_inumber);//fetching the inode of the present working directory
  b_number = current_inode.addr[0];
  dir_type dir[32];
  lseek(fd, block_size*b_number, SEEK_SET);
  read(fd, &dir, sizeof(dir));

  for(i = 0; i < 32; i++) {
    if(strcmp(filename,dir[i].filename)==0){//checking the directory entries of the present working directory
      file_inode = getInode(dir[i].inode);
      file_inode.flags=file_inode.flags & 1<<15;
      if(file_inode.flags==32768){
        for(x = 0; x<file_inode.size1/block_size; x++)
        {
          b_number = file_inode.addr[x];
          addFreeBlock(b_number);//adding the data blocks to the free list
        }
        if(0<file_inode.size1%block_size){
          b_number = file_inode.addr[x];
          addFreeBlock(b_number);
        }
        addFreeInode(dir[i].inode);//unallocating the inode
	      printf("The freed inode is %d \n", dir[i].inode);
        dir[i]=dir[(current_inode.size1/sizeof(dir_type))-1];
        current_inode.size1-=sizeof(dir_type);//decrementing the pwd size after deleting the entry of the deleted file
        
        lseek(fd, current_inode.addr[0]*block_size, SEEK_SET);
        write(fd, &dir, current_inode.size1);
        
        int inode_bnumber=(current_inumber*inode_size)/block_size+2;
        int offset = (current_inumber*inode_size)%block_size-64;
        lseek(fd, (block_size*inode_bnumber)+offset, SEEK_SET);
        write(fd, &current_inode, sizeof(current_inode));//writing back the changes made to the inode
        return;
      }
      else{
        printf("\n%s\n","File not found\n");
        return;
      }
    }
  }
  printf("File not found\n");
  return;
}

//main function
int main(){
  char command[100];
  char pathname[100];
  char *cmd, *argument1, *argument2;
  while(1){
  printf("Enter command: ");
  scanf(" %[^\n]s", command);
  cmd = strtok(command, " ");

  if (strcmp(cmd, "q") == 0)
    q(fd); //calling the quit function
  else if (strcmp(cmd, "openfs") == 0) {
    argument1 = strtok(NULL, " ");
    strcpy(pathname, argument1);
    if(openfs(pathname) == 0)//calling the openfs function
    initfs(n1, n2);
  }
  else if (strcmp(cmd, "cpin") == 0) {
   argument1 = strtok(NULL, " ");
   argument2 = strtok(NULL, " ");
   cpin(argument1, argument2);//calling the cpin function
  }
  else if (strcmp(cmd, "cpout") == 0) {
   argument1 = strtok(NULL, " ");
   argument2 = strtok(NULL, " ");
   cpout(argument1, argument2);//calling the cpout function
  }
  else if (strcmp(cmd, "rm") == 0) {
   argument1 = strtok(NULL, " ");
   rm(argument1);//calling the rm function
  }
  else if (strcmp(cmd, "mkdir") == 0) {
   argument1 = strtok(NULL, " ");
   makedir(argument1);//calling the mkdir function
  }
  else if (strcmp(cmd, "cd") == 0) {
   argument1 = strtok(NULL, " ");
   changedir(argument1);//calling the changedir function
  }
  else
    printf("Invalid command, enter a valid command\n");//error message if the given command is an invalid one
  }
  return 0;
}
