#include "parent_process.h"

// This is the Parent process part of the task.
// 1) Get opcode input from user and write it to pipe
// 2) Once written, wait until child process writes back the result
// 3) prints said result
void parentProcessTask(int *pipeMainToChildFD,  int *pipeChildToMainFD, int childPid) {
    int opCode;

    // Close pipe ends that are not in use
    close(pipeMainToChildFD[READ_END]);
    close(pipeChildToMainFD[WRITE_END]);

    do {
        clearScreen();
        printMenu();
        try {
            opCode = getUserOpcodeInput();

            // In case user choose options 6-7
            if(opCode > OPTION_NUM_FIVE)
            {
                handleNonChildTasks(opCode, childPid, pipeChildToMainFD);
            }
            // In case user choose options 1-5
            else {
                writeParametersToChildProcess(pipeMainToChildFD, opCode);

                // Read the result from the child process and prints it
                string result = readStringFromChild(pipeChildToMainFD);
            }
            if(opCode == 5 || opCode == 6) // Clear buffer after the use of some of the functions
                bufferFlush(); // needed to clear the buffer in order to make the getchar() to wait for the user's input
        }
        catch (exception &e) {
            cout << e.what() << endl;
            bufferFlush();
        }
        waitForUserInputToContinue();
    } while (static_cast<MenuOptions>(opCode) != MenuOptions::GracefulExit);
}

void writeParametersToChildProcess(int *pipeMainToChildFD, int opCode)
{
    write(pipeMainToChildFD[WRITE_END], &opCode, sizeof(opCode)); // Send opCode to the child process.

    // In case of options 1-3, need to send input params.
    if(opCode <= 3 && opCode >= 1) {
        getParamsFromUserAndSendToChild(pipeMainToChildFD, opCode);
    }
}

void getParamsFromUserAndSendToChild(int *pipeMainToChildFD, int opCode)
{
    string userInput = getStringsListFromUser(opCode);
    write(pipeMainToChildFD[WRITE_END],userInput.c_str(), userInput.length());
}

// User inputs multiple string in the line and the func combine them together with a delimiter and return the string.
string getStringsListFromUser(int opCode) {
    string s;
    bufferFlush();

    printMessageByOpCode(opCode);
    getline(cin, s);

    return s;
}

void printMessageByOpCode(int opCode)
{
    MenuOptions userInput = static_cast<MenuOptions>(opCode); // Cast opCode to enum

    switch (userInput) {
        case MenuOptions::FetchAirportsIncomingFlights:
            cout << "Please enter airport ICOA names: " << flush;
            break;
        case MenuOptions::FetchAirportsFullFlightSchedule:
            cout << "Please enter airport ICOA name: " << flush;
            break;
        case MenuOptions::FetchAircraftsFullFlightSchedule:
            cout << "Please enter all required aircraft Icoa24: " << flush;
            break;
    }
}

// Handle tasks in main process when the user choose options 6-7.
void handleNonChildTasks(int opCode, int childPid, int* pipeChildToMainFD)
{
    MenuOptions userInput = static_cast<MenuOptions>(opCode); // Cast opCode to enum

    switch (userInput) {
        case MenuOptions::GetChildProcessID:
            cout << "Child PID: "<< childPid<< endl;
            break;
        case MenuOptions::GracefulExit:
            cout << "Closing the program, Goodbye." << endl << endl;
            gracefulExit(childPid, pipeChildToMainFD);
        default:
            cout << "Input not listed on menu" << endl;
            break;
    }
}


// Send SIGUSR1 signal to child process to terminate it, then terminates the main process
void gracefulExit(int childPid, int* pipeChildToMainFD) {
    // Wait for child process to finish
    int res = kill(childPid, SIGUSR1);

    if (res == 0) {
        cout << "SIGUSR1 signal sent to child process." << endl;
    } else {
        cout << "Failed to send SIGUSR1 signal." << endl;
    }

    // Wait for the child process to exit, obtain the exit status
    int status;
    waitpid(childPid, &status, 0);

    // Read the result from the child process and prints it
    string result = readStringFromChild(pipeChildToMainFD);
    cout << result << endl;

    // Print the status
    if (WIFEXITED(status)) {
        int exitStatus = WEXITSTATUS(status);
        cout << "Child process exited with status: " << exitStatus << endl;
    } else if (WIFSIGNALED(status)) {
        int termSignal = WTERMSIG(status);
        cout << "Child process terminated by signal: " << termSignal << endl;
    }

    // Terminate the main process, finished.
    exit(0);
}


void signalHandlerMainProcess(int signal) {
    if(signal == SIGINT) {
        cout << endl << "Main process received SIGINT signal. Sending SIGINT to child..." << endl << endl;

        zipDatabaseFiles();

        // Terminate the program
        exit(signal);
    }
}


