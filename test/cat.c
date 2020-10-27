/* cat.c
 * Simple program that read file name from user input then print its data to console
 * This program will use file read and write by default for all operation
*/

#include "syscall.h"

// Size of buffer
#define MAX_FILE_LENGTH 64
#define BUFFER_SIZE 10

// Constants for file operations
#define OPEN_READ_WRITE 0
#define STDIN 0
#define STDOUT 1

// Message
#define PROGRAM_INS "***Cat program | Max file length allowed: 64***\n"
#define PROGRAM_INS_LEN 48


int main()
{
	char fileName[MAX_FILE_LENGTH];
	char fileBuffer[BUFFER_SIZE];	
	int fileId;	
	int byteRead;
		
	PrintS(PROGRAM_INS, PROGRAM_INS_LEN);	
	
	// Get file name
	Read(fileName, MAX_FILE_LENGTH, STDIN);

	// Open file
	fileId = Open(fileName, OPEN_READ_WRITE);

	// Read file buffer by buffer
	byteRead = Read(fileBuffer, BUFFER_SIZE, fileId);
	while (byteRead > 0)
	{
		PrintS(fileBuffer, byteRead);
		byteRead = Read(fileBuffer, BUFFER_SIZE, fileId);
	}

	// Close file
	Close(fileId);
	
	PrintS("\n", 1);
	return 0;
}
