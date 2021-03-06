// filesys.h 
//	Data structures to represent the Nachos file system.
//
//	A file system is a set of files stored on disk, organized
//	into directories.  Operations on the file system have to
//	do with "naming" -- creating, opening, and deleting files,
//	given a textual file name.  Operations on an individual
//	"open" file (read, write, close) are to be found in the OpenFile
//	class (openfile.h).
//
//	We define two separate implementations of the file system. 
//	The "STUB" version just re-defines the Nachos file system 
//	operations as operations on the native UNIX file system on the machine
//	running the Nachos simulation.  This is provided in case the
//	multiprogramming and virtual memory assignments (which make use
//	of the file system) are done before the file system assignment.
//
//	The other version is a "real" file system, built on top of 
//	a disk simulator.  The disk is simulated using the native UNIX 
//	file system (in a file named "DISK"). 
//
//	In the "real" implementation, there are two key data structures used 
//	in the file system.  There is a single "root" directory, listing
//	all of the files in the file system; unlike UNIX, the baseline
//	system does not provide a hierarchical directory structure.  
//	In addition, there is a bitmap for allocating
//	disk sectors.  Both the root directory and the bitmap are themselves
//	stored as files in the Nachos file system -- this causes an interesting
//	bootstrap problem when the simulated disk is initialized. 
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef FS_H
#define FS_H

#include "copyright.h"
#include "openfile.h"


typedef int OpenFileId;	// ID of file opened

#define MAX_OPEN_FILE 10 // Maximum number of files allowed to open (max size of file table)

#ifdef FILESYS_STUB 		// Temporarily implement file system calls as 
				// calls to UNIX, until the real file system
				// implementation is available
class FileSystem {
  private:
	OpenFile** _open_files; // Open file table. Hold info of files opened
  	int FindFreeIndex(){ // Find available slot in open files table
		for (int i = 0; i < MAX_OPEN_FILE; ++i)
		{
			if (_open_files[i] == NULL)
			{
				return i;
			}
		}
		return -1;
	}
  public:
    FileSystem(bool format) {
		// Init open files
		_open_files = new OpenFile*[MAX_OPEN_FILE];
		for (int i = 0; i < MAX_OPEN_FILE; ++i)
		{
			_open_files[i] = NULL;
		}

		// Init first two file (stdin and stdout)
		Create("stdin", 0);
		Create("stdout", 0);

		// Open stdin and stdout. Its file id should be 0 and 1
		_open_files[0] =  Open("stdin", 2);
		_open_files[1] = Open("stdout", 3);
	}


	// Destructor for file system
	~FileSystem()
	{
		for (int i = 0; i < MAX_OPEN_FILE; ++i)
		{
			if (_open_files[i] != NULL)
			{
				delete _open_files[i];	
			}
		}

		delete[] _open_files;
	}


    bool Create(char *name, int initialSize) { 
	int fileDescriptor = OpenForWrite(name);

	if (fileDescriptor == -1) return FALSE;
	Close(fileDescriptor); 
	return TRUE; 
	}

	// Default method
    OpenFile* Open(char *name) {
	  int fileDescriptor = OpenForReadWrite(name, FALSE);

	  if (fileDescriptor == -1) return NULL;

	  return new OpenFile(fileDescriptor);
      }
	
	OpenFile* Open(char* name, int type){
	  int fileDescriptor = OpenForReadWrite(name, FALSE);

	  if (fileDescriptor == -1) return NULL;
	  return new OpenFile(fileDescriptor, type);
	  }	
	
	OpenFile* Open(char* name, int type, OpenFileId& id){
	  int fileDescriptor = OpenForReadWrite(name, FALSE);

	  if (fileDescriptor == -1) return NULL;
	  if (id < 0 || id >= MAX_OPEN_FILE || _open_files[id] != NULL)
	  {
	 	  id = FindFreeIndex();
	  }
	  if (id < 0) return NULL;
	  _open_files[id] = new OpenFile(fileDescriptor, type);
	  return _open_files[id]; 
	  }	
	int Close(OpenFileId id)
	{
		if (id < 0 || id >= MAX_OPEN_FILE || _open_files[id] == NULL)
		{
			return -1;
		}
		delete _open_files[id];
		_open_files[id] = NULL;
		return 0;
	}
    bool Remove(char *name) { return Unlink(name) == 0; }
	
	OpenFile* FileAt(OpenFileId id){ // Return file opened with specified id
	if (id < 0 || id >= MAX_OPEN_FILE)
	{
		return NULL;
	}
	return _open_files[id];
	}

};

#else // FILESYS
class FileSystem {
  public:
    FileSystem(bool format);		// Initialize the file system.
					// Must be called *after* "synchDisk" 
					// has been initialized.
    					// If "format", there is nothing on
					// the disk, so initialize the directory
    					// and the bitmap of free blocks.
	~FileSystem(); // Destructor of file system
    bool Create(char *name, int initialSize);  	
					// Create a file (UNIX creat)

    OpenFile* Open(char *name); 	// Open a file (UNIX open)
	OpenFile* Open(char *name, int type);	// Open a file (with type specified (read, write,...))	
	OpenFile* Open(char *name, int type, OpenFileId& id);	// Open a file (with type specified (read, write,...))
													// id is where file will be placed on open files table. If this is not valid, another place will be used 
	int Close(OpenFileId id);	// Close a file with OpenFileId given

    bool Remove(char *name);  		// Delete a file (UNIX unlink)

    void List();			// List all the files in the file system

    void Print();			// List all the files and their contents
	
	OpenFile* FileAt(OpenFileId id);	// Return file object opened with specified id
  private:
   OpenFile* freeMapFile;		// Bit map of free disk blocks,
					// represented as a file
   OpenFile* directoryFile;		// "Root" directory -- list of 
					// file names, represented as a file
	OpenFile** _open_files; // Open file table. Hold info of files opened
	
	int FindFreeIndex(); // Find available slot in open files table
};

#endif // FILESYS

#endif // FS_H
