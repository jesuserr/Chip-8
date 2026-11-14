# Chip-8

This repository is a small CHIP-8 emulator built incrementally as a learning
project. The notes below are a development diary: each entry records what was
added in a commit and what the project was trying to achieve at that point.
This README is kept in the `resources/` directory alongside the implementation
notes and ROM references.

## Development Diary

### 2026-10-19 - `e9888d6` - First commit

The project began with a README and a name: Chip-8. Before writing the
emulator, I wanted a place to record the decisions and milestones that would
shape the implementation.

### 2026-10-25 - `c604636` - Create and init struct that models CHIP-8 machine

The first real implementation work was to describe the CHIP-8 machine in C.
The project scaffold, Makefiles, and `libft` dependency were added, along with
the machine state:

- 4 KB of RAM
- sixteen 8-bit registers (`V0` through `VF`)
- the index register and program counter
- a 16-level stack
- delay and sound timers
- a 64x32 monochrome display
- a 16-key keypad
- the built-in font data

At this stage, the emulator had a model of the machine but no ROM execution.

### 2026-10-30 - `c963736` - Parse and load ROMs in memory

The emulator learned to accept a ROM path from the command line. ROM bytes are
loaded into memory beginning at `0x200`, the standard CHIP-8 program address,
and the ROM size is tracked for later execution. The IBM Logo ROM became the
first test program, with an empty ROM added to check input validation.

### 2026-10-30 - `7da52d0` - New `data` structure for CHIP-8 state

ROM metadata and execution-related values needed a home alongside the machine
itself. A `t_data` wrapper was introduced to hold the CHIP-8 state, ROM size,
the current opcode, and the decoded opcode fields that would be needed by the
interpreter.

This commit also briefly included a compiled binary in the repository. That
was corrected in the next step.

### 2026-10-30 - `f4f2241` - Split ROM loading into `parser.c`

ROM input was moved out of `main.c` and into `parser.c`, giving the project a
clearer division of responsibilities. ROM size validation was added using
`MAX_ROM_SIZE`, and the size handling was corrected to use `size_t`. The
accidentally committed binary was removed.

The structure was now becoming clearer:

- `main.c`: program loop and opcode flow
- `parser.c`: command-line validation and ROM loading
- `chip8.c` / `chip8.h`: machine state and instruction behavior

### 2026-11-07 - `eb68e59` - Add implementation notes and ROM references

Before expanding the interpreter, the repository gained a set of working
notes. `resources/CHIP8_IMPLEMENTATION_STEPS.md` describes the planned order:
machine state, ROM loading, opcode execution, drawing, timing, SDL input and
output, emulator quirks, and testing. The IBM Logo assembly listing and a
short opcode checklist were added as references for the first milestone.

The IBM Logo program exercises these instructions:

`00E0`, `1NNN`, `6XNN`, `7XNN`, `ANNN`, and `DXYN`.

### 2026-11-07 - `d3995b7` - Start the fetch-decode-execute cycle

The emulator began executing instructions instead of only loading them. CHIP-8
initialization was moved into `init_chip8`, which clears the machine, sets the
program counter to `0x200`, and copies the font set to `0x50`.

The main loop now fetches two bytes, combines them into a 16-bit opcode,
decodes the `X`, `Y`, `N`, `NN`, and `NNN` fields, then dispatches by opcode
family. The first implemented handlers cover the IBM Logo milestone, while
debug output makes the program counter, index register, registers, and decoded
opcode visible during development.

### 2026-11-13 - `5f80728` - Implement the minimum opcode set for IBM Logo

The first complete ROM milestone was reached. The interpreter now implements
the minimum instruction set needed by `roms/IBM_Logo.ch8`: `00E0`, `1NNN`,
`6XNN`, `7XNN`, `ANNN`, and `DXYN`.

The drawing instruction now reads sprite bytes from memory, XORs them onto the
64x32 display, clips pixels at the screen edges, and sets `VF` when a lit pixel
is erased. The display dimensions were also named as constants, and a second
copy of the IBM Logo ROM was added for testing.

The emulator can now execute the IBM Logo instruction path in the terminal.
It is still a development build: there is no graphical display, keyboard
input, timer loop, or sound yet.

### 2026-11-13 - `08e8d94` - Add basic display output and optimize `DXYN`

The display became visible in the terminal. A new `display.c` module renders
the 64x32 display buffer using `X` for an active pixel and a space for an
inactive pixel. The main loop now prints this display after each instruction,
making the IBM Logo milestone observable without SDL.

The `DXYN` implementation was also tightened: it only toggles pixels when the
sprite bit is set, records collisions when an active pixel is erased, and no
longer emits a raw hex dump after every draw. The build now includes
`display.c`, and the loop delay was adjusted so the terminal output can be
followed while the ROM runs.

## Build and Run

Build the emulator from the repository root:

```sh
make
```

Run it from the repository root with a CHIP-8 ROM:

```sh
./chip8 roms/IBM_Logo.ch8
```

The current build prints decoded instructions, machine state, and a text
rendering of the display to the terminal. It does not yet open an SDL window.

## Next Entries

The next development steps are to add the remaining opcode families, add the
60 Hz timer and input model, and then connect the emulator to SDL for a window,
keyboard, and sound. Each step will be added to this diary as its own commit.
