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
- No file persistence yet: tickets are stored only in memory.
- Coach type and status enums are printed as integers instead of names.
- Refund penalty calculation is not implemented.
- Client–Cashier interaction is simplified: methods only print actions (cout).
- Error handling and input validation are minimal.
These limitations will be addressed in upcoming releases.

## Build & Run
Make sure you have `g++` (C++17 or newer) installed.  

To build and run:
```bash
make run
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
src/               # source code
tests/             # test programs
docs/release-1/    # Release 1 materials (slides, report, updated SRS/SDS)
docs/release-2/
docs/release-3/
docs/release-4/
Makefile           # build commands
README.md          # project description, team members, build/run instructions
.gitignore         # ignore generated files, IDE configs, etc.

