# Railway Ticket Sales System

## Team Members
- Diana Anastassija Turks (@molchanka)

## Project Description
This project is an implementation of a **Railway Ticket Sales System** written in C++.
### Description from Offered Project Topics File
Railway tickets are sold as follows: a client requests availability for a specific date and destination, optionally indicating the type of coach (sleeper, compartment, etc.). The cashier searches the database and offers available options, which are temporarily blocked until the purchase is completed. The client provides passport data, and the cashier issues the ticket and accepts payment.

Data entered by the cashier are stored in a central database to prevent double sales. Ticket returns are possible: the client receives a refund minus a penalty, and the ticket is marked as available again. The penalty depends on the time before departure:
- 1% if returned one month before,
- 5% if returned 15 days before,
- 10% if returned 3 days before,
- 30% if returned on the day of travel.

At the end of the day, the cashier submits a report on sold and returned tickets to the
central office.

## Current Limitations
- No automated logging files.
These limitations will be addressed in upcoming releases.

## Build & Run
Make sure you have `g++` (C++17 or newer) installed. If using Windows CMake, make sure your have `make`, `cmake` and a C++ toolchain: Visual Studio (MSVC) or MinGW-w64/MSYS2 (GCC). Read *Populate the Repositories* section before the first startup of the application.

To build and run:
```bash
make run
```
```powershell
# from project root
mkdir build
cd build
cmake ..
cmake --build . --config Release
.\Release\RailwayApp.exe
```

### Populate the Repositories
If you want to simulate real operation, take .csv files from the `samples` directory and drop them into the same directory as the built .exe file. The application will read the necessary info from them - the refund policy and the ticket list.

### Folders for documents
├───docs
│ ├───release-1 - Release 1 materials (slides, report, updated SRS/SDS)
│ └───release-2 - Release 2 materials (updated SRS/SDS, DLD)
│ └───release-3 - Release 3 materials (DLD R3)
├───include - public headers (part of Modular Design principle)
├───src - implementation files (part of Modular Design principle)
├───samples - sample .csv files that the program can use
└───tests - test programs
Makefile - build commands
CMakeList.txt - build commands
README.md - project description, team members, build/run instructions
.gitignore - ignore generated files, IDE configs, etc.
