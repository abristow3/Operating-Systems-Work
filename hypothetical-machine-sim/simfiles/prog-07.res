==================== cycle: 1
-------------------- fetch
CPU Registers
-------------
PC: 300
AC: 2
IR: 4940

Memory
------
300: 4940
301: 5941
302: 2941
940: 1
941: 2

-------------------- execute
CPU Registers
-------------
PC: 301
AC: 1
IR: 4940
    opcode : 4 (SUB)
    address: 940

Memory
------
300: 4940
301: 5941
302: 2941
940: 1
941: 2

==================== cycle: 2
-------------------- fetch
CPU Registers
-------------
PC: 301
AC: 1
IR: 5941

Memory
------
300: 4940
301: 5941
302: 2941
940: 1
941: 2

-------------------- execute
CPU Registers
-------------
PC: 302
AC: 3
IR: 5941
    opcode : 5 (ADD)
    address: 941

Memory
------
300: 4940
301: 5941
302: 2941
940: 1
941: 2

==================== cycle: 3
-------------------- fetch
CPU Registers
-------------
PC: 302
AC: 3
IR: 2941

Memory
------
300: 4940
301: 5941
302: 2941
940: 1
941: 2

-------------------- execute
CPU Registers
-------------
PC: 303
AC: 3
IR: 2941
    opcode : 2 (STORE)
    address: 941

Memory
------
300: 4940
301: 5941
302: 2941
940: 1
941: 3

==================== cycle: 4
-------------------- fetch
CPU Registers
-------------
PC: 303
AC: 3
IR: 0

Memory
------
300: 4940
301: 5941
302: 2941
940: 1
941: 3

-------------------- execute
CPU Registers
-------------
PC: 303
AC: 3
IR: 0

Memory
------
300: 4940
301: 5941
302: 2941
940: 1
941: 3
