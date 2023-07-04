#include "child_process.h"
#include "programs.h"

int main() {

    string namedPipeMainToChild = "namedPipeMainToChild";
    string namedPipeChildToMain = "namedPipeChildToMain";
    mkfifo(namedPipeMainToChild.c_str(), 0666);
    mkfifo(namedPipeChildToMain.c_str(), 0666);

    int namedPipeMainToChildFD, namedPipeChildToMainFD;
    namedPipeMainToChildFD = open(namedPipeMainToChild.c_str(), O_RDWR);
    namedPipeChildToMainFD = open(namedPipeChildToMain.c_str(), O_RDWR);

    pid_t currPID, childPID;

    try {
        unzipDB();
        currPID = createNewProcess();

        // Get the child PID
        childPID = sendChildPIDtoParent(namedPipeChildToMainFD, currPID);

        if (currPID != CHILD_PROCESS_ID) {
            signal(SIGINT, signalHandlerMainProcess);
            setpgid(0, childPID);
            parentProcessTask(namedPipeMainToChildFD, namedPipeChildToMainFD, childPID);

            // close write and read pipes which were in use
            close(namedPipeChildToMainFD);
            close(namedPipeMainToChildFD);
        } else {
            // Allow child process' signal handler to handle SIGUSR1 and SIGUSR2
            signal(SIGUSR1, signalHandler);
            signal(SIGINT, signalHandler);

            childProcessTask(namedPipeMainToChildFD, namedPipeChildToMainFD);

            // close write and read pipes which were in use
            close(namedPipeMainToChildFD);
            close(namedPipeChildToMainFD);
        }
        unlink(namedPipeMainToChild.c_str());
        unlink(namedPipeChildToMain.c_str());
    }
    catch (exception& e) {
        cout << e.what() << endl;
        return 1;
    }
}

