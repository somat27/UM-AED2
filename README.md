# SGAU — Patient Management and Attendance System

Console application in C for managing patients and doctors, developed for AED2 (University of Minho). The system lets you register and consult patients and doctors, manage per‑doctor waitlists, and navigate interactive text menus with the keyboard.

## Features

- Patient management
  - Create, edit, view, remove, and list patients
  - Show whether a patient is on a waitlist
- Doctor management
  - Create, edit, view, remove, and list doctors
  - List all patients for a doctor
- Waitlists (per doctor)
  - Add a patient to their doctor’s waitlist
  - Remove the next patient from a doctor’s waitlist
  - View a doctor’s waitlist and the total waiting
  - Identify the doctor with the largest waitlist
- Text‑file persistence
  - `patients.txt`: name, code, doctor_code
  - `doctors.txt`: name, code
- Inconsistency checks
  - On startup, patients with an invalid doctor are flagged for reassignment

## Prerequisites

- Windows (tested with Dev‑C++/MinGW; uses `conio.h` and `system("cls")`)
- C compiler with C99 support (e.g., MinGW‑w64)
- Make (optional, to use `Makefile.win`)

## Build

Choose one approach:

1) Dev‑C++ (5.11)
- Open `SGAU.dev` and build.

2) Command line with MinGW/Make
- From the project root:
  - `mingw32-make -f Makefile.win`
  - or `make -f Makefile.win`

3) Command line with GCC
- From the project root:
  - `gcc -std=c99 main.c database.c queue.c menus.c doctors.c patients.c -o SGAU.exe`

The expected executable is `SGAU.exe` at the project root.

## Getting the executable (.exe)

This repository does not include binaries. To get `SGAU.exe`, build locally using one of the methods in the “Build” section:
- GCC: `gcc -std=c99 main.c database.c queue.c menus.c doctors.c patients.c -o SGAU.exe`
- Makefile.win (MinGW/Make): `mingw32-make -f Makefile.win` or `make -f Makefile.win`
- Dev‑C++: open `SGAU.dev` and build

## Run

- From the executable: `./SGAU.exe`
- On first run, `patients.txt` and `doctors.txt` are created automatically (if they don’t exist).

## Usage

- Navigate menus with arrow keys and Enter.
- Main menus:
  - Patient Attendance: manage waitlists (add/remove/list)
  - Patients Menu: create, edit, view, remove, list
  - Doctors Menu: create, edit, view, remove, list
- Validation notes:
  - Names cannot contain digits
  - You cannot edit/remove a patient if they are in the waitlist
  - You cannot edit/remove a doctor if they have patients in the waitlist

## Data structures (summary)

- `Patient` (see `structures.h`): `name`, `code`, `doctor_code`, `next`
- `Doctor` (see `structures.h`): `name`, `code`, `waitlist` (linked list of `Patient`), `next`

## Project layout

- C sources: `main.c`, `menus.c`, `doctors.c`, `patients.c`, `queue.c`, `database.c`
- Headers: `structures.h`, `menus.h`, `doctors.h`, `patients.h`, `queue.h`, `database.h`
- Persistence: `patients.txt`, `doctors.txt` (created and managed by the app)
- Build (Windows): `Makefile.win`, `SGAU.dev`, `SGAU_private.*`, `SGAU.ico`
- Build artifacts (generated): `SGAU.exe`, `*.o`, `*.res`, `gmon.out`, `report.txt`

## Data file formats

- `doctors.txt`
  - One line per doctor: `DOCTOR_NAME,DOCTOR_CODE`
  - Example: `Jose Miguel,110173`
- `patients.txt`
  - One line per patient: `PATIENT_NAME,PATIENT_CODE,DOCTOR_CODE`
  - Example: `Fernando,110258,110186`

To “reset” data, delete `patients.txt` and/or `doctors.txt` while the app is closed. They are recreated on startup.

## Credits

Academic project for AED2 — University of Minho.