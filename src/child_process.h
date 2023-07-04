#ifndef CHILD_PROCESS_H
#define CHILD_PROCESS_H

#include "parent_process.h"

vector<string> convertInputParamsToVector(int namedPipeMainToChildFD);

vector<string> getInputParametersFromMain(int namedPipeMainToChildFD, int namedPipeChildToMainFD);

void childProcessTask(int namedPipeMainToChildFD, int namedPipeChildToMainFD);

vector<string> getOpCodeAndInputParamsFromPipe(int namedPipeMainToChildFD, int namedPipeChildToMainFD, int &opCode);

void signalHandler(int signal);


#endif // CHILD_PROCESS_H