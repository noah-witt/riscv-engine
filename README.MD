The engine for the risc-v runtime.

Will likely integrate in to an electron JS app. and will run in the WASM environment.

TODO
    - [X] Memory System
    - [ ] ALU
    - [ ] Assembler
    - [ ] syscall management


TODO: setup github CI.
TODO: assembler take source and make a memory map from it 

run the commands from memory
Unit tests should be used liberally to mitigate bugs at time to stich it all up.


the basic structure of this software should load a RISC-V assembly string, compile it, load it in to memory and start executing. Unit tests should be used to liberally ensure that code is well tested. A user interface will need to be provided to allow users to interact with the software in a constructive way.