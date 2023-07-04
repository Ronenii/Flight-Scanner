#ifndef PARENT_PROCESS_H
#define PARENT_PROCESS_H

#include "program_manager.h"

void parentProcessTask(int namedPipeMainToChildFD, int namedPipeChildToMainFD, int childPid);

void handleNonChildTasks(int opCode, int childPid, int namedPipeChildToMainFD);

void writeParametersToChildProcess(int namedPipeMainToChildFD, int opCode);

void getParamsFromUserAndSendToChild(int namedPipeMainToChildFD, int opCode);

string getStringsListFromUser(int opCode);

void gracefulExit(int childPid, int namedPipeChildToMainFD);

void printMessageByOpCode(int opCode);

void signalHandlerMainProcess(int signal);

#endif // PARENT_PROCESS_H