#include "syscall.h"

#define SCHEDULER_INS "***scheduler program***\n"
#define SCHEDULER_LEN 24

void main()
{
    int pingID, pongID;
    PrintS(SCHEDULER_INS, SCHEDULER_LEN);
    pingID = Exec("./test/ping");
    pongID = Exec("./test/pong");
    Join(pingID);
    Join(pongID);
}
