/* open.c
 * Simple program to test open file syscall
 * Just open file and do nothing
 */

#include "syscall.h"

#define PROGRAM_INS "***Open Close Program | Open file | Max file length 64***\n"
#define PROGRAM_INS_LEN 58
#define SUCCESS_MSG "File was successfully opened. File ID: "
#define SUCCESS_MSG_LEN 39
#define ERROR_MSG "Error\n"
#define ERROR_MSG_LEN 6
#define FILE_CLOSED_MSG "File closed\n"
#define FILE_CLOSED_MSG_LEN 12

#define NAME_BUFFER_SIZE 65

int main()
{
	// Name of file
	char fileName[NAME_BUFFER_SIZE];

	// ID of opened file
	OpenFileId id;

	// Convert id to char array to print
	char txtId[4];
	txtId[0] = '0';
	txtId[1] = '0';
	txtId[2] = '\n';
	txtId[3] = '\0';

	// Get file name
	PrintS(PROGRAM_INS, PROGRAM_INS_LEN);
	ReadS(fileName, NAME_BUFFER_SIZE);

	// Open file and check for error/success
	id = Open(fileName, 0);
	if (id >= 0)
	{
		PrintS(SUCCESS_MSG, SUCCESS_MSG_LEN);
		if (id == 10)
		{
			txtId[0] = '1';
		}
		else
		{
			txtId[1] = '0' + id;
		}
		PrintS(txtId, 3);
	}
	else
	{
		PrintS(ERROR_MSG, ERROR_MSG_LEN);
	}
	
	// Close file
	Close(id);
	PrintS(FILE_CLOSED_MSG, FILE_CLOSED_MSG_LEN);
	return 0;
}
