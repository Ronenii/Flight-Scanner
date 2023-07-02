#ifndef PARENT_PROCESS_H
#define PARENT_PROCESS_H

#include "program_manager.h"

void parentProcessTask(int *pipeMainToChildFD, int *pipeChildToMainFD, int childPid);

void handleNonChildTasks(int opCode, int childPid, int *pipeChildToMainFD);

void writeParametersToChildProcess(int *pipeMainToChildFD, int opCode);

void getParamsFromUserAndSendToChild(int *pipeMainToChildFD, int opCode);

string getStringsListFromUser(int opCode);

void gracefulExit(int childPid, int *pipeChildToMainFD);

void printMessageByOpCode(int opCode);

void signalHandlerMainProcess(int signal);

#endif // PARENT_PROCESS_H