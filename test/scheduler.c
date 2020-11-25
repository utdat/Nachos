/* scheduler.c
 * Simple program that simultaneusly run ping and pong 
 * (print A/B 1000 times each simultaneusly)
*/

#include "syscall.h"
#define SCHEDULER_INS "***Scheduler program | Simultaneusly Ping and Pong***\n" 
#define SCHEDULER_INS_LEN 54
#define PING "./test/ping"
#define PONG "./test/pong"

int main()
{
	int pingId, pongId, i;
	PrintS(SCHEDULER_INS, SCHEDULER_INS_LEN);
	pingId = Exec(PING);
	pongId = Exec(PONG);
	Await(pingId);
	Await(pongId);
	PrintS("\n", 1);
	return 0;
}
