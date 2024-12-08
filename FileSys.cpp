// Computing Systems: File System
// Implements the file system commands that are available to the shell.

#include <cstring>
#include <iostream>
using namespace std;

#include "FileSys.h"
#include "BasicFileSys.h"
#include "Blocks.h"

// mounts the file system
void FileSys::mount() {
  bfs.mount();
  curr_dir = 1;
}

// unmounts the file system
void FileSys::unmount() {
  bfs.unmount();
}

// make a directory
void FileSys::mkdir(const char *name)
{
  // validate directory name
  if (strlen(name) == 0 || strlen(name) > MAX_FNAME_SIZE) {
    cerr << "Error: Invalid directory name given." << endl;
    return;
  }

  // check: directory exists
  dirblock_t curr_d_block;
  bfs.read_block(curr_dir, (void*) &curr_d_block);

  for (int i = 0; i < curr_d_block.num_entries; i++) {
    if (strcmp(name, curr_d_block.dir_entries[i].name) == 0) {
      cerr << "Error: Directory already exists." << endl;
      return;
    }
  }

  // check if curr dir has space for new dir
  if (curr_d_block.num_entries == MAX_DIR_ENTRIES) {
    cerr << "Error: Maximum number of directory entries reached." << endl;
    return;
  }

  // find new block num
  short new_block_num = bfs.get_free_block();
  if (new_block_num == 0) {
    cerr << "Error: Disk is full." << endl;
    return;
  }

  // init new dir block
  dirblock_t directory_block;
  directory_block.magic = DIR_MAGIC_NUM;
  directory_block.num_entries = 0;

  // write new dir to disk
  bfs.write_block(new_block_num, (void*) &directory_block);
  
  // add new dir to curr dir
  int index = curr_d_block.num_entries;
  strcpy(curr_d_block.dir_entries[index].name, name); 
  curr_d_block.dir_entries[index].block_num = new_block_num;
  curr_d_block.num_entries++;

  // write curr dir to disk
  bfs.write_block(curr_dir, (void*) &curr_d_block);
}

// switch to a directory
void FileSys::cd(const char *name)
{
  dirblock_t curr_dir_block;    // get curr dir block
  bfs.read_block(curr_dir, (void*) &curr_dir_block);

  for (int i = 0; i < curr_dir_block.num_entries; i++) {  // iterate thru dir entries
    if (strcmp(curr_dir_block.dir_entries[i].name, name) == 0) {    // dir name matches
      curr_dir = curr_dir_block.dir_entries[i].block_num;
      return;
    }
  }
  cerr << "Error: Directory " <<  name << " not found." << endl;
}

// switch to home directory
void FileSys::home() {
  curr_dir = HOME_DIR;
}

// remove a directory
void FileSys::rmdir(const char *name)
{
}

// list the contents of current directory
void FileSys::ls()
{
  dirblock_t curr_dir_block;  // get curr dir block
  bfs.read_block(curr_dir, (void*) &curr_dir_block);

  for (int i = 0; i < curr_dir_block.num_entries; i++) {
    short block = curr_dir_block.dir_entries[i].block_num;
    cout << curr_dir_block.dir_entries[i].name;
    if (is_directory(block)) {  // if is directory, append '/'
      cout << '/' << endl;
    }
    else {  // regular file, no appending
      cout << endl;
    }
  }
}

// create an empty data file
void FileSys::create(const char *name)
{
}

// append data to a data file
void FileSys::append(const char *name, const char *data)
{
}

// display the contents of a data file
void FileSys::cat(const char *name)
{
}

// display the last N bytes of the file
void FileSys::tail(const char *name, unsigned int n)
{
}

// delete a data file
void FileSys::rm(const char *name)
{
}

// display stats about file or directory
void FileSys::stat(const char *name)
{
}

// HELPER FUNCTIONS (optional)
bool FileSys::is_directory(short block_num) {
  dirblock_t block;
  bfs.read_block(block_num, (void*) &block);

  if (block.magic == DIR_MAGIC_NUM) {
    return true;
  }
  return false;
}

