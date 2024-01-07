#include "libUtil/util.h"

int test_mat_part1(int argc, char* argv[]);

using namespace std;
using namespace app;
int main(int argc, char* argv[])
{
	const string logFilename("./log.txt");
	const bool showInConsole = true;
	startLogThread(logFilename, showInConsole);

	APP_LOG( "test1,i=%d", 10);
	APP_ASSERT(2 == 2, "xyz=%d,egg=%s", 3, "passed");
	APP_EXIT("xyz=%d,efg=%s", 2, "quit");
	endLogThread();
	return 0;
}

