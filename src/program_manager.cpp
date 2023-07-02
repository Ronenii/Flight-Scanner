#include "program_manager.h"


// Creates new pipe, throws exception if creation failed
void createNewPipe(int* pipeFd) {
    if (pipe(pipeFd) == ERROR_CREATING)
        throw system_error(errno, generic_category(), "Error creating pipe");
}

// Creates new process, throws exception if creation failed
pid_t createNewProcess() {
    pid_t pid;

    if ((pid = fork()) == ERROR_CREATING)
        throw system_error(errno, generic_category(), "Error creating new process");

    return pid;
}


void printMenu() {
    cout << endl << "**************************" << endl <<
         "1. Fetch Airports Incoming Flights" << endl <<
         "2. Fetch Airports Full Flights Schedule" << endl <<
         "3. Fetch Aircrafts Full Flights Schedule" << endl <<
         "4. Update Database" << endl <<
         "5. Zip Database Files" << endl <<
         "6. Get Child PID" << endl <<
         "7. Exit" << endl <<
         "Please make your choice: " << flush;
}


// ANSI escape sequence to clear the screen
void clearScreen() {
    cout << "\033[2J\033[1;1H" << flush;
}


// Waits for any key to be entered to continue with the program
void waitForUserInputToContinue() {
    cout << "Press any key to continue..." << flush;
    getchar();
}


// Read the size of the string received, read the message
string readStringFromChild(int* pipefd)
{
    int stringSize;
    string output;

    read(pipefd[READ_END],&stringSize, sizeof(stringSize));
    char buffer[stringSize];
    read(pipefd[READ_END],buffer,sizeof(buffer));

    cout<< buffer<< endl;

    return output;
}

// Gets user opcode input, throws exception if input invalid
int getUserOpcodeInput() {

    string input;
    cin >> input;

    try {
        return stoi(input);
    }
    catch (const exception &e) {
        throw std::runtime_error("Invalid input: Must be numbers");
    }
}

// If we are inside the child process, then write the childPID to parent pipe.
// otherwise if we are in the parent process then read the child PID.
// In either case return it.
pid_t sendChildPIDtoParent(int* pipeFd, pid_t pid) {
    pid_t childPid;
    if(pid == CHILD_PROCESS_ID) {
        childPid = getpid();
        write(pipeFd[WRITE_END], &childPid, sizeof (pid_t));
    }
    else {
        read(pipeFd[READ_END], &childPid, sizeof(pid_t));
    }
    return childPid;
}


// Discard remaining characters in the input buffer
void bufferFlush() {
    int c;
    while (((c = getchar()) != '\n') && (c != EOF)) {}
}

// Read the received string from parent. This function doesn't use while loop in order to read
// the string from the main process.
string readStringFromParent(int* pipefd)
{
    ssize_t bytesRead;
    char buffer[4096];
    bytesRead = read(pipefd[READ_END], buffer, sizeof(buffer));
    if (bytesRead > 0) {
        string receivedMessage(buffer, bytesRead);
        return receivedMessage;
    }
    else {
        throw runtime_error("Error reading from pipe");
    }
}

void unzipDB()
{
    if(std::filesystem::exists("./DB.zip"))
    {
        int status = std::system("unzip -o ./DB.zip -d .");

        if(status != 0)
        {
            throw runtime_error("unzip failed with exit status: " + to_string(status));
        }
    }
}
