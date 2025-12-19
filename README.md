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
- Single-user CLI application.
These limitations were out of scope of the development releases.

## Build & Run
### Requirements

- C++17-compatible compiler (g++, MSVC, or MinGW)
- One of the following build systems:
    - Make
    - CMake

Read *Populate the Repositories* section before the first startup of the application.

To build and run with make:
```bash
# from project root
make
# generate sample data
make generate
# run the application
make run
```

To build and run with CMake:
```powershell
# from project root
mkdir build
cd build
cmake ..
cmake --build . --config Release
cd Release
# generate sample data
# use the following if you want to set a specific date from which the sample data will be generated:
# (replace YYYY with year, MM with month, DD with day)
# generator.exe YYYY-MM-DD
generator.exe
RailwayApp.exe
```

### Populate the Repositories
A separate executable (`generator`) is provided to generate tickets.csv and refund_policy.csv.

The generator supports an explicit reference date (YYYY-MM-DD) to ensure deterministic testing of refund tiers.

### Folders for documents
├───docs
│ ├───release-1 - Release 1 materials (slides, report, updated SRS/SDS)
│ └───release-2 - Release 2 materials (updated SRS/SDS, DLD)
│ └───release-3 - Release 3 materials (DLD R3)
│ └───release-3 - Release 3 materials (DLD R4, completion report)
├───include - public headers (part of Modular Design principle)
├───src - implementation files (part of Modular Design principle)
├───samples
│ └───generate_samples.cpp - sample data generator code
└───tests - test programs
Makefile - build commands
CMakeList.txt - build commands
README.md - project description, team members, build/run instructions
.gitignore - ignore generated files, IDE configs, etc.
