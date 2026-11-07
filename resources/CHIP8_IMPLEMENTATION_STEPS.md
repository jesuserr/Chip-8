# CHIP-8 emulator in C: implementation steps

## 1\. Project setup

- Use C99 or C11 and SDL2 for the window, input and audio.  
- Suggested layout: `main.c` (main loop), `chip8.c/.h` (CPU core), `platform.c/.h` (SDL code).  
- Build with `gcc -Wall -Wextra -O2 *.c $(sdl2-config --cflags --libs)`.

## 2\. Define the machine state

```c
typedef struct {
    uint8_t  memory[4096];
    uint8_t  V[16];            // V0–VF, VF = flag register
    uint16_t I;                // index register
    uint16_t pc;               // starts at 0x200
    uint16_t stack[16];
    uint8_t  sp;
    uint8_t  delay_timer;
    uint8_t  sound_timer;
    uint8_t  display[64 * 32]; // 1 = pixel on
    uint8_t  keys[16];
    bool     draw_flag;
} Chip8;
```

## 3\. Initialization

- Zero the state and set `pc = 0x200`.  
- Load the built-in font (16 characters × 5 bytes, 0–F) into memory, usually at `0x050`.  
- Seed the RNG (used by `CXNN`).

## 4\. ROM loading

- `fread` the ROM into `memory[0x200]`.  
- Reject files larger than `4096 - 0x200` bytes.

## 5\. Fetch, decode, execute

- **Fetch:** `opcode = memory[pc] << 8 | memory[pc+1]; pc += 2;` (instructions are big-endian)  
- **Decode:** pull out the fields:  
  - `X = (op >> 8) & 0xF`  
  - `Y = (op >> 4) & 0xF`  
  - `N = op & 0xF`  
  - `NN = op & 0xFF`  
  - `NNN = op & 0xFFF`  
- **Execute:** `switch (op >> 12)`, then nested switches for the `0`, `8`, `E` and `F` groups.

## 6\. Implement the 35 opcodes, in this order

1. `00E0` (clear screen), `1NNN` (jump), `6XNN` (set VX), `7XNN` (add to VX), `ANNN` (set I), `DXYN` (draw). These are enough to run the **IBM logo** ROM, your first milestone.  
2. Flow control: `2NNN`, `00EE`, `3XNN`, `4XNN`, `5XY0`, `9XY0`, `BNNN`.  
3. ALU group: `8XY0` to `8XYE`. Set VF for carry, borrow and shifted-out bits, and write VF **after** the result.  
4. `CXNN` (random), `EX9E`/`EXA1` (key skips), and the `FX__` group: timers, `FX0A` (wait for key), `FX1E`, `FX29` (font address), `FX33` (BCD), `FX55`/`FX65` (store/load registers).

## 7\. Drawing (`DXYN`)

- Read N bytes of sprite data starting at I.  
- XOR each bit onto the display at (VX, VY).  
- Wrap the starting coordinate (`% 64`, `% 32`) but clip pixels that run past the edge.  
- Set VF \= 1 if any lit pixel was turned off (collision), otherwise 0\.

## 8\. Main loop timing

- Run about 500–700 instructions per second, for example \~10 per frame at 60 FPS.  
- Decrement both timers at **60 Hz**, separately from the CPU rate.  
- Play a beep while `sound_timer > 0`.  
- Redraw only when `draw_flag` is set.

## 9\. Platform layer (SDL2)

- Draw the 64×32 display scaled up (e.g. ×10) into an SDL texture.  
- Map the hex keypad to the keyboard:

```
1 2 3 C      1 2 3 4
4 5 6 D  →   Q W E R
7 8 9 E      A S D F
A 0 B F      Z X C V
```

- Generate the beep with a square wave from an SDL audio callback.

## 10\. Quirks

CHIP-8 variants disagree on a few behaviors. Make these configurable:

- `8XY6`/`8XYE`: shift VY or VX.  
- `FX55`/`FX65`: increment I or leave it unchanged.  
- `BNNN`: jump to NNN \+ V0, or the SUPER-CHIP behavior (NNN \+ VX).  
- `8XY1/2/3`: reset VF or not.  
- `DXYN`: clip or wrap sprites at the screen edge.

## 11\. Testing

- Run the test ROMs in order: IBM Logo, then the **Timendus chip8-test-suite** (corax+ opcodes, flags, quirks, keypad).  
- Then try games: Pong, Tetris, Space Invaders, Breakout.  
- Add a debug mode that prints PC, opcode and registers for each step.

## 12\. Optional extras

Pause/step debugger, savestates, SUPER-CHIP/XO-CHIP support, adjustable speed, configurable colors.

## References

- Tobias V. Langhoff, "Guide to making a CHIP-8 emulator"  
- Cowgod's CHIP-8 Technical Reference