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

### 2026-11-14 - `e1803ea` - Integrate SDL graphics and input events

The emulator moved from terminal output to a graphical display. The new
`graphics.c` module initializes SDL2 and SDL2_gfx, creates a scaled borderless
window, and renders the 64x32 CHIP-8 display with a visible pixel grid.

The fetch-decode-execute logic was moved into `cycle.c`, leaving `main.c` to
coordinate initialization, input, execution, rendering, and cleanup. SDL
events are now processed every loop: `Escape` quits the emulator, while
`Space` or `P` pauses and resumes execution. The window also shows a small
status overlay for the emulator and pause state.

### 2026-11-15 - `dfe4d5b` - Replace opcode switch with a function-pointer table

Opcode dispatch was simplified by replacing the `switch` statement in
`cycle.c` with a 16-entry function-pointer table indexed by the opcode's high
nibble. Implemented opcode families point directly to their handlers, while
unimplemented families use `opcode_tbd` and report that the instruction is not
implemented yet. This makes adding new opcode families more localized.

The repository also gained a small ROM test set: the CHIP-8 logo, IBM logo,
Corax+ opcode tests, and flags tests. These ROMs provide the next checkpoints
for expanding the interpreter beyond the initial IBM Logo instruction set.

### 2026-11-16 - `4d77486` - Implement call, return, and conditional skips

The interpreter gained its first flow-control group beyond plain jumps. It now
supports `00EE` return, `2NNN` subroutine calls, `3XNN` and `4XNN` immediate
skips, and `5XY0` and `9XY0` register comparison skips.

Subroutine calls save the next program counter on the 16-level stack, and
returns restore it after decrementing the stack pointer. Stack underflow and
overflow are detected and stop execution with a diagnostic instead of
accessing invalid stack entries. The new handlers are also registered in the
function-pointer dispatch table.

### 2026-11-19 - `d1566f1` - Implement the `8XYN` arithmetic family

The interpreter gained the register-to-register arithmetic and logic
instructions `8XY0` through `8XY7` and `8XYE`: load, OR, AND, XOR, add,
subtract, shift right, reverse subtract, and shift left.

These handlers update `VF` according to CHIP-8 behavior. Addition records
carry, subtraction records whether no borrow occurred, and the shift
instructions preserve the bit shifted out. The logical operations clear `VF`.
The `8XYN` family is selected through a second function-pointer table indexed
by the opcode's lowest nibble, and its handlers were moved into
`opcodes_8xyn.c`.

### 2026-11-20 - `fe7f25a` - Implement the `FX__` opcodes except `FX0A`

The interpreter gained the supported `FX__` instructions: `FX07`, `FX15`,
`FX18`, `FX1E`, `FX29`, `FX33`, `FX55`, and `FX65`. These cover reading and
writing the delay and sound timers, adding `Vx` to `I`, locating font sprites,
converting `Vx` to packed decimal digits, and transferring registers between
memory and the emulator state.

The new handlers live in `opcodes_fxnn.c` and are selected through a dedicated
function-pointer table. Memory-bound checks were added for BCD and register
stores. `FX0A`, which waits for a key press, remains a TODO until keyboard
input is implemented. A quirks test ROM was also added for the next round of
validation.

### 2026-11-22 - `6259256` - Implement `BNNN`, `CXNN`, and fetch safety

The interpreter gained `BNNN` and `CXNN`. `BNNN` jumps to the 12-bit address
plus the value of `V0`, with a guard that stops execution if the resulting
address exceeds RAM. `CXNN` generates a random byte and masks it with the
instruction's immediate byte.

Random generation is now seeded once during CHIP-8 initialization. The fetch
stage also verifies that the program counter can read a complete two-byte
instruction before accessing memory, stopping the emulator with a diagnostic
if the address leaves valid RAM. The main dispatch table now routes `B` and
`C` opcode families to their handlers.

### 2026-11-22 - `da60ace` - Consolidate opcode handlers and refactor fetch

The central CHIP-8 opcode implementations and the nested `8XYN` and `FXNN`
dispatch tables were consolidated in `opcodes.c`. Initialization moved with
those handlers, leaving `main.c` focused on startup, the SDL event loop, and
cleanup.

Fetch now returns `true` after successfully reading an instruction and `false`
when the program counter is outside RAM. `instruction_cycle` uses that result
to stop before decode or execute, making invalid-fetch handling explicit instead
of part of the instruction execution flow.

### 2026-11-24 - `c7d9b3d` - Implement keypad input and key wait

The emulator now implements `EX9E`, `EXA1`, and `FX0A`. The skip instructions
check whether the keypad key in `Vx` is currently pressed or unpressed.
`FX0A` waits until a key is pressed and released, then stores that key value
in `Vx`.

SDL polls the physical keyboard each frame and updates the 16-entry CHIP-8
keypad array. The standard mapping uses `X/1/2/3/QWER/ASDZC/4RFV` for the
CHIP-8 keys `0-F`. `FX0A` uses a latched key state so that a press is not
recorded until the corresponding key is released.

A keypad test ROM was added at `roms/6-keypad.ch8` for validating keypad
skips, key waits, and keyboard mapping.

## Build and Run

Build the emulator from the repository root:

```sh
make
```

The build requires SDL2 and SDL2_gfx because the executable links against
`-lSDL2` and `-lSDL2_gfx`.

Run it from the repository root with a CHIP-8 ROM:

```sh
./chip8 roms/IBM_Logo.ch8
```

The current build opens an SDL window and renders the IBM Logo ROM. Press
`Escape` to quit, or `Space` / `P` to pause and resume. Debug information is
still printed to the terminal.

Additional test ROMs are available in `roms/`, for example:

```sh
./chip8 roms/3-corax+.ch8
./chip8 roms/6-keypad.ch8
```

## Next Entries

The next development steps are to complete the 60 Hz delay and sound timer
model, add sound output, and run broader validation against the flags, quirks,
and keypad ROMs. Remaining opcode edge cases and emulator quirks should be
resolved during testing. Each step will be added to this diary as its own
commit.
