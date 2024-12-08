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
  // val directory name
  if (strlen(name) == 0 || strlen(name) > MAX_FNAME_SIZE) {
    cerr << "Error: Invalid directory name given." << endl;
    return;
  }

  // check: directory exists
  dirblock_t curr_d_block;
  bfs.read_block(curr_dir, (void*) & curr_d_block);

  for (int i = 0; i < new_dir.num_entries; i++) {
    if (curr_dir[i].name == name) {
      cerr << "Error: directory already exists." << endl;
      return;
    }
  }

  // find new block
  short new_d_block = bfs.get_free_block();
  if (new_d_block == 0) {
    cerr << "Error: disk is full." << endl;
    return;
  }


}

// switch to a directory
void FileSys::cd(const char *name)
{
}

// switch to home directory
void FileSys::home() {
}

// remove a directory
void FileSys::rmdir(const char *name)
{
}

// list the contents of current directory
void FileSys::ls()
{
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
bool is_directory(short block_num) {
  dirblock_t block;
  bfs.read_block(block_num, (void*) &block);

  if (block.magic == DIR_MAGIC_NUM) {
    return true;
  }
  return false;
}

