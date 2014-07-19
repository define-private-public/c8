c8 -- A CHIP-8 Emulator
=======================

This was another attempt at making CHIP-8 Emulator/Interpreter.  This one
turned out much better and is more modular.  While it does run more ROMs it
doesn't run them all; there are still a few bugs.


Requirements:
 * `g++`
 * `make`
 * `SDL2`
 * `SDL2_mixer`


Building:
---------
 1. Make sure you have all of the requirements met.  From the home directory,
 2. `cd sdl/`
 3. `make`
 4. You should be good to go then.

In the file `./sdl/main.cpp` there are two variables in the `main()` function
that you can change to alter how the emulator will work.
 * `cpu.stepModeOn` -- Turn on step by step execution, default is `false`
 * `cpu.debugInfo` -- Extra debugging information, default is `false`


Usage:
------
    $ ./c8_sdl <rom_file>

 * `<rom_file>` -- Location of the CHIP-8 ROM to run

The key mapping is as follows:

        COSMAC:           Keyboard:
  
     1 | 2 | 3 | C      1 | 2 | 3 | 4
    ---------------    ---------------
     4 | 5 | 6 | D      Q | W | E | R
    ---------------    ---------------
     7 | 8 | 9 | E      A | S | D | F
    ---------------    ---------------
     A | 0 | B | F      Z | X | C | V

This emulator also supports save states.  Save states are written to your
current working directory, with the extension `.c8s`.  The program will
overwrite an existing save state if it shares the save filename.  To use the
save state feature.

The save state counter controls which save state "slot," you are currently
using.  There are a maximum of 1000 save states, though I doubt you'll use all
of them.

The controls are as follows:
 * `p` -- Write a new save state
 * `o` -- Load most recent save state
 * [Minus key] -- Decrement the save state counter
 * [Equals key] -- Increment the save state counter


If you have turned on step by step execution in the source, press the `m` key to
go to the next instruction.


Directories:
-----------
 * `./sdl/` -- Main source code for the emualtor.  Was going to make a Qt
               version as well, but I scrapped that idea.  You'll notice that
               most of the source code (except for input and sound) is
               independant of which toolkit you're using.
 * `./utils/` -- Utility programs for me.  E.g. a hexcode ROM dumper.


Misc.
-----
If you encounter any bugs, issues, or have have a feature request, put it in the
GitHub tracker please.
 
