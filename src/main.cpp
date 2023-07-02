#include "child_process.h"
#include "programs.h"

int main() {
    // Define the pipes used
    static int pipeMainToChildFD[2];
    static int pipeChildToMainFD[2];

    pid_t currPID, childPID;

    try {
        unzipDB();
        createNewPipe(pipeMainToChildFD);
        createNewPipe(pipeChildToMainFD);
        currPID = createNewProcess();

        // Get the child PID
        childPID = sendChildPIDtoParent(pipeChildToMainFD, currPID);

        if (currPID != CHILD_PROCESS_ID) {
            signal(SIGINT, signalHandlerMainProcess);
            setpgid(0, childPID);
            parentProcessTask(pipeMainToChildFD, pipeChildToMainFD, childPID);

            // close write and read pipes which were in use
            close(pipeChildToMainFD[READ_END]);
            close(pipeMainToChildFD[WRITE_END]);
        } else {
            // Allow child process' signal handler to handle SIGUSR1 and SIGUSR2
            signal(SIGUSR1, signalHandler);
            signal(SIGINT, signalHandler);

            childProcessTask(pipeMainToChildFD, pipeChildToMainFD);

            // close write and read pipes which were in use
            close(pipeMainToChildFD[READ_END]);
            close(pipeChildToMainFD[WRITE_END]);
        }
    }
    catch (exception& e) {
        cout << e.what() << endl;
        return 1;
    }
}

