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
  if (strlen(name) > MAX_FNAME_SIZE) {
    cerr << "Error: File name is too long." << endl;
    return;
  }

  // check: directory exists
  dirblock_t curr_d_block;
  bfs.read_block(curr_dir, (void*) &curr_d_block);

  for (int i = 0; i < curr_d_block.num_entries; i++) {
    if (strcmp(name, curr_d_block.dir_entries[i].name) == 0) {
      cerr << "Error: Directory exists." << endl;
      return;
    }
  }

  // check if curr dir has space for new dir
  if (curr_d_block.num_entries == MAX_DIR_ENTRIES) {
    cerr << "Error: Directory is full." << endl;
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
      short new_block = curr_dir_block.dir_entries[i].block_num;
      
      if (!is_directory(new_block)) {   // not dir
        cerr << "Error: File is not a directory." << endl;
        return;
      }
       else {   // match & is directory
        curr_dir = new_block;
        return;
      }
    }
  }
  cerr << "Error: File does not exist." << endl;
}

// switch to home directory
void FileSys::home() {
  curr_dir = HOME_DIR;
}

// remove a directory
void FileSys::rmdir(const char *name)
{
  dirblock_t curr_dir_block;    // get curr dir block
  bfs.read_block(curr_dir, (void*) &curr_dir_block);

  for (int i = 0; i < curr_dir_block.num_entries; i++) {
    if (strcmp(name, curr_dir_block.dir_entries[i].name) == 0) {
      short find_block = curr_dir_block.dir_entries[i].block_num;


      if (!is_directory(find_block)) {   // not dir
          cerr << "Error: File is not a directory." << endl;
          return;
      }
      
      else {
        dirblock_t dir_to_remove;
        bfs.read_block(find_block, (void*) &dir_to_remove);

        if (dir_to_remove.num_entries != 0) {
          cerr << "Error: Directory is not empty." << endl;
          return;
        }

        bfs.reclaim_block(find_block);

        remove_helper(i);
        return;
      }
    }
  }
  cerr << "Error: File does not exist." << endl;
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
  // validate directory name
  if (strlen(name) > MAX_FNAME_SIZE) {
    cerr << "Error: File name is too long." << endl;
    return;
  }

  // check: file exists
  dirblock_t curr_dir_block;
  bfs.read_block(curr_dir, (void*) &curr_dir_block);

  for (int i = 0; i < curr_dir_block.num_entries; i++) {
    if (strcmp(curr_dir_block.dir_entries[i].name, name) == 0) {
        cerr << "Error: File exists." << endl;
        return;
    }
  }

  // check if curr dir has space for new dir
  if (curr_dir_block.num_entries >= MAX_DIR_ENTRIES) {
    cerr << "Error: Directory is full." << endl;
    return;
  }

  // find a free block
  short new_block_num = bfs.get_free_block();
  if (new_block_num == 0) {
      cerr << "Error: Disk is full." << endl;
      return;
  }

  // init new inode block
  inode_t new_inode;
  new_inode.magic = INODE_MAGIC_NUM;
  new_inode.size = 0;
  for (int i = 0; i < MAX_DATA_BLOCKS; i++) {
      new_inode.blocks[i] = 0;
  }

  // write new file(inode) to disk
  bfs.write_block(new_block_num, (void*) &new_inode);

  // add new file to curr dir
  int index = curr_dir_block.num_entries;
  strcpy(curr_dir_block.dir_entries[index].name, name);
  curr_dir_block.dir_entries[index].block_num = new_block_num;
  curr_dir_block.num_entries++;

  // update curr dir to disk
  bfs.write_block(curr_dir, (void*) &curr_dir_block);
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

/**
 * @brief checks if given block num correlates to dir
 * @param block_num - given block num
 * @return true/false 
 */
bool FileSys::is_directory(short block_num) {
  dirblock_t block;
  bfs.read_block(block_num, (void*) &block);

  if (block.magic == DIR_MAGIC_NUM) {   // check w/ magic num
    return true;
  }
  return false;
}

/**
 * @brief Helper method to remove an entry at the given index from the curr
 * dir's entry array
 * @param index - index whose entry will be removed
 */
void FileSys:: remove_helper(int index) {
  dirblock_t curr_dir_block;
  bfs.read_block(curr_dir, (void*) &curr_dir_block);

  for (int i = index; i < curr_dir_block.num_entries; i++) {    // cycle & replace
    curr_dir_block.dir_entries[i] = curr_dir_block.dir_entries[i + 1];
  }

  curr_dir_block.dir_entries[curr_dir_block.num_entries - 1].block_num = 0;   // mark as empty
  curr_dir_block.num_entries --;    
  bfs.write_block(curr_dir, (void*) & curr_dir_block);    // write back to disk
}
