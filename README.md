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
- No durable persistence (CSV/atomic writes).
- No automated logging files.
- No full TrainRepository schedules.
- Error handling and input validation are minimal.
These limitations will be addressed in upcoming releases.

## Build & Run
Make sure you have `g++` (C++17 or newer) installed. If using Windows CMake, make sure your have `make`, `cmake` and a C++ toolchain: Visual Studio (MSVC) or MinGW-w64/MSYS2 (GCC).

To build and run:
```bash
make run
```
```powershell
# from project root
mkdir build
Set-Location build
cmake ..
cmake --build . --config App
# run the executable (adjust path/name as needed)
.\App\app.exe
```

To run basic tests:
```bash
make test
```

To clean up build artifacts"
```bash
make clean
```

### Folders for documents
src/               # implementation files (part of Modular Design principle)
include/           # public headers (part of Modular Design principle)
tests/             # test programs
docs/release-1/    # Release 1 materials (slides, report, updated SRS/SDS)
docs/release-2/    # Release 2 materials (updated SRS/SDS, DLD)
docs/release-3/
docs/release-4/
Makefile           # build commands
README.md          # project description, team members, build/run instructions
.gitignore         # ignore generated files, IDE configs, etc.

