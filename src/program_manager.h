#ifndef PROGRAM_MANAGER_H
#define PROGRAM_MANAGER_H

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <filesystem>
#include "programs.h"
#include "exception"

#define STOUT_FILENO 1
#define OPTION_NUM_FIVE 5

enum class MenuOptions {
    FetchAirportsIncomingFlights = 1,
    FetchAirportsFullFlightSchedule,
    FetchAircraftsFullFlightSchedule,
    UpdateDatabase,
    ZipDatabaseFiles,
    GetChildProcessID,
    GracefulExit
};

void printMenu();

void clearScreen();

void waitForUserInputToContinue();

void createNewPipe(int pipeFd);

pid_t createNewProcess();

string readStringFromChild(int pipeFd);

int getUserOpcodeInput();

pid_t sendChildPIDtoParent(int pipeFd, pid_t pid);

void bufferFlush();

string readStringFromParent(int pipefd);

void unzipDB();

#endif // PROGRAM_MANAGER_H