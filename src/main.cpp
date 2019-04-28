#include <iostream>
#include <string>

#include "DataBaseManager.h"

int main(int argc, char **argv) {
	const int maxCmdLen = (1 << 16) + 3;
	static char cmd[maxCmdLen];

	DataBaseManager master = new DataBaseManager();

	while (cin.getline(cmd, maxCmdLen, '\n'))
	{
		master->handleCommand(cmd);
	}

	return 0;
}