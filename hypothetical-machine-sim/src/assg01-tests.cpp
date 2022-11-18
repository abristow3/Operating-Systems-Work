/** @file assg01-tests.cpp
 * @brief Unit tests for Hypothetical Machine simulation
 *
 * @author Derek Harter
 * @note   cwid: 123456
 * @date   Fall 2020
 * @note   ide:  VS Code Editor/IDE ; GNU gcc tools
 *
 * Unit tests for assignment 1, the hypothetical machine simulator.
 * You should start with the first set of tests in the first TEST_CASE,
 * and get the tests to pass in order from the first one in this file
 * to the last one.  If all unit tests pass, you will most likely have
 * a working system simulator for this assignment.
 */
#include "HypotheticalMachineSimulator.hpp"
#include "SimulatorException.hpp"
#include "catch.hpp"
#include <string>
using namespace std;

// change these to #define when ready to begin working on each task,
// once defined they will enable the tests declared below for each task
#define task1
#define task2
#define task3_1
#define task3_2
#define task4
#define task5
#define task6_1 // executeLoad()
#define task6_2 // executeStore()
#define task6_3 // executeAdd() and executeSub()
#define task6_4 // executeJump()
#define task7

/// simulator instance used for all of the tests
HypotheticalMachineSimulator sim;

/**
 * @brief Task 1: test memory initialization
 */
#ifdef task1
TEST_CASE("Task 1: <initializeMemory()> memory initialization simple values", "[task1]")
{
  // make sure memory set correctly, especially memory size determined correctly
  sim.initializeMemory(300, 999);
  CHECK(sim.getMemoryBaseAddress() == 300);
  CHECK(sim.getMemoryBoundsAddress() == 999);
  CHECK(sim.getMemorySize() == 700);
}

TEST_CASE("Task 1: <initializeMemory()> reset of simulation", "[task1]")
{
  // reset should be working to reset state of simulator
  sim.reset();
  CHECK(sim.getMemoryBaseAddress() == 0);
  CHECK(sim.getMemoryBoundsAddress() == 0);
  CHECK(sim.getMemorySize() == 0);
}

TEST_CASE("Task 1: <initializeMemory()> memory initialization more complex ", "[task1]")
{
  // another random test
  sim.initializeMemory(42, 917);
  CHECK(sim.getMemoryBaseAddress() == 42);
  CHECK(sim.getMemoryBoundsAddress() == 917);
  CHECK(sim.getMemorySize() == (917 - 42 + 1));
}

TEST_CASE("Task 1: <initializeMemory()> memory initialization error conditions", "[task1]")
{

  // we use XYYY addresses, where X is an opcode and YYY is a reference
  // address, thus memory can't exceed 1000, only valid memory is 0 - 999
  sim.reset();

  // a bounds address of 1000 or bigger should throw an exception
  CHECK_THROWS_AS(sim.initializeMemory(99, 1000), SimulatorException);
  CHECK_THROWS_AS(sim.initializeMemory(99, 5000), SimulatorException);

  // a bounds address of less than 0 should be an error
  CHECK_THROWS_AS(sim.initializeMemory(99, -1), SimulatorException);

  // a base address of 1000 or bigger should throw an exception
  CHECK_THROWS_AS(sim.initializeMemory(5000, 700), SimulatorException);
  CHECK_THROWS_AS(sim.initializeMemory(-10, 200), SimulatorException);

  // memory that is negative in size, e.g. base address > bounds, is illegal
  CHECK_THROWS_AS(sim.initializeMemory(701, 700), SimulatorException);
  CHECK_THROWS_AS(sim.initializeMemory(1, 0), SimulatorException);
}

TEST_CASE("Task 1: <initializeMemory()> edge cases of memory initialization", "[task1]")
{

  // simulation with memory starting at base address 0 to 999 is valid
  sim.reset();
  sim.initializeMemory(0, 999);
  CHECK(sim.getMemoryBaseAddress() == 0);
  CHECK(sim.getMemoryBoundsAddress() == 999);
  CHECK(sim.getMemorySize() == 1000);

  // simulation with memory size of 1 is possible
  sim.reset();
  sim.initializeMemory(10, 10);
  CHECK(sim.getMemoryBaseAddress() == 10);
  CHECK(sim.getMemoryBoundsAddress() == 10);
  CHECK(sim.getMemorySize() == 1);
}

// NOTE: we really should test if you initialize all of memory to have 0 values, but we cannot
//   test this until we implement the peekAddress().
#endif

/**
 * @brief Task 2: test memory address translation
 */
#ifdef task2
TEST_CASE("Task 2: <translateAddress()>  basic memory address translation", "[task2]")
{
  // a typical memory address space to test first
  sim.initializeMemory(300, 999);
  CHECK(sim.getMemoryBaseAddress() == 300);
  CHECK(sim.getMemoryBoundsAddress() == 999);
  CHECK(sim.getMemorySize() == 700);

  // test some translations of addresses in this address space
  CHECK(sim.translateAddress(300) == 0);
  CHECK(sim.translateAddress(476) == 176);
  CHECK(sim.translateAddress(999) == 699); // last legal address in ths address space
}

TEST_CASE("Task 2: <translateAddress()>  basic illegal translation requests", "[task2]")
{
  // a typical memory address space to test first
  sim.reset();
  sim.initializeMemory(300, 999);

  // check bounds testing, we should get exception if try and reference an
  // illegal address
  CHECK_THROWS_AS(sim.translateAddress(299), SimulatorException);
  CHECK_THROWS_AS(sim.translateAddress(1000), SimulatorException);
}

TEST_CASE("Task 2: <translateAddress()>  more complex memory address translation", "[task2]")
{
  // a second more difficult memory address space
  sim.reset();
  sim.initializeMemory(187, 432);
  CHECK(sim.getMemoryBaseAddress() == 187);
  CHECK(sim.getMemoryBoundsAddress() == 432);
  CHECK(sim.getMemorySize() == (432 - 187 + 1));

  // test some translations of addresses in this address space
  CHECK(sim.translateAddress(187) == 0);
  CHECK(sim.translateAddress(217) == 30);
  CHECK(sim.translateAddress(432) == (432 - 187)); // last legal address in ths address space
}

TEST_CASE("Task 2: <translateAddress()>  more complex illegal address translations", "[task2]")
{
  // a second more difficult memory address space
  sim.reset();
  sim.initializeMemory(187, 432);

  // check bounds testing, we should get exception if try and reference an
  // illegal address
  CHECK_THROWS_AS(sim.translateAddress(186), SimulatorException);
  CHECK_THROWS_AS(sim.translateAddress(433), SimulatorException);
}
#endif

/**
 * @brief Task 3: test memory access functions
 */
#ifdef task3_1
TEST_CASE("Task 3: <peek() and poke()> check memory initialization", "[task3]")
{
  // We couldn't test that you were allocating memory and initializing all values to 0
  // until we had a peak() implementation.  pokes and peaks may crash if you are not
  // dynamically allocating memory, reread task 1 if you get a runtime crash trying
  // to access memory for the first time here
  sim.reset();
  sim.initializeMemory(300, 999);

  CHECK(sim.peekAddress(300) == 0);
  CHECK(sim.peekAddress(999) == 0);

  for (int simAddress = 300; simAddress <= 999; simAddress++)
  {
    CHECK(sim.peekAddress(simAddress) == 0);
  }
}

TEST_CASE("Task 3: <peek() and poke()> basic  memory peeks and pokes", "[task3]")
{
  // a typical memory address space to test first
  sim.reset();
  sim.initializeMemory(300, 999);
  CHECK(sim.getMemoryBaseAddress() == 300);
  CHECK(sim.getMemoryBoundsAddress() == 999);
  CHECK(sim.getMemorySize() == 700);

  // poke some random locations then peek and make sure
  // we get same value back
  CHECK(sim.peekAddress(300) == 0);
  sim.pokeAddress(300, 42);
  CHECK(sim.peekAddress(300) == 42);

  CHECK(sim.peekAddress(999) == 0);
  sim.pokeAddress(999, 1867);
  CHECK(sim.peekAddress(999) == 1867);

  CHECK(sim.peekAddress(456) == 0);
  sim.pokeAddress(456, 789);
  CHECK(sim.peekAddress(456) == 789);

  CHECK(sim.peekAddress(789) == 0);
  sim.pokeAddress(789, 456);
  CHECK(sim.peekAddress(789) == 456);
}

TEST_CASE("Task 3: <peek() and poke()> poke and peak error checking", "[task3]")
{
  // a typical memory address space to test first
  sim.reset();
  sim.initializeMemory(300, 999);

  // should  be illegal to peek and poke to addresses beyond
  // our simulation memory address space
  CHECK_THROWS_AS(sim.pokeAddress(299, 42), SimulatorException);
  CHECK_THROWS_AS(sim.peekAddress(299), SimulatorException);
  CHECK_THROWS_AS(sim.pokeAddress(1000, 42), SimulatorException);
  CHECK_THROWS_AS(sim.peekAddress(1000), SimulatorException);
}
#endif

/**
 * @brief After Task 3: test program loading function.  Upon completion of first 3 tasks,
 *   the loadProgram() function should have all member methods implemented in order for
 *   it to be able to now load simulation programs from files.  But you do need to uncomment
 *   the calls to pokeAddress() and initializeMemory() in the loadProgram() member function
 *   first.
 */
#ifdef task3_2
TEST_CASE("Task 3: <loadProgram()> HypotheticalMachineController test program load", "[task3]")
{
  // should be throwing exception when file name is not correct
  string progFile = "simfiles/badfile.sim";
  CHECK_THROWS_AS(sim.loadProgram(progFile), SimulatorException);

  // load actual program and see we get expected values into
  // the hypothetical machine
  progFile = "simfiles/prog-01.sim";
  sim.loadProgram(progFile);
  CHECK(sim.getPC() == 300);
  CHECK(sim.getAC() == 0);
  CHECK(sim.getIR() == 0);
  CHECK(sim.getMemoryBaseAddress() == 300);
  CHECK(sim.getMemoryBoundsAddress() == 999);
  CHECK(sim.getMemorySize() == 700);

  // test memory was loaded as expected
  CHECK(sim.peekAddress(300) == 1940);
  CHECK(sim.peekAddress(301) == 5941);
  CHECK(sim.peekAddress(302) == 2941);
  CHECK(sim.peekAddress(303) == 0);
  CHECK(sim.peekAddress(939) == 0);
  CHECK(sim.peekAddress(940) == 3);
  CHECK(sim.peekAddress(941) == 2);
  CHECK(sim.peekAddress(942) == 0);

  // load another program for more tests
  progFile = "simfiles/prog-02.sim";
  sim.loadProgram(progFile);
  CHECK(sim.getPC() == 50);
  CHECK(sim.getAC() == 0);
  CHECK(sim.getIR() == 0);
  CHECK(sim.getMemoryBaseAddress() == 50);
  CHECK(sim.getMemoryBoundsAddress() == 150);
  CHECK(sim.getMemorySize() == 101);

  // test memory was loaded as expected
  CHECK(sim.peekAddress(50) == 1141);
  CHECK(sim.peekAddress(51) == 5140);
  CHECK(sim.peekAddress(52) == 3051);
  CHECK(sim.peekAddress(53) == 4142);
  CHECK(sim.peekAddress(54) == 0);
  CHECK(sim.peekAddress(140) == 2);
  CHECK(sim.peekAddress(141) == 4);
  CHECK(sim.peekAddress(142) == 8);
  CHECK(sim.peekAddress(143) == 0);
}
#endif

/**
 * @brief Task 4: test fetch phase
 */
#ifdef task4
TEST_CASE("Task 4: <fetch()> test fetch phase for prog-01", "[task4]")
{
  // load a program and check results of performing fetch cycles
  string progFile = "simfiles/prog-01.sim";
  sim.loadProgram(progFile);

  // initially in prog-01 PC is 300, should fetch instruction 1940
  // and then we fetch until the 0/noop/halt
  sim.fetch();
  CHECK(sim.getIR() == 1940);
  CHECK(sim.getPC() == 300);
  sim.incrementPC();

  sim.fetch();
  CHECK(sim.getIR() == 5941);
  CHECK(sim.getPC() == 301);
  sim.incrementPC();

  sim.fetch();
  CHECK(sim.getIR() == 2941);
  CHECK(sim.getPC() == 302);
  sim.incrementPC();

  sim.fetch();
  CHECK(sim.getIR() == 0);
  CHECK(sim.getPC() == 303);
  sim.incrementPC();
}

TEST_CASE("Task 4: <fetch()> test fetch phase for prog-02", "[task4]")
{
  // load a second program and step through it
  string progFile = "simfiles/prog-02.sim";
  sim.loadProgram(progFile);

  // initially in prog-01 PC is 300, should fetch instruction 1940
  // and then we fetch until the 0/noop/halt
  sim.fetch();
  CHECK(sim.getIR() == 1141);
  CHECK(sim.getPC() == 50);
  sim.incrementPC();

  sim.fetch();
  CHECK(sim.getIR() == 5140);
  CHECK(sim.getPC() == 51);
  sim.incrementPC();

  sim.fetch();
  CHECK(sim.getIR() == 3051);
  CHECK(sim.getPC() == 52);
  sim.incrementPC();

  sim.fetch();
  CHECK(sim.getIR() == 4142);
  CHECK(sim.getPC() == 53);
  sim.incrementPC();

  sim.fetch();
  CHECK(sim.getIR() == 0);
  CHECK(sim.getPC() == 54);
  sim.incrementPC();
}
#endif

/**
 * @brief Task 5: test execute phase
 */
#ifdef task5
TEST_CASE("Task 5: <execute()> ir translation in execute() for prog-01", "[task5]")
{
  // the execute() part of cycle starts out by translating
  // the fetched ir into opcode and address parts, test these are translated
  // correctly
  string progFile = "simfiles/prog-01.sim";
  sim.loadProgram(progFile);
  sim.fetch();
  CHECK(sim.getIR() == 1940);
  sim.execute();
  CHECK(sim.getIROpcode() == 1);
  CHECK(sim.getIRAddress() == 940);

  sim.fetch();
  CHECK(sim.getIR() == 5941);
  sim.execute();
  CHECK(sim.getIROpcode() == 5);
  CHECK(sim.getIRAddress() == 941);

  sim.fetch();
  CHECK(sim.getIR() == 2941);
  sim.execute();
  CHECK(sim.getIROpcode() == 2);
  CHECK(sim.getIRAddress() == 941);
}

TEST_CASE("Task 5: <execute()> test execution of error value", "[task5]")
{
  // the execute() part of cycle starts out by translating
  // the fetched ir into opcode and address parts, test these are translated
  // correctly
  string progFile = "simfiles/prog-01.sim";
  sim.loadProgram(progFile);
  sim.fetch();
  sim.execute();

  sim.fetch();
  sim.execute();

  sim.fetch();
  sim.execute();

  // test that a nonsensical ir value is detected
  sim.pokeAddress(303, 10000); // only values of 0000 - 9999 make sense in this sim
  sim.fetch();
  CHECK(sim.getIR() == 10000);
  CHECK_THROWS_AS(sim.execute(), SimulatorException);

  // PC should not be incremented, try a negative value in ir
  sim.pokeAddress(303, -5); // only values of 0000 - 9999 make sense in this sim
  sim.fetch();
  CHECK(sim.getIR() == -5);
  CHECK_THROWS_AS(sim.execute(), SimulatorException);
}

TEST_CASE("Task 5: <execute()> ir translation in execute() for prog-02", "[task5]")
{

  // check a second set of translations to ensure working
  string progFile = "simfiles/prog-02.sim";
  sim.loadProgram(progFile);
  sim.fetch();
  CHECK(sim.getIR() == 1141);
  sim.execute();
  CHECK(sim.getIROpcode() == 1);
  CHECK(sim.getIRAddress() == 141);

  sim.fetch();
  CHECK(sim.getIR() == 5140);
  sim.execute();
  CHECK(sim.getIROpcode() == 5);
  CHECK(sim.getIRAddress() == 140);

  sim.fetch();
  CHECK(sim.getIR() == 3051);
  sim.execute();
  CHECK(sim.getIROpcode() == 3);
  CHECK(sim.getIRAddress() == 51);
}
#endif

/**
 * @brief Task 6: test load instruction
 */
#ifdef task6_1
TEST_CASE("Task 6: <executeLoad()> load instruction", "[task6]")
{
  // prog-03 tests load instructions
  string progFile = "simfiles/prog-03.sim";
  sim.loadProgram(progFile);

  // fetch and execute the 3 load instructions and test the result is
  // as we expect
  sim.fetch();
  sim.execute();
  CHECK(sim.getIROpcode() == 1);
  CHECK(sim.getIRAddress() == 150);
  CHECK(sim.getAC() == 42);

  sim.fetch();
  sim.execute();
  CHECK(sim.getIROpcode() == 1);
  CHECK(sim.getIRAddress() == 175);
  CHECK(sim.getAC() == -5);

  sim.fetch();
  sim.execute();
  CHECK(sim.getIROpcode() == 1);
  CHECK(sim.getIRAddress() == 190);
  CHECK(sim.getAC() == 123);

  // load from out of bounds reference should fail
  sim.fetch();
  CHECK_THROWS_AS(sim.execute(), SimulatorException);
}
#endif

/**
 * @brief Task 6: test store instruction
 */
#ifdef task6_2
TEST_CASE("Task 6: <executeStore()> store instruction", "[task6]")
{
  // prog-04 tests store instructions
  string progFile = "simfiles/prog-04.sim";
  sim.loadProgram(progFile);

  // fetch and execute the 3 store instructions and test the result is
  // as we expect
  sim.fetch();
  sim.execute();
  CHECK(sim.getIROpcode() == 2);
  CHECK(sim.getIRAddress() == 250);
  CHECK(sim.getAC() == 32);
  CHECK(sim.peekAddress(250) == 32);

  sim.fetch();
  sim.execute();
  CHECK(sim.getIROpcode() == 2);
  CHECK(sim.getIRAddress() == 242);
  CHECK(sim.getAC() == 32);
  CHECK(sim.peekAddress(242) == 32);

  sim.fetch();
  sim.execute();
  CHECK(sim.getIROpcode() == 2);
  CHECK(sim.getIRAddress() == 287);
  CHECK(sim.getAC() == 32);
  CHECK(sim.peekAddress(287) == 32);

  // next instruction loads from out of bounds reference and should fail
  sim.fetch();
  CHECK_THROWS_AS(sim.execute(), SimulatorException);
}
#endif

/**
 * @brief Task 6: test add and subtract instructions
 */
#ifdef task6_3
TEST_CASE("Task 6: <executeAdd() and executeSubtract()>  add and subtract instructions", "[task6]")
{
  // prog-05 tests addition and subtraction
  string progFile = "simfiles/prog-05.sim";
  sim.loadProgram(progFile);

  // subtract SUB=4
  // additon  ADD=5
  // fetch and execute the 8 instructions in this test program and determine
  // if the results are calculated as we expect
  sim.fetch();
  sim.execute();
  CHECK(sim.getIROpcode() == 4);
  CHECK(sim.getIRAddress() == 701);
  CHECK(sim.getAC() == 41);

  sim.fetch();
  sim.execute();
  CHECK(sim.getIROpcode() == 5);
  CHECK(sim.getIRAddress() == 701);
  CHECK(sim.getAC() == 42);

  sim.fetch();
  sim.execute();
  CHECK(sim.getIROpcode() == 4);
  CHECK(sim.getIRAddress() == 702);
  CHECK(sim.getAC() == 43);

  sim.fetch();
  sim.execute();
  CHECK(sim.getIROpcode() == 5);
  CHECK(sim.getIRAddress() == 702);
  CHECK(sim.getAC() == 42);

  sim.fetch();
  sim.execute();
  CHECK(sim.getIROpcode() == 4);
  CHECK(sim.getIRAddress() == 703);
  CHECK(sim.getAC() == 35);

  sim.fetch();
  sim.execute();
  CHECK(sim.getIROpcode() == 5);
  CHECK(sim.getIRAddress() == 704);
  CHECK(sim.getAC() == 30);

  sim.fetch();
  sim.execute();
  CHECK(sim.getIROpcode() == 4);
  CHECK(sim.getIRAddress() == 705);
  CHECK(sim.getAC() == 15);

  sim.fetch();
  sim.execute();
  CHECK(sim.getIROpcode() == 5);
  CHECK(sim.getIRAddress() == 705);
  CHECK(sim.getAC() == 30);
}
#endif

/**
 * @brief Task 6: test jump instruction
 */
#ifdef task6_4
TEST_CASE("Task 6: <executeJump()> jump instructions", "[task6]")
{
  // prog-06 tests jump instructions
  string progFile = "simfiles/prog-06.sim";
  sim.loadProgram(progFile);

  // JMP=6
  sim.fetch();
  sim.execute();
  CHECK(sim.getIROpcode() == 3);
  CHECK(sim.getIRAddress() == 750);
  CHECK(sim.getPC() == 750);

  sim.fetch();
  sim.execute();
  CHECK(sim.getIROpcode() == 3);
  CHECK(sim.getIRAddress() == 725);
  CHECK(sim.getPC() == 725);

  sim.fetch();
  sim.execute();
  CHECK(sim.getIROpcode() == 3);
  CHECK(sim.getIRAddress() == 790);
  CHECK(sim.getPC() == 790);

  sim.fetch();
  sim.execute();
  CHECK(sim.getIROpcode() == 0);
  CHECK(sim.getIRAddress() == 0);
}
#endif

/**
 * @brief Task 7: test full simulation
 */
#ifdef task7
TEST_CASE("Task 7: <fetch/execute cycles> simulation of whole program prog-01", "[task7]")
{
  // test program 01 execution
  string progFile = "simfiles/prog-01.sim";
  sim.loadProgram(progFile);

  int cycle = sim.runSimulation();
  CHECK(cycle == 4);
  CHECK(sim.getPC() == 303);
  CHECK(sim.getAC() == 5);
  CHECK(sim.peekAddress(941) == 5);
}

TEST_CASE("Task 7: <fetch/execute cycles> simulation of whole programs prog-02", "[task7]")
{
  // test program 02 execution
  // has an infinite loop so will run for the max cycles
  string progFile = "simfiles/prog-02.sim";
  sim.loadProgram(progFile);

  int cycle = sim.runSimulation();
  CHECK(cycle == 100);
  CHECK(sim.getPC() == 52);
  CHECK(sim.getAC() == 104);
}
#endif
