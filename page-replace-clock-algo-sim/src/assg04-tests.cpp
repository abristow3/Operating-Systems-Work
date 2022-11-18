/** @file assg04-tests.cpp
 * @brief Unit tests for page replacement scheme simulations and
 *   specifically for the implementation of the Clock Algorithm
 *   page replacement scheme.
 *
 * @author Student Name
 * @note   cwid: 123456
 * @date   Fall 2019
 * @note   ide:  g++ 8.2.0 / GNU Make 4.2.1
 *
 * Unit tests for assignment 04, Clock Algorithm page replacement
 * scheme simulation.
 */
// kludge, we redeclare private to be public here, which allows us to peek into
// classes private methods for testing
#define private public

#include "ClockPageReplacementScheme.hpp"
#include "FifoPageReplacementScheme.hpp"
#include "PageReplacementScheme.hpp"
#include "PagingSystem.hpp"
#include "SimulatorException.hpp"
#include "catch.hpp"

using namespace std;

/**
 * @brief Initial tests of loading and converting page file streams
 */
TEST_CASE("Setup 1: Initial tests of loading and converting page file streams", "[setup1]")
{
  // a basic paging system simulation with 5 physical frames of memory
  PagingSystem sim(5);

  // test bad file name exceptions
  CHECK_THROWS_AS(sim.loadPageStream("simfiles/bogus.sim"), SimulatorException);

  // test loading simulated page reference streams from file
  sim.loadPageStream("simfiles/pageref-01.sim");
  CHECK(sim.pageStreamToString() == "[ 2, 3, 2, 1, 5, 2, 4, 5, 3, 2, 5, 2 ]");

  sim.loadPageStream("simfiles/pageref-02.sim");
  CHECK(sim.pageStreamToString() == "[ 6, 5, 2, 4, 2, 4, 3, 5, 3, 6, 7, 5, 3, 5, 4, 3, 4, 7, 2, 1, 3, 7, 6, 4, 1, 2, 1, 3, 4, 7 ]");

  // test randomly generated page reference stream, test that random number
  // generator getting expected results for given seed value of 42
  sim.generateRandomPageStream(1, 10, 42);
  CHECK(sim.pageStreamToString() == "[ 7 ]");

  sim.generateRandomPageStream(20, 100, 42);
  CHECK(sim.pageStreamToString() == "[ 67, 41, 82, 42, 13, 59, 22, 41, 36, 44, 75, 44, 18, 5, 97, 63, 93, 49, 99, 60 ]");
}

/**
 * @brief Task 1: Implement basic PagingSystem accessor methods
 */
#define task1_1
#ifdef task1_1
TEST_CASE("Task 1: implement getMemorySize() accessor method", "[task1]")
{
  SECTION("Task 1: test memory size with 5 frames", "[task1]")
  {
    PagingSystem sim(5);

    CHECK(sim.getMemorySize() == 5);
  }

  SECTION("Task 1: test memory size with 25 frames", "[task1]")
  {
    PagingSystem sim(25);

    CHECK(sim.getMemorySize() == 25);
  }
}
#endif

#define task1_2
#ifdef task1_2
TEST_CASE("Task 1: implement getSystemTime() accessor method", "[task1]")
{
  SECTION("Task 1: systemTime should be initialized to 0", "[task1]")
  {
    PagingSystem sim(7);
    CHECK(sim.getSystemTime() == 0);
  }
}
#endif

#define task1_3
#ifdef task1_3
TEST_CASE("Task 1: implement getNumPageReferences() accessor method", "[task1]")
{
  SECTION("Task 1: numPageReferences for test 01 is 12", "[task1]")
  {
    PagingSystem sim(5);
    sim.loadPageStream("simfiles/pageref-01.sim");
    CHECK(sim.getNumPageReferences() == 12);
  }

  SECTION("Task 1: numPageReferences for test 02 is 30", "[task1]")
  {
    PagingSystem sim(5);
    sim.loadPageStream("simfiles/pageref-02.sim");
    CHECK(sim.getNumPageReferences() == 30);
  }

  SECTION("Task 1: numPageReferences for 20 random page stream", "[task1]")
  {
    PagingSystem sim(5);
    sim.generateRandomPageStream(20, 100, 42);
    CHECK(sim.getNumPageReferences() == 20);
  }

  SECTION("Task 1: numPageReferences for 1000 random page stream", "[task1]")
  {
    PagingSystem sim(5);
    sim.generateRandomPageStream(1000, 100, 42);
    CHECK(sim.getNumPageReferences() == 1000);
  }
}
#endif

/**
 * @brief Task 2: Implement isMemoryFull() member method
 */
#define task2
#ifdef task2
TEST_CASE("Task 2: Implement isMemoryFull() member method (3 frames)", "[task2]")
{
  // create a basic paging system simulation with 3 physical frames of
  // and load the basic page stream from simulation 01
  PagingSystem sim(3);
  sim.loadPageStream("simfiles/pageref-01.sim");
  CHECK(sim.getMemorySize() == 3);
  CHECK(sim.getSystemTime() == 0);
  CHECK(sim.getNumPageReferences() == 12);

  SECTION("Task 2: initially memory is not full", "[task2]")
  {
    CHECK_FALSE(sim.isMemoryFull());
  }

  // populate frame 03
  sim.pokeMemory(0, 2); // memory[0] 2
  CHECK(sim.peekMemory(0) == 2);
  CHECK(sim.memoryToString() == "[ 2, 0, 0 ]");

  SECTION("Task 2: memory still not full with page in frame 0", "[task2]")
  {
    CHECK_FALSE(sim.isMemoryFull());
  }

  // populate last frame
  sim.pokeMemory(2, 4); // memory[2] 4
  CHECK(sim.peekMemory(2) == 4);
  CHECK(sim.memoryToString() == "[ 2, 0, 4 ]");

  SECTION("Task 2: memory still not full because frame 1 still empty", "[task2]")
  {
    CHECK_FALSE(sim.isMemoryFull());
  }

  // fill up memory
  sim.pokeMemory(1, 1); // memory[1] 1
  CHECK(sim.peekMemory(1) == 1);
  CHECK(sim.memoryToString() == "[ 2, 1, 4 ]");

  SECTION("Task 2: memory is now full", "[task2]")
  {
    CHECK(sim.isMemoryFull());
  }

  // empty out frame 0, memory should not be full again
  sim.pokeMemory(0, EMPTY_FRAME); // memory[1] 0
  CHECK(sim.peekMemory(0) == 0);
  CHECK(sim.memoryToString() == "[ 0, 1, 4 ]");

  SECTION("Task 2: memory not full again with empty frame 0", "[task2]")
  {
    CHECK_FALSE(sim.isMemoryFull());
  }

  // make only last frame be empty
  sim.pokeMemory(0, 5);           // memory[0] 5
  sim.pokeMemory(2, EMPTY_FRAME); // memory[2] 0
  CHECK(sim.peekMemory(0) == 5);
  CHECK(sim.peekMemory(2) == 0);
  CHECK(sim.memoryToString() == "[ 5, 1, 0 ]");

  SECTION("Task 2: memory not full again with only frame 2 empty", "[task2]")
  {
    CHECK_FALSE(sim.isMemoryFull());
  }

  // make mamory full again
  sim.pokeMemory(2, 3); // memory[2] 3
  CHECK(sim.peekMemory(2) == 3);
  CHECK(sim.memoryToString() == "[ 5, 1, 3 ]");

  SECTION("Task 2: memory is now full again", "[task2]")
  {
    CHECK(sim.isMemoryFull());
  }
}

TEST_CASE("Task 2: Implement isMemoryFull() member method (7 frames)", "[task2]")
{
  // create a basic paging system simulation with 7 physical frames of
  // and load the basic page stream from simulation 01
  PagingSystem sim(7);
  sim.loadPageStream("simfiles/pageref-01.sim");
  CHECK(sim.getMemorySize() == 7);
  CHECK(sim.getSystemTime() == 0);
  CHECK(sim.getNumPageReferences() == 12);

  SECTION("Task 2: 7 frames initially memory is not full", "[task2]")
  {
    CHECK_FALSE(sim.isMemoryFull());
  }

  // populate first 4 frames
  sim.pokeMemory(0, 2);
  sim.pokeMemory(1, 3);
  sim.pokeMemory(2, 4);
  sim.pokeMemory(3, 5);
  CHECK(sim.memoryToString() == "[ 2, 3, 4, 5, 0, 0, 0 ]");

  SECTION("Task 2: 7 frames first 4 frames populated, but memory still not full", "[task2]")
  {
    CHECK_FALSE(sim.isMemoryFull());
  }

  // fill up memory
  sim.pokeMemory(4, 6);
  sim.pokeMemory(5, 7);
  sim.pokeMemory(6, 8);
  CHECK(sim.memoryToString() == "[ 2, 3, 4, 5, 6, 7, 8 ]");

  SECTION("Task 2: 7 frames memory should now be full", "[task2]")
  {
    CHECK(sim.isMemoryFull());
  }

  // make sure if a hole opens up in middle, memory is now not full again
  sim.pokeMemory(4, EMPTY_FRAME);
  CHECK(sim.memoryToString() == "[ 2, 3, 4, 5, 0, 7, 8 ]");

  SECTION("Task 2: 7 frames memory is not full again because empty frame 4", "[task2]")
  {
    CHECK_FALSE(sim.isMemoryFull());
  }
}
#endif

/**
 * @brief Task 3: Implement isPageHit() member method
 */
#define task3
#ifdef task3
TEST_CASE("Task 3: Implement isPageHit() member method (3 frames)", "[task3]")
{
  // create a basic paging system simulation with 3 physical frames of
  // and load the basic page stream from simulation 01
  PagingSystem sim(3);
  sim.loadPageStream("simfiles/pageref-01.sim");

  // populate memory with some initial page reference for testing
  sim.pokeMemory(0, 2); // memory[0] 2
  CHECK(sim.peekMemory(0) == 2);
  sim.pokeMemory(1, 1); // memory[1] 1
  CHECK(sim.peekMemory(1) == 1);
  sim.pokeMemory(2, 4); // memory[2] 4
  CHECK(sim.peekMemory(2) == 4);

  // this is what system state should look like now
  CHECK(sim.getMemorySize() == 3);
  CHECK(sim.getSystemTime() == 0);
  CHECK(sim.getNumPageReferences() == 12);
  CHECK(sim.isMemoryFull());
  CHECK(sim.memoryToString() == "[ 2, 1, 4 ]");

  // test hit or miss determination and system time increment
  // systemTime = 0 is reference to page 2, which is a hit
  SECTION("Task 3: 3 frames, time 0 is a hit", "[task3]")
  {
    CHECK(sim.isPageHit());
  }

  // systemTime = 1 page reference 3 is a miss
  sim.incrementSystemTime();
  SECTION("Task 3: 3 frames, time 1 is a miss", "[task3]")
  {
    CHECK(sim.getSystemTime() == 1);
    CHECK_FALSE(sim.isPageHit());
  }

  // systemTime = 2 page referece 2 is a hit
  sim.incrementSystemTime();
  SECTION("Task 3: 3 frames, time 2 is a hit", "[task3]")
  {
    CHECK(sim.getSystemTime() == 2);
    CHECK(sim.isPageHit());
  }

  // systemTime = 3 page reference 1 is a hit
  sim.incrementSystemTime();
  SECTION("Task 3: 3 frames, time 3 is a hit", "[task3]")
  {
    CHECK(sim.getSystemTime() == 3);
    CHECK(sim.isPageHit());
  }

  // systemTime = 4  page reference 5 is a miss
  sim.incrementSystemTime();
  SECTION("Task 3: 3 frames, time 4 is a miss", "[task3]")
  {
    CHECK(sim.getSystemTime() == 4);
    CHECK_FALSE(sim.isPageHit());
  }

  // systemTime = 5 page reference 2 is a hit
  sim.incrementSystemTime();
  SECTION("Task 3: 3 frames, time 5 is a hit", "[task3]")
  {
    CHECK(sim.getSystemTime() == 5);
    CHECK(sim.isPageHit());
  }

  // systemTime = 6 page reference 4 is a hit
  sim.incrementSystemTime();
  SECTION("Task 3: 3 frames, time 6 is a hit", "[task3]")
  {
    CHECK(sim.getSystemTime() == 6);
    CHECK(sim.isPageHit());
  }

  // systemTime = 7 page reference 5 is a miss
  sim.incrementSystemTime();
  SECTION("Task 3: 3 frames, time 7 is a miss", "[task3]")
  {
    CHECK(sim.getSystemTime() == 7);
    CHECK_FALSE(sim.isPageHit());
  }

  // systemTime = 8 page reference 3 is a miss
  sim.incrementSystemTime();
  SECTION("Task 3: 3 frames, time 8 is a miss", "[task3]")
  {
    CHECK(sim.getSystemTime() == 8);
    CHECK_FALSE(sim.isPageHit());
  }

  // systemTime = 9 page reference 2 is a hit
  sim.incrementSystemTime();
  SECTION("Task 3: 3 frames, time 9 is a hit", "[task3]")
  {
    CHECK(sim.getSystemTime() == 9);
    CHECK(sim.isPageHit());
  }

  // systemTime = 10 page reference 5 is a miss
  sim.incrementSystemTime();
  SECTION("Task 3: 3 frames, time 10 is a miss", "[task3]")
  {
    CHECK(sim.getSystemTime() == 10);
    CHECK_FALSE(sim.isPageHit());
  }

  // systemTime = 11 page reference 2 is a hit
  sim.incrementSystemTime();
  SECTION("Task 3: 3 frames, time 11 is a hit", "[task3]")
  {
    CHECK(sim.getSystemTime() == 11);
    CHECK(sim.isPageHit());
  }
}

TEST_CASE("Task 3: Implement isPageHit() member method (6 frames)", "[task3]")
{
  // create a basic paging system simulation with 6 physical frames of
  // and load the basic page stream from simulation 02
  PagingSystem sim(6);
  sim.loadPageStream("simfiles/pageref-02.sim");

  // populate memory with some initial page reference for testing
  sim.pokeMemory(0, 2);
  sim.pokeMemory(1, 8);
  sim.pokeMemory(2, 9);
  sim.pokeMemory(3, 6);
  sim.pokeMemory(4, 10);
  sim.pokeMemory(5, 3);

  // this is what system state should look like now
  CHECK(sim.getMemorySize() == 6);
  CHECK(sim.getSystemTime() == 0);
  CHECK(sim.getNumPageReferences() == 30);
  CHECK(sim.isMemoryFull());
  CHECK(sim.memoryToString() == "[ 2, 8, 9, 6, 10, 3 ]");

  // test hit or miss determination and system time increment
  // systemTime = 0 is reference to page 6, which is a hit
  SECTION("Task 3: 6 frames, time 0 is a hit", "[task3]")
  {
    CHECK(sim.isPageHit());
  }

  // systemTime = 1 page reference 5 is a miss
  sim.incrementSystemTime();
  SECTION("Task 3: 6 frames, time 1 is a miss", "[task3]")
  {
    CHECK(sim.getSystemTime() == 1);
    CHECK_FALSE(sim.isPageHit());
  }

  // systemTime = 2 page reference 2 is a hit
  sim.incrementSystemTime();
  SECTION("Task 3: 6 frames, time 2 is a hit", "[task3]")
  {
    CHECK(sim.getSystemTime() == 2);
    CHECK(sim.isPageHit());
  }

  // systemTime = 3 page reference 4 is a miss
  sim.incrementSystemTime();
  SECTION("Task 3: 6 frames, time 3 is a miss", "[task3]")
  {
    CHECK(sim.getSystemTime() == 3);
    CHECK_FALSE(sim.isPageHit());
  }

  // systemTime = 4 page reference 2 is a hit
  sim.incrementSystemTime();
  SECTION("Task 3: 6 frames, time 4 is a hit", "[task3]")
  {
    CHECK(sim.getSystemTime() == 4);
    CHECK(sim.isPageHit());
  }

  // systemTime = 5 page reference 4 is a miss
  sim.incrementSystemTime();
  SECTION("Task 3: 6 frames, time 5 is a miss", "[task3]")
  {
    CHECK(sim.getSystemTime() == 5);
    CHECK_FALSE(sim.isPageHit());
  }

  // systemTime = 6 page reference 3 is a hit
  sim.incrementSystemTime();
  SECTION("Task 3: 6 frames, time 6 is a hit", "[task3]")
  {
    CHECK(sim.getSystemTime() == 6);
    CHECK(sim.isPageHit());
  }
}
#endif

/**
 * @brief Task 4: Implement doPagePlacement() member method
 */
#define task4
#ifdef task4
TEST_CASE("Task 4: Test initial doPagePlacement() into empty frames (3 frames)", "[task4]")
{
  // create a basic paging system simulation with 3 physical frames of
  // and load the basic page stream from simulation 01
  PagingSystem sim(3);
  sim.loadPageStream("simfiles/pageref-01.sim");

  // memory is initially empty.  Calling processNextPageReference()
  // should cause initial page placements to occur, until memory
  // becomes full.
  sim.processNextPageReference();
  SECTION("Task 4: (3 frames) time 0 page 2 placed in frame 0", "[task4]")
  {
    CHECK(sim.getSystemTime() == 1);
    CHECK_FALSE(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 2, 0, 0 ]");
  }

  // next reference will go to next free frame
  sim.processNextPageReference();
  SECTION("Task 4: (3 frames) time 1 page 3 placed in frame 1", "[task4]")
  {
    CHECK(sim.getSystemTime() == 2);
    CHECK_FALSE(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 2, 3, 0 ]");
  }

  // next reference is a hit, memory should not change
  sim.processNextPageReference();
  SECTION("Task 4: (3 frames) time 2 hit no placement occurred", "[task4]")
  {
    CHECK(sim.getSystemTime() == 3);
    CHECK(not sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 2, 3, 0 ]");
  }

  // next reference is a miss and it fills up our 3 frame memory
  sim.processNextPageReference();
  SECTION("Task 4: (3 frames) time 3 page 1 placed in frame 2", "[task4]")
  {
    CHECK(sim.getSystemTime() == 4);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 2, 3, 1 ]");
  }

  // doPagePlacement() should never be called when memory is full
  // test correctly throws exception here
  SECTION("Task 4: (3 frames) error for doPagePlacement() to be called when memory is full", "[task4]")
  {
    CHECK_THROWS_AS(sim.doPagePlacement(), SimulatorException);
  }

  // if we free up some frames of memory, we should be able to
  // continue with page placements
  sim.pokeMemory(0, EMPTY_FRAME);
  sim.pokeMemory(2, EMPTY_FRAME);
  sim.processNextPageReference();
  SECTION("Task 4: (3 frames) time 4 page 5 placed in now empty frame 0", "[task4]")
  {
    CHECK(sim.getSystemTime() == 5);
    CHECK(not sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 5, 3, 0 ]");
  }

  // next reference is a miss
  sim.processNextPageReference();
  SECTION("Task 4: (3 frames) time 5 page 2 placed in now empty frame 2", "[task4]")
  {
    CHECK(sim.getSystemTime() == 6);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 5, 3, 2 ]");
  }
}

TEST_CASE("Task 4: Test initial doPagePlacement() into empty frames (5 frames)", "[task4]")
{
  // create a basic paging system simulation with 5 physical frames of
  // and load the basic page stream from simulation 02
  PagingSystem sim(5);
  sim.loadPageStream("simfiles/pageref-02.sim");

  // memory is initially empty.  Calling processNextPageReference()
  // should cause initial page placements to occur, until memory
  // becomes full.
  sim.processNextPageReference();
  SECTION("Task 4: (5 frames) time 0 page 6 placed in frame 0", "[task4]")
  {
    CHECK(sim.getSystemTime() == 1);
    CHECK_FALSE(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 6, 0, 0, 0, 0 ]");
  }

  // next reference will go to next free frame
  sim.processNextPageReference();
  SECTION("Task 4: (5 frames) time 1 page 5 placed in frame 1", "[task4]")
  {
    CHECK(sim.getSystemTime() == 2);
    CHECK_FALSE(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 6, 5, 0, 0, 0 ]");
  }

  // next reference will go to next free frame
  sim.processNextPageReference();
  SECTION("Task 4: (5 frames) time 2 page 2 placed in frame 2", "[task4]")
  {
    CHECK(sim.getSystemTime() == 3);
    CHECK_FALSE(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 6, 5, 2, 0, 0 ]");
  }

  // next reference will go to next free frame
  sim.processNextPageReference();
  SECTION("Task 4: (5 frames) time 3 page 4 placed in frame 3", "[task4]")
  {
    CHECK(sim.getSystemTime() == 4);
    CHECK_FALSE(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 6, 5, 2, 4, 0 ]");
  }

  // next reference is a hit, no placement occurs
  sim.processNextPageReference();
  SECTION("Task 4: (5 frames) time 4 hit on reference to page 2", "[task4]")
  {
    CHECK(sim.getSystemTime() == 5);
    CHECK_FALSE(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 6, 5, 2, 4, 0 ]");
  }

  // next reference is a hit, no placement occurs
  sim.processNextPageReference();
  SECTION("Task 4: (5 frames) time 5 hit on reference to page 4", "[task4]")
  {
    CHECK(sim.getSystemTime() == 6);
    CHECK_FALSE(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 6, 5, 2, 4, 0 ]");
  }

  // next reference is a miss fills up last empty frame
  sim.processNextPageReference();
  SECTION("Task 4: (5 frames) time 6 page 3 placed in last tmpty frame 4", "[task4]")
  {
    CHECK(sim.getSystemTime() == 7);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 6, 5, 2, 4, 3 ]");
  }

  // doPagePlacement() should never be called when memory is full
  // test correctly throws exception here
  SECTION("Task 4: (5 frames) error for doPagePlacement() to be called when memory is full", "[task4]")
  {
    CHECK_THROWS_AS(sim.doPagePlacement(), SimulatorException);
  }

  // if we free up a frame of memory should be able to continue with
  // page placements
  sim.pokeMemory(3, EMPTY_FRAME);

  // next reference is a hit, no placement occurs
  sim.processNextPageReference();
  SECTION("Task 4: (5 frames) time 7 hit on reference to page 5", "[task4]")
  {
    CHECK(sim.getSystemTime() == 8);
    CHECK_FALSE(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 6, 5, 2, 0, 3 ]");
  }

  // next reference is a hit, no placement occurs
  sim.processNextPageReference();
  SECTION("Task 4: (5 frames) time 8 hit on reference to page 3", "[task4]")
  {
    CHECK(sim.getSystemTime() == 9);
    CHECK_FALSE(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 6, 5, 2, 0, 3 ]");
  }

  // next reference is a hit, no placement occurs
  sim.processNextPageReference();
  SECTION("Task 4: (5 frames) time 9 hit on reference to page 6", "[task4]")
  {
    CHECK(sim.getSystemTime() == 10);
    CHECK_FALSE(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 6, 5, 2, 0, 3 ]");
  }

  // next reference is a miss, should get placed in empty frame 3
  sim.processNextPageReference();
  SECTION("Task 4: (5 frames) time 10 page 7 placed in empty frame 3", "[task4]")
  {
    CHECK(sim.getSystemTime() == 11);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 6, 5, 2, 7, 3 ]");
  }
}
#endif

/**
 * @brief Task 5: Enable full PagingSystem simulations using default Fifo page replacement policy
 */
#define task5
#ifdef task5
TEST_CASE("Task 5: Test basic page replacement using default fifo replacement scheme", "[task5]")
{
  // create a basic paging system simulation with 3 physical frames of
  // and load the basic page stream from simulation 01
  PagingSystem sim(3);
  sim.loadPageStream("simfiles/pageref-01.sim");

  // test that doPageReplacement() throws exception if called when
  // memory is not full
  CHECK_THROWS_AS(sim.doPageReplacement(), SimulatorException);

  // first 4 page references should cause 3 page placements (3 misses
  // and 1 hit) where we end up with a full memory
  for (int time = 0; time < 4; time++)
  {
    sim.processNextPageReference();
  }

  SECTION("Task 5: test stream 01 after initial page placements (fifo)", "[task5]")
  {
    CHECK(sim.getSystemTime() == 4);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 2, 3, 1 ]");
  }

  // memory is now full, we can now check the fifo page replacement
  // next memory reference is page 5, should replace page 2 in
  // frame 0
  sim.processNextPageReference();
  SECTION("Task 5: test stream 01 time 4 page fault replaces frame 0 (fifo)", "[task5]")
  {
    CHECK(sim.getSystemTime() == 5);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 5, 3, 1 ]");
  }

  // next reference is to page 2, another miss, page 3 in frame 1
  // should be replaced
  sim.processNextPageReference();
  SECTION("Task 5: test stream 01 time 5 page fault replaces frame 1 (fifo)", "[task5]")
  {
    CHECK(sim.getSystemTime() == 6);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 5, 2, 1 ]");
  }

  // next reference is again a miss, replace frame 2 page 1 with
  // page 4
  sim.processNextPageReference();
  SECTION("Task 5: test stream 01 time 6 page fault replaces frame 2 (fifo)", "[task5]")
  {
    CHECK(sim.getSystemTime() == 7);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 5, 2, 4 ]");
  }

  // next reference is to page 5 which is a hit, memory should not change
  sim.processNextPageReference();
  SECTION("Task 5: test stream 01 time 7 page hit (fifo)", "[task5]")
  {
    CHECK(sim.getSystemTime() == 8);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 5, 2, 4 ]");
  }

  // next refernce is page 3, a miss.  Test wrap around back to
  // beginning of physical memory, frame 0/page 5 should be replaced
  sim.processNextPageReference();
  SECTION("Task 5: test stream 01 time 8 page fault, framePointer wraps replaces frame 0 (fifo)", "[task5]")
  {
    CHECK(sim.getSystemTime() == 9);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 3, 2, 4 ]");
  }

  // next reference is a hit on page 2, no changes in memory
  sim.processNextPageReference();
  SECTION("Task 5: test stream 01 time 9 page hit (fifo)", "[task5]")
  {
    CHECK(sim.getSystemTime() == 10);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 3, 2, 4 ]");
  }

  // next reference to page 5 is a miss, replace frame 1/page 2
  sim.processNextPageReference();
  SECTION("Task 5: test stream 01 time 10 page fault replaces frame 1 (fifo)", "[task5]")
  {
    CHECK(sim.getSystemTime() == 11);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 3, 5, 4 ]");
  }

  // final reference is to page 2, which is a miss again, replace
  // frame 2/page 4
  sim.processNextPageReference();
  SECTION("Task 5: test stream 01 time 11 page fault replaces frame 2 (fifo)", "[task5]")
  {
    CHECK(sim.getSystemTime() == 12);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 3, 5, 2 ]");
  }

  // we have reached end of simulation, it is an error to try
  // and continue processing page references
  SECTION("Task 5: test stream 01 simulator exception if no more page references in input (fifo)", "[task5]")
  {
    CHECK_THROWS_AS(sim.processNextPageReference(), SimulatorException);
  }
}

/**
 * @brief Task 5: Test full simulation exectuon on the default fifo page
 *   replacement scheme
 */
TEST_CASE("Task 5: Test full fifo simulation execution", "[task5]")
{
  SECTION("Task 5: test page reference stream 01", "[task5]")
  {
    // create a basic paging system simulation with 3 physical frames of
    // and load the basic page stream from simulation 01
    PagingSystem sim(3);

    // test that runSimulaiton() properly croaks if not page stream
    // has been loaded to simulate yet
    CHECK_THROWS_AS(sim.runSimulation(), SimulatorException);

    // load the simulation 01 and run the simulation to the end
    sim.loadPageStream("simfiles/pageref-01.sim");
    sim.runSimulation();
    CHECK(sim.getSystemTime() == 12);
    CHECK(sim.memoryToString() == "[ 3, 5, 2 ]");
  }

  SECTION("Task 5: test page reference stream 02", "[task5]")
  {
    // create a basic paging system simulation with 3 physical frames of
    // and load the basic page stream from simulation 01
    PagingSystem sim(5);

    // load the simulation 02 and run the simulation to the end
    sim.loadPageStream("simfiles/pageref-02.sim");
    sim.runSimulation();
    CHECK(sim.getSystemTime() == 30);
    CHECK(sim.memoryToString() == "[ 7, 1, 6, 2, 4 ]");
  }

  SECTION("Task 5: test randomly generated page reference stream with seed of 42", "[task5]")
  {
    // create a basic paging system simulation with 3 physical frames of
    // and load the basic page stream from simulation 01
    PagingSystem sim(25);

    // create a long random simulation with a known seed and
    // ensure always end up with same memory contents using
    // fifo replacement scheme
    sim.generateRandomPageStream(1000000, 100, 42);
    CHECK(sim.getNumPageReferences() == 1000000);
    sim.runSimulation();
    CHECK(sim.getSystemTime() == 1000000);
    CHECK(sim.memoryToString() == "[ 84, 34, 56, 7, 76, 93, 90, 72, 60, 29, 67, 4, 31, 96, 5, 30, 95, 69, 20, 1, 36, 70, 92, 80, 21 ]");
  }
}
#endif

/**
 * @brief Task 6: Clock data structures and reset/initialization tests
 */
#define task6
#ifdef task6
TEST_CASE("Task 6: Test reset and initialization of use bits, memory size 3", "[task6]")
{
  // reset scheme is called when the ClockPageReplacementScheme constructor is
  // invoked, test useBit member variable defined and initialized for memory size 3
  SECTION("Task 6: test reset scheme initialization memory size 3", "[task6]")
  {
    // create a paging system that uses Clock page replacement with a memory size of 3
    PagingSystem sim(3, CLOCK);

    // get a handle on the private clock replacement scheme from the simulation so we
    // can test it
    ClockPageReplacementScheme* scheme = (ClockPageReplacementScheme*)sim.scheme;

    // test frame pointer initialized
    CHECK(scheme->framePointer == 0);

    // test that we have 3 use bits all initialized to true and named
    // useBit as expected
    for (FrameNumber frame = 0; frame < 3; frame++)
    {
      CHECK(scheme->useBit[frame] == true);
    }
  }

  // test useBit member variable defined and initialized for very large
  // simulated memory, which will probably cause a crash if not being correctly
  // dynamically allocated
  SECTION("Task 6: test reset scheme initialization memory size 10000", "[task6]")
  {
    // create a paging system that uses Clock page replacement with a memory size of 3
    PagingSystem sim(10000, CLOCK);

    // get a handle on the private clock replacement scheme from the simulation so we
    // can test it
    ClockPageReplacementScheme* scheme = (ClockPageReplacementScheme*)sim.scheme;

    // test frame pointer initialized
    CHECK(scheme->framePointer == 0);

    // test that we have 10000 use bits all initialized to true and named
    // useBit as expected
    for (FrameNumber frame = 0; frame < 10000; frame++)
    {
      CHECK(scheme->useBit[frame] == true);
    }
  }
}
#endif

/**
 * @brief Task 7: Clock pageHit() member function tests
 */
#define task7
#ifdef task7
TEST_CASE("Task 7: test pageHit() in Clock scheme", "[task7]")
{
  // test pageHit() on simulation with memory size of 3 first
  SECTION("Task 7: test pageHit() in Clock scheme with memory size 3", "[task7]")
  {
    // create a paging system that uses Clock page replacement with a memory size of 3
    PagingSystem sim(3, CLOCK);

    // get a handle on the private clock replacement scheme from the simulation so we
    // can test it
    ClockPageReplacementScheme* scheme = (ClockPageReplacementScheme*)sim.scheme;

    // test frame pointer initialized
    CHECK(scheme->framePointer == 0);

    // artificially set all use bits to false
    for (FrameNumber frame = 0; frame < 3; frame++)
    {
      scheme->useBit[frame] = false;
    }

    // perform page hits on frames and check result
    scheme->pageHit(0);
    CHECK(scheme->useBit[0] == true);
    CHECK(scheme->useBit[1] == false);
    CHECK(scheme->useBit[2] == false);

    // perform page hits on frames and check result
    scheme->pageHit(2);
    CHECK(scheme->useBit[0] == true);
    CHECK(scheme->useBit[1] == false);
    CHECK(scheme->useBit[2] == true);

    // nothing happens if hit on page which already has use bit set
    scheme->pageHit(0);
    CHECK(scheme->useBit[0] == true);
    CHECK(scheme->useBit[1] == false);
    CHECK(scheme->useBit[2] == true);

    // hit final frame
    scheme->pageHit(1);
    CHECK(scheme->useBit[0] == true);
    CHECK(scheme->useBit[1] == true);
    CHECK(scheme->useBit[2] == true);
  }

  // test pageHit() on simulation with big memory size 10000
  SECTION("Task 7: test pageHit() in Clock scheme with memory size 10000", "[task7]")
  {
    // create a paging system that uses Clock page replacement with a memory size of 3
    PagingSystem sim(10000, CLOCK);

    // get a handle on the private clock replacement scheme from the simulation so we
    // can test it
    ClockPageReplacementScheme* scheme = (ClockPageReplacementScheme*)sim.scheme;

    // test frame pointer initialized
    CHECK(scheme->framePointer == 0);

    // artificially set all use bits to false
    for (FrameNumber frame = 0; frame < 10000; frame++)
    {
      scheme->useBit[frame] = false;
    }

    // perform page hits on frame at beginning of memory
    scheme->pageHit(0);
    CHECK(scheme->useBit[0] == true);
    CHECK(scheme->useBit[1] == false);
    CHECK(scheme->useBit[2] == false);

    // perform page hits on frame in middle somewhere
    scheme->pageHit(5555);
    CHECK(scheme->useBit[5554] == false);
    CHECK(scheme->useBit[5555] == true);
    CHECK(scheme->useBit[5556] == false);

    // perform page hits on last frame of allocated memory
    scheme->pageHit(9999);
    CHECK(scheme->useBit[9997] == false);
    CHECK(scheme->useBit[9998] == false);
    CHECK(scheme->useBit[9999] == true);

    // hit all memory frames and ensure all are now true
    for (FrameNumber frame = 0; frame < 10000; frame++)
    {
      scheme->pageHit(frame);
    }

    for (FrameNumber frame = 0; frame < 10000; frame++)
    {
      CHECK(scheme->useBit[frame] == true);
    }
  }
}
#endif

/**
 * @brief Task 8: Clock makeReplacementDecision() member function tests
 */
#define task8
#ifdef task8
TEST_CASE("Task 8: test makeReplacementDecision() in Clock scheme memory size 3", "[task8]")
{
  // create a paging system that uses Clock page replacement with a memory size of 3
  PagingSystem sim(3, CLOCK);

  // get a handle on the private clock replacement scheme from the simulation so we
  // can test it
  // ClockPageReplacementScheme* scheme = (ClockPageReplacementScheme*)sim.scheme;

  // load simulation 01 and perform first 4 page references to get memory
  // initially loaded/full of pages
  sim.loadPageStream("simfiles/pageref-01.sim");

  // first 4 page references should cause 3 page placements (3 misses
  // and 1 hit) where we end up with a full memory
  for (int time = 0; time < 4; time++)
  {
    sim.processNextPageReference();
  }

  SECTION("Task 8: test stream 01 after initial page placements (clock)", "[task8]")
  {
    CHECK(sim.getSystemTime() == 4);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 2, 3, 1 ]");
  }

  // next page reference cause clock to make replacement decision, but should
  // select frame 0 here as all use bits are 1 and framePointer should point
  // to frame 0
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 01 time 4 page fault replaces frame 0", "[task8]")
  {
    CHECK(sim.getSystemTime() == 5);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 5, 3, 1 ]");
  }

  // frame 0 use bit is set, but frame 1 and 2 are not.  frame pointer points
  // to frame 1.  next reference is a miss so frame 1 is replace immediately
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 01 time 5 page fault replaces frame 1", "[task8]")
  {
    CHECK(sim.getSystemTime() == 6);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 5, 2, 1 ]");
  }

  // frame 0 and 1 use bit sets, frame 2 is not.  next refernce is a fault which
  // will replace frame 2
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 01 time 6 page fault replaces frame 2", "[task8]")
  {
    CHECK(sim.getSystemTime() == 7);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 5, 2, 4 ]");
  }

  // all use bits again set.  framePointer pointing to frame 0.  next reference is a hit
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 01 time 7 page hit occurs", "[task8]")
  {
    CHECK(sim.getSystemTime() == 8);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 5, 2, 4 ]");
  }

  // all use bits again set.  framePointer pointing to frame 0.  next reference is a fault so
  // all use bits flipped to false and frame 0 replaced again
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 01 time 8 page fault replaces frame 0", "[task8]")
  {
    CHECK(sim.getSystemTime() == 9);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 3, 2, 4 ]");
  }

  // frame 0 use bit set, but not frame 1 or 2.  Frame pointer points to frame 1
  // reference to page 2 is a hit
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 01 time 9 page hit on frame 1", "[task8]")
  {
    CHECK(sim.getSystemTime() == 10);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 3, 2, 4 ]");
  }

  // frame 0 and 1 have use bits set, but frame pointer points to frame 1 still
  // next reference to page 5 is a fault, replaces frame 2.  First difference
  // from fifo here
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 01 time 10 page fault replaces frame 2", "[task8]")
  {
    CHECK(sim.getSystemTime() == 11);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 3, 2, 5 ]");
  }

  // frame 0 and 2 have use bits set, frame pointer points to frame 0
  // next reference is a hit on frame 1 causing all use bits to be set again
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 01 time 11 page hit on frame 1", "[task8]")
  {
    CHECK(sim.getSystemTime() == 12);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 3, 2, 5 ]");
  }
}

TEST_CASE("Task 8: test makeReplacementDecision() in Clock scheme memory size 5", "[task8]")
{
  // create a paging system that uses Clock page replacement with a memory size of 5
  PagingSystem sim(5, CLOCK);

  // get a handle on the private clock replacement scheme from the simulation so we
  // can test it
  // ClockPageReplacementScheme* scheme = (ClockPageReplacementScheme*)sim.scheme;

  // load simulation 02 and perform first 6 page references to get memory
  // initially loaded/full of pages
  sim.loadPageStream("simfiles/pageref-02.sim");

  // first 4 page references should cause 3 page placements (3 misses
  // and 1 hit) where we end up with a full memory
  for (int time = 0; time < 7; time++)
  {
    sim.processNextPageReference();
  }

  SECTION("Task 8: test stream 02 after initial page placements (clock)", "[task8]")
  {
    CHECK(sim.getSystemTime() == 7);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 6, 5, 2, 4, 3 ]");
  }

  // next page reference is page hit on frame 1
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 02 time 7 page hit to frame 1", "[task8]")
  {
    CHECK(sim.getSystemTime() == 8);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 6, 5, 2, 4, 3 ]");
  }

  // next page reference is page hit to frame 4
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 02 time 8 page hit to frame 4", "[task8]")
  {
    CHECK(sim.getSystemTime() == 9);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 6, 5, 2, 4, 3 ]");
  }

  // next page reference is hit on frame 0
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 02 time 9 page hit to frame 0", "[task8]")
  {
    CHECK(sim.getSystemTime() == 10);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 6, 5, 2, 4, 3 ]");
  }

  // next page reference is fault replaces frame 0
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 02 time 10 page hit to frame 0", "[task8]")
  {
    CHECK(sim.getSystemTime() == 11);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 7, 5, 2, 4, 3 ]");
  }

  // next page reference is hit on frame 1
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 02 time 11 page hit to frame 0", "[task8]")
  {
    CHECK(sim.getSystemTime() == 12);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 7, 5, 2, 4, 3 ]");
  }

  // next page reference is hit on frame 4
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 02 time 12 page hit to frame 4", "[task8]")
  {
    CHECK(sim.getSystemTime() == 13);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 7, 5, 2, 4, 3 ]");
  }

  // next page reference is hit on frame 1
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 02 time 13 page hit to frame 1", "[task8]")
  {
    CHECK(sim.getSystemTime() == 14);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 7, 5, 2, 4, 3 ]");
  }

  // next page reference is hit on frame 3
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 02 time 14 page hit to frame 3", "[task8]")
  {
    CHECK(sim.getSystemTime() == 15);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 7, 5, 2, 4, 3 ]");
  }

  // next page reference is hit to frame 4
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 02 time 15 page hit to frame 4", "[task8]")
  {
    CHECK(sim.getSystemTime() == 16);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 7, 5, 2, 4, 3 ]");
  }

  // next page reference is hit to frame 3
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 02 time 16 page hit to frame 3", "[task8]")
  {
    CHECK(sim.getSystemTime() == 17);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 7, 5, 2, 4, 3 ]");
  }

  // next page reference is hit to frame 0
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 02 time 17 page hit to frame 0", "[task8]")
  {
    CHECK(sim.getSystemTime() == 18);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 7, 5, 2, 4, 3 ]");
  }

  // next page reference is hit to frame 1, all use bits set again and frame
  // pointer points to frame 1
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 02 time 18 page hit to frame 1", "[task8]")
  {
    CHECK(sim.getSystemTime() == 19);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 7, 5, 2, 4, 3 ]");
  }

  // next page reference is fault that replaces frame 1
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 02 time 19 page fault replaces frame 1", "[task8]")
  {
    CHECK(sim.getSystemTime() == 20);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 7, 1, 2, 4, 3 ]");
  }

  // next page reference is hit to frame 4
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 02 time 20 page hit on frame 4", "[task8]")
  {
    CHECK(sim.getSystemTime() == 21);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 7, 1, 2, 4, 3 ]");
  }

  // next page reference is hit on frame 0
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 02 time 21 page hit on frame 0", "[task8]")
  {
    CHECK(sim.getSystemTime() == 22);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 7, 1, 2, 4, 3 ]");
  }

  // next page reference is fault that replaces frame 2
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 02 time 22 page fault replaces frame 2", "[task8]")
  {
    CHECK(sim.getSystemTime() == 23);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 7, 1, 6, 4, 3 ]");
  }

  // next page reference hit on frame 3, all use bits set and frame pinter now at frame 3
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 02 time 23 page hit on frame 3", "[task8]")
  {
    CHECK(sim.getSystemTime() == 24);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 7, 1, 6, 4, 3 ]");
  }

  // next page reference is hit on frame 1
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 02 time 24 page hit on frame 1", "[task8]")
  {
    CHECK(sim.getSystemTime() == 25);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 7, 1, 6, 4, 3 ]");
  }

  // next page reference is fault that replaces frame 3
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 02 time 25 page fault replaces frame 3", "[task8]")
  {
    CHECK(sim.getSystemTime() == 26);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 7, 1, 6, 2, 3 ]");
  }

  // next page reference is hit on frame 1
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 02 time 26 page hit on frame 1", "[task8]")
  {
    CHECK(sim.getSystemTime() == 27);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 7, 1, 6, 2, 3 ]");
  }

  // next page reference is hit on frame 4
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 02 time 27 page hit on frame 4", "[task8]")
  {
    CHECK(sim.getSystemTime() == 28);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 7, 1, 6, 2, 3 ]");
  }

  // next page reference is fault that replaces frame 0 because frame 4 use bit
  // is 1 so it is skipped, and frame 0 use bit is 0
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 02 time 28 page fault that replaces frame 0", "[task8]")
  {
    CHECK(sim.getSystemTime() == 29);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 4, 1, 6, 2, 3 ]");
  }

  // next page reference is fault that replaces frame 2
  sim.processNextPageReference();
  SECTION("Task 8: (clock) test stream 02 time 29 page fault that replaces frame 2", "[task8]")
  {
    CHECK(sim.getSystemTime() == 30);
    CHECK(sim.isMemoryFull());
    CHECK(sim.memoryToString() == "[ 4, 1, 7, 2, 3 ]");
  }
}

/**
 * @brief Task 8: Test full simulation exectuon on the clock page
 *   replacement scheme
 */
TEST_CASE("Task 8: Test full clock simulation execution", "[task8]")
{
  SECTION("Task 8: test full clock simulation, sim 01 memory size 4", "[task8]")
  {
    // create a basic paging system simulation with 3 physical frames of
    // and load the basic page stream from simulation 01
    PagingSystem sim(4, CLOCK);

    // test that runSimulaiton() properly croaks if not page stream
    // has been loaded to simulate yet
    CHECK_THROWS_AS(sim.runSimulation(), SimulatorException);

    // load the simulation 01 and run the simulation to the end
    sim.loadPageStream("simfiles/pageref-01.sim");
    sim.runSimulation();
    CHECK(sim.memoryToString() == "[ 4, 3, 2, 5 ]");
  }

  SECTION("Task 8: test full clock simulation, sim 02 memory size 4", "[task8]")
  {
    // create a basic paging system simulation with 3 physical frames of
    // and load the basic page stream from simulation 01
    PagingSystem sim(4, CLOCK);

    // load the simulation 02 and run the simulation to the end
    sim.loadPageStream("simfiles/pageref-02.sim");
    sim.runSimulation();
    CHECK(sim.memoryToString() == "[ 2, 4, 3, 7 ]");
  }

  SECTION("Task 8: test full clock simulation, large random page stream memory size 20", "[task8]")
  {
    // create a basic paging system simulation with 3 physical frames of
    // and load the basic page stream from simulation 01
    PagingSystem sim(20, CLOCK);

    // create a long random simulation with a known seed and
    // ensure always end up with same memory contents using
    // clock replacement scheme
    sim.generateRandomPageStream(1000000, 100, 42);
    CHECK(sim.getNumPageReferences() == 1000000);
    sim.runSimulation();
    CHECK(sim.memoryToString() == "[ 30, 92, 76, 80, 95, 69, 84, 21, 56, 4, 16, 31, 20, 34, 96, 5, 1, 35, 36, 70 ]");
  }
}
#endif
