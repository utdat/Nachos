#include "syscall.h"

#define FILENAME_BUFFER_SIZE 65
#define OPEN_R 1
#define GAP 5

#define PROGRAM_INS "***Seek Program | Print 1 char for every 5 char | Max file length 64***\n"
#define PROGRAM_INS_LEN 75
#define REQ_FILENAME "File name: "
#define REQ_FILENAME_LEN 11
#define MSG_EOF "\n(EOF)\n"
#define MSG_EOF_LEN 7

int main()
{
	char filename[FILENAME_BUFFER_SIZE];
	int fid;
	int pos, flen;
	char c;
	PrintS(PROGRAM_INS, PROGRAM_INS_LEN);
	
	// Get file name
	PrintS(REQ_FILENAME, REQ_FILENAME_LEN);
	ReadS(filename, FILENAME_BUFFER_SIZE);

	// Open file name
	fid	= Open(filename, OPEN_R);

	// Get file length by seek to eof
	flen = Seek(-1, fid);

	// Print to IO device 1 char for every 5 char. Length limited
	pos = 0;
	while (pos < flen && Seek(pos, fid) >= 0)
	{
		Read(&c, 1, fid);
		PrintS(&c, 1);
		pos = pos + GAP;
	}

	// Seek at eof
	
	// Seek at eof
	Seek(-1, fid);
	if (Read(&c, 1, fid) == -2)
	{
		PrintS(MSG_EOF, MSG_EOF_LEN);
	}	
	
	Close(fid);
	return 0;
}
