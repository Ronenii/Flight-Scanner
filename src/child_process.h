#ifndef CHILD_PROCESS_H
#define CHILD_PROCESS_H

#include "parent_process.h"

vector<string> convertInputParamsToVector(int *pipeMainToChildFD);

vector<string> getInputParametersFromMain(int *pipeMainToChildFD, int *pipeChildToMainFD);

void childProcessTask(int *pipeMainToChildFD, int *pipeChildToMainFD);

vector<string> getOpCodeAndInputParamsFromPipe(int *pipeMainToChildFD, int *pipeChildToMainFD, int &opCode);

void signalHandler(int signal);


#endif // CHILD_PROCESS_H