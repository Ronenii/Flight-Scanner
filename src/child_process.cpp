#include "child_process.h"


// This is the child process part of the task.
// 1) Wait until opcode is read from the pipe
// 2) Once opcode is read successfully execute its task
// 3) Writes to pipe the result of the process
void childProcessTask(int namedPipeMainToChildFD, int namedPipeChildToMainFD) {
    int opCode, size;
    string res;
    vector<string> inputParams;

    // Will run in this loop, until the main process sends a signal to stop
    while(true) {
        inputParams = getOpCodeAndInputParamsFromPipe(namedPipeMainToChildFD, namedPipeChildToMainFD, opCode);
        MenuOptions userInput = static_cast<MenuOptions>(opCode); // Cast opCode to enum

        switch (userInput) {
            case MenuOptions::FetchAirportsIncomingFlights:
                res = fetchAirportsIncomingFlights(inputParams);
                break;
            case MenuOptions::FetchAirportsFullFlightSchedule:
                res = fetchAirportsFullFlightSchedule(inputParams);
                break;
            case MenuOptions::FetchAircraftsFullFlightSchedule:
                res = fetchAircraftsFullFlightSchedule(inputParams);
                break;
            case MenuOptions::UpdateDatabase:
                res = updateDatabase();
                break;
            case MenuOptions::ZipDatabaseFiles:
                res = zipDatabaseFiles();
                break;
            default:
                cout << "Invalid operation code." << endl;
                break;
        }

        size = res.length()+1;
        write(namedPipeChildToMainFD, &size, sizeof(size));
        write(namedPipeChildToMainFD, res.c_str(), size);
    }
}


// Waits until receiving an opcode, returns it if valid, otherwise throw exception
vector<string> getOpCodeAndInputParamsFromPipe(int namedPipeMainToChildFD, int namedPipeChildToMainFD, int &opCode) {
    vector<string> inputParams;

    // Read op-code from pipe
    if (read(namedPipeMainToChildFD, &opCode, sizeof(opCode)) != sizeof(opCode)) {
        throw runtime_error("Error reading opCode");
    }

    // If the user choose options 1-3, input parameters will be sent.
    if (opCode <= 3 && opCode >= 1) {
        inputParams = getInputParametersFromMain(namedPipeMainToChildFD, namedPipeChildToMainFD);
    }

    return inputParams;
}


// When user choose options 1-3, child process will recive input parameters using this function.
vector<string> getInputParametersFromMain(int namedPipeMainToChildFD, int namedPipeChildToMainFD) {
    vector<string> inputParameters;

    // Reading the input pramas and store the data in a string vector.
    inputParameters = convertInputParamsToVector(namedPipeMainToChildFD);

    return inputParameters;
}


// Reciving a string from main process with delimiters and put each string in a string vector.
vector<string> convertInputParamsToVector(int namedPipeMainToChildFD) {
    vector<string> res;
    string inputParams;

    inputParams = readStringFromParent(namedPipeMainToChildFD);

    char *cInputParams = new char[inputParams.length() + 1]; // convert string to char* in order to use strtok.
    strcpy(cInputParams, inputParams.c_str());
    const char *delimiter = " ";
    char *token = strtok(cInputParams, delimiter);

    while (token != nullptr) {
        res.push_back(token); // put the token in the res vector.
        token = strtok(nullptr, delimiter);
    }

    delete[] cInputParams;
    return res;
}


// Handler function for signals received from the main process.
void signalHandler(int signal) {
    switch(signal) {
        case SIGUSR1:   // Graceful exit
            cout << "Child process received SIGUSR1 signal. Starting graceful exit..." << endl << endl;

            // zip the current database files
            zipDatabaseFiles();

            // Terminate the child process
            exit(signal);

        case SIGINT:    // When user sends CTRL + C
            cout << "Child process received SIGINT signal. Zipping files and exiting..." << endl << endl;

            // zip the current database files
            zipDatabaseFiles();

            // Terminate the child process
            exit(signal);
    }
}


