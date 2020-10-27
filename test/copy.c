/* copy.c
 * Simple program that copy a file and parse to another destination file
 * This program will read source file, create and write to destinaion file
*/

#include "syscall.h"

#define FILENAME_BUFFER_SIZE 65
#define BUFFER_SIZE 256
#define OPEN_RW 0
#define OPEN_R 1

#define PROGRAM_INS "***Copy program | Max file name length allowed: 64***\n"
#define PROGRAM_INS_LEN 54
#define SRC_REQUEST "Source file name: "
#define SRC_REQUEST_LEN 18
#define DST_REQUEST "Destination file name: "
#define DST_REQUEST_LEN 23
#define ERR_OPEN_SRC "Error: Cant open source file\n"
#define ERR_OPEN_SRC_LEN 29
#define ERR_CREATE_DST "Error: Cant create destination file\n"
#define ERR_CREATE_DST_LEN 36
#define ERR_OPEN_DST "Error: Cant open destination file\n"
#define ERR_OPEN_DST_LEN 34
#define ERR_COPY "Error: Unexpected error occured when copying file\n"
#define ERR_COPY_LEN 50
#define MSG_SUCCESS "DONE! File was copied successfully\n"
#define MSG_SUCCESS_LEN 35


int main()
{
	char srcName[FILENAME_BUFFER_SIZE];
	char dstName[FILENAME_BUFFER_SIZE];
	char buffer[BUFFER_SIZE];
	int srcId, dstId;
	int bytes;
	
	PrintS(PROGRAM_INS, PROGRAM_INS_LEN);

	// Get source file name
	PrintS(SRC_REQUEST, SRC_REQUEST_LEN);
	ReadS(srcName, FILENAME_BUFFER_SIZE);

	// Get destination file name
	PrintS(DST_REQUEST, DST_REQUEST_LEN);
	ReadS(dstName, FILENAME_BUFFER_SIZE);
	
	// Open source file (read-only)
	srcId = Open(srcName, OPEN_R);
	if (srcId == -1)
	{
		PrintS(ERR_OPEN_SRC, ERR_OPEN_SRC_LEN);
		return -1;
	}
	
	// Create dest file
	dstId = Create(dstName);
	if (dstId < 0)
	{
		PrintS(ERR_CREATE_DST, ERR_CREATE_DST_LEN);
		return -1;
	}

	// Open dest file for write
	dstId = Open(dstName, OPEN_RW);
	if (dstId == -1)
	{
		PrintS(ERR_OPEN_DST, ERR_OPEN_DST_LEN);
		return -1;
	}

	// Read block of BUFFER_SIZE bytes from src and write to dst repeatedly until src end
	bytes = Read(buffer, BUFFER_SIZE, srcId);
	while (bytes > 0)
	{
		Write(buffer, bytes, dstId);
		bytes = Read(buffer, BUFFER_SIZE, srcId);
	} 

	// File copy corrupted
	if (bytes == -1)
	{
		PrintS(ERR_COPY, ERR_COPY_LEN);
		return -1;
	}
	else
	{
		PrintS(MSG_SUCCESS, MSG_SUCCESS_LEN);
	}

	return 0;
}

