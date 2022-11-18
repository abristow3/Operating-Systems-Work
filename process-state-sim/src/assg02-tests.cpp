/** @file assg02-tests.cpp
 * @brief Unit tests for three-state process simulator.
 *
 * @author Student Name
 * @note   cwid: 123456
 * @date   Fall 2019
 * @note   ide:  VS Code Editor / IDE ; g++ 8.2.0 / GNU Make 4.2.1
 *
 * Unit tests for assignment 02, the three-state process simulator.
 * We test the needed functions to implement handling the
 * simulated events of our process simulator.
 */
#include "Process.hpp"
#include "ProcessSimulator.hpp"
#include "ProcessState.hpp"
#include "catch.hpp"
using namespace std;

/**
 * @brief test Process class operations.  These are general tests of the Process class
 *   that you will be using when implementing the ProcessSimulator member methods to
 *   manage processes in the simulated system.
 */
TEST_CASE("Task 0: Process constructor and support functions for process management", "[task0]")
{
  //----- test constructors
  // test an idle or unused process
  Process idle;
  CHECK(idle.isInState(IDLE, NEW, 0, 0, 0, NA_EVENT));

  // test basic Process constructor
  Process p1(1, 5);
  CHECK(p1.isInState(1, NEW, 5, 0, 0, NA_EVENT));
  Process p5(5, 25);
  CHECK(p5.isInState(5, NEW, 25, 0, 0, NA_EVENT));

  //----- test process simulator support functions
  // ready() puts process into a READY state in preparation
  // for being placed on ready queue by the simulator
  p1.ready();
  CHECK(p1.isInState(1, READY, 5, 0, 0, NA_EVENT));
  p5.ready();
  CHECK(p5.isInState(5, READY, 25, 0, 0, NA_EVENT));

  // an idle process will cause exception if we try and transition it with one
  // of the support functions
  CHECK_THROWS_AS(idle.ready(), SimulatorException);

  // likewise functions not in the state we expect should always cause
  // an exception to be thrown rather than doing the requested function
  CHECK_THROWS_AS(p1.ready(), SimulatorException);

  //-----
  // dispatch() make the process the running process
  p1.dispatch();
  CHECK(p1.isInState(1, RUNNING, 5, 0, 0, NA_EVENT));
  p5.dispatch();
  CHECK(p5.isInState(5, RUNNING, 25, 0, 0, NA_EVENT));

  CHECK_THROWS_AS(idle.dispatch(), SimulatorException);
  CHECK_THROWS_AS(p1.dispatch(), SimulatorException);

  //-----
  // cpuCycleProcess simulates the process running for 1 cpu cycle
  // run p1 for 3 quantums of time
  p1.cpuCycle();
  p1.cpuCycle();
  p1.cpuCycle();
  CHECK(p1.isInState(1, RUNNING, 5, 3, 3, NA_EVENT));

  // run p5 for 5 quantums of time
  p5.cpuCycle();
  p5.cpuCycle();
  p5.cpuCycle();
  p5.cpuCycle();
  p5.cpuCycle();
  CHECK(p5.isInState(5, RUNNING, 25, 5, 5, NA_EVENT));

  CHECK_THROWS_AS(idle.cpuCycle(), SimulatorException);

  //-----
  // isQuantumExceeded() tests if a running process has reached or exceeded
  // the time slice quantum limit yet.
  CHECK_FALSE(p1.isQuantumExceeded(5));
  CHECK(p5.isQuantumExceeded(5));

  CHECK_THROWS_AS(idle.isQuantumExceeded(5), SimulatorException);

  //-----
  // timeout() will put a running process back into a ready state
  p1.timeout();
  CHECK(p1.isInState(1, READY, 5, 3, 0, NA_EVENT));
  p5.timeout();
  CHECK(p5.isInState(5, READY, 25, 5, 0, NA_EVENT));

  // we did not test previous 2 functions throw exceptions for non RUNNING
  // processes, check that now.  Both cpuCycle() and isQuantumExceeded()
  // should throw an exception if asked to perform on a non-running process
  CHECK_THROWS_AS(p1.cpuCycle(), SimulatorException);
  CHECK_THROWS_AS(p5.isQuantumExceeded(5), SimulatorException);

  CHECK_THROWS_AS(idle.timeout(), SimulatorException);
  CHECK_THROWS_AS(p1.timeout(), SimulatorException);

  //-----
  // block() will block the running process and put it into the set of processes
  // waiting for an event to occur
  CHECK_THROWS_AS(idle.block(1), SimulatorException);
  // p1 is in ready state, so it can't block yet
  CHECK_THROWS_AS(p1.block(1), SimulatorException);

  // get p1 back to a running state
  p1.dispatch();
  p1.cpuCycle();
  p1.cpuCycle();
  CHECK(p1.isInState(1, RUNNING, 5, 5, 2, NA_EVENT));
  p1.block(1);
  CHECK(p1.isInState(1, BLOCKED, 5, 5, 0, 1));

  //-----
  // isWaitingOnEvent() test if the indicated process is waiting on the
  // indicated event id or not
  CHECK(p1.isWaitingOnEvent(1));
  CHECK(not p1.isWaitingOnEvent(2));
  CHECK_THROWS_AS(idle.isWaitingOnEvent(1), SimulatorException);
  CHECK_THROWS_AS(p5.isWaitingOnEvent(1), SimulatorException);

  //-----
  // unblock() unblock a blocked process and return it to the ready queue
  p1.unblock();
  CHECK(p1.isInState(1, READY, 5, 5, 0, NA_EVENT));
  CHECK_THROWS_AS(idle.unblock(), SimulatorException);
  CHECK_THROWS_AS(p5.unblock(), SimulatorException);
}

/**
 * @brief Task 1: ProcessSimulator initial state and getter accessor methods
 */
#define task1
#ifdef task1
TEST_CASE("Task 1: task 1 test case section", "[task1]")
{
  /// @brief We use this instantiation of the ProcessSimulator in all
  ///   subsequent test cases below for task 1 tests.
  ProcessSimulator sim(5);

  SECTION("Task 1: constructor and getter accessor methods", "[task1]")
  { // check that the initial state of a simulation is as expected
    CHECK(sim.getTimeSliceQuantum() == 5);
    CHECK(sim.getNextProcessId() == 1);
    CHECK(sim.getSystemTime() == 1);
  }

  SECTION("Task 1: process control block number of active and finished processes", "[task1]")
  {
    // process control block
    CHECK(sim.getNumActiveProcesses() == 0);
    CHECK(sim.getNumFinishedProcesses() == 0);
  }

  SECTION("Task 1: ready queue accessor methods", "[task1]")
  {
    // ready queue
    CHECK(sim.readyQueueSize() == 0);
    CHECK(sim.readyQueueFront() == IDLE);
    CHECK(sim.readyQueueBack() == IDLE);
  }

  SECTION("Task 1: blocked list accessor methods", "[task1]")
  {
    // blocked list
    CHECK(sim.blockedListSize() == 0);
  }

  SECTION("Task 1: initial simulation state", "[task1]")
  {
    // the isInState is a convenience method for unit testing, to test that
    // all of these important state values are as expected in the simulation
    // at any point
    CHECK(sim.isInState(5, 1, 0, 0, IDLE, 0, IDLE, IDLE, 0));
  }
}
#endif

/**
 * @brief Task 2: ProcessSimulator <newEvent()> member method tests
 */
TEST_CASE("Task 2: task 2 test case section", "[task2]")
{
#define task2_1
#ifdef task2_1

  /// @brief We use this instantiation of the ProcessSimulator in all
  ///   subsequent test cases below for task 2 tests.
  ProcessSimulator sim(5);
  // create a new process to test
  sim.newEvent();

  SECTION("Task 2: <newEvent()> check member variable and control block update", "[task2]")
  {
    // create a new process and test it is assigned pid 1, put in READY state and
    // put on the ready queue
    // sim.newEvent();

    // first of all, you need to be able to keep track of the next pid that needs
    // to be assigned.
    CHECK(sim.getNextProcessId() == 2);

    // We need a way to manage the processes we have in the system.  At a minimum
    // we should be able to determine the number of processes that are currently
    // being managed.
    CHECK(sim.getNumActiveProcesses() == 1);
  }
#endif

#define task2_2
#ifdef task2_2
  SECTION("Task 2: <getProcess()> check process put in processControlBlock", "[task2]")
  {
    // But more than that, you need to keep track of processes when a
    // process is created, you need a process control block, or a list
    // or array of processes.  The process that was just created should
    // be in a READY state and should have the systemTime set correctly
    // and the timeUsed and quantumUsed set to 0 to begin with.
    // If asked you need to be able to return the Proceess
    Process p1 = sim.getProcess(1); // get process Pid=1 from the simulation
    CHECK(p1.getPid() == 1);
  }

  SECTION("Task 2: <getProcess()> check process added to ready queue", "[task2]")
  {
    // this process should be in READY state, with a startTime of 1, etc.
    Process p1 = sim.getProcess(1); // get process Pid=1 from the simulation
    CHECK(p1.isInState(1, READY, 1, 0, 0, NA_EVENT));

    // In addition you now need to have a ready queue container working.  New
    // processes should be in the READY state, and need to be put onto
    // the back of the ready queue.
    CHECK(sim.readyQueueSize() == 1);
    CHECK(sim.readyQueueFront() == 1);
    CHECK(sim.readyQueueBack() == 1);
  }

  SECTION("Task 2: <newProcess()> and <getProcess()> add an additional process", "[task2]")
  {
    // now we add a second process.  This will ensure that we probably have a real
    // queue working for the ready queue at this point
    sim.newEvent();
    CHECK(sim.getNextProcessId() == 3);
    CHECK(sim.getNumActiveProcesses() == 2);
    CHECK(sim.readyQueueSize() == 2);
    CHECK(sim.readyQueueFront() == 1);
    CHECK(sim.readyQueueBack() == 2);

    Process p2 = sim.getProcess(2);
    CHECK(p2.isInState(2, READY, 1, 0, 0, NA_EVENT));

    // final check make sure all sim state is what we expect at this point
    CHECK(sim.isInState(5, 1, 2, 0, IDLE, 2, 1, 2, 0));
  }
#endif
}

/**
 * @brief Task 3: ProcessSimulator <dispatch()> member function
 */
TEST_CASE("Task 3: task 3 test case section", "[task3]")
{
#define task3_1
#ifdef task3_1

  /// @brief We use this instantiation of the ProcessSimulator in all
  ///   subsequent test cases below for task 2 tests.
  ProcessSimulator sim(5);
  // create two processes for task 3 testing to begin with
  sim.newEvent();
  sim.newEvent();

  SECTION("Task 3: cpu initialized <isCpuIdle> and <runningProcess> created", "[task3]")
  {
    // dispatch() should cause process at front of the ready queue to become the
    // running process.  Lots of things happen with this first dispatch.
    // Pid 1 changes to be RUNNING.  The runningProcess is pid 1.  The ready queue
    // now only has 1 process on it.  You should work on all of these one by one.
    // First of all, before we dispatch, lets check that isCpuIdle() and
    // runningProcess() are both working. The cpu should currently be idle
    CHECK(sim.isCpuIdle());
    CHECK(sim.runningProcess() == IDLE);
  }
#endif

#define task3_2
#ifdef task3_2
  // now try to dispatch()
  sim.dispatch();

  SECTION("Task 3: <dispatch()> gets process when cpu idle and process on queue", "[task3]")
  {
    // first, was the cpu updated correctly
    CHECK_FALSE(sim.isCpuIdle());
    CHECK(sim.runningProcess() == 1);

    // next does the ready queue look correct
    CHECK(sim.readyQueueSize() == 1);
    CHECK(sim.readyQueueFront() == 2);
    CHECK(sim.readyQueueBack() == 2);

    CHECK(sim.isInState(5, 1, 2, 0, 1, 1, 2, 2, 0));
  }

  SECTION("Task 3: <dispatch()> correctly sets dispatched process to running", "[task3]")
  {
    // check that the process that is running was correctly put into the running state
    Pid pid = sim.runningProcess();
    Process p = sim.getProcess(pid);
    CHECK(p.isInState(1, RUNNING, 1, 0, 0, NA_EVENT));
  }

  // a process is running now, so dispatch should not cause any change
  sim.dispatch();

  SECTION("Task 3: <dispatch()> dispatch does nothing when cpu is running", "[task3]")
  {
    CHECK_FALSE(sim.isCpuIdle());
    CHECK(sim.runningProcess() == 1);
    CHECK(sim.readyQueueSize() == 1);
    CHECK(sim.readyQueueFront() == 2);
    CHECK(sim.readyQueueBack() == 2);
    CHECK(sim.isInState(5, 1, 2, 0, 1, 1, 2, 2, 0));
  }

  SECTION("Task 3: <dispatch()> cpu still idle if dispatch when queue is empty", "[task3]")
  {
    // finally we will check this more later, but if the ready queue
    // is empty then an idle cpu will remain idle.  We will create another
    // simulation to check this
    ProcessSimulator sim2(8);
    CHECK(sim2.isInState(8, 1, 0, 0, IDLE, 0, IDLE, IDLE, 0));
    sim2.dispatch();
    CHECK(sim2.isCpuIdle());
    CHECK(sim2.runningProcess() == IDLE);
    CHECK(sim2.readyQueueSize() == 0);
    CHECK(sim2.readyQueueFront() == IDLE);
    CHECK(sim2.readyQueueBack() == IDLE);
    CHECK(sim2.isInState(8, 1, 0, 0, IDLE, 0, IDLE, IDLE, 0));
  }
#endif
}

/**
 * @brief Task 4: ProcessSimulator cpu event initial tests.  We just test
 * that time used and quantum used is being incremented.  Later
 * tests will test other aspects of this function.
 */
#define task4
#ifdef task4
TEST_CASE("Task 4: task 4 test case section", "[task4]")
{
  /// @brief We use this instantiation of the ProcessSimulator in all
  ///   subsequent test cases below for task 4 tests.
  ProcessSimulator sim(5);

  // create two processes for task 4 testing to begin with
  sim.newEvent();
  sim.newEvent();

  // this test case also expects dispatch() has been called 2 times
  sim.dispatch();
  sim.dispatch();

  // in sim Pid 1 is currently running.  Cause it to run 3 cpu cycles
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();

  SECTION("Task 4: <cpuEvent()> test 3 cpu cycles", "[task4]")
  {
    // simulation is still in same state
    CHECK(sim.getSystemTime() == 4);
    CHECK(sim.isInState(5, 4, 2, 0, 1, 1, 2, 2, 0));
  }

  SECTION("Task 4: <cpuEvent()> check process did cpu cycles", "[task4]")
  {
    // but process should now have 3 time cycles and 3 quantums used
    Process p1 = sim.getProcess(1);
    CHECK(not p1.isQuantumExceeded(5));
    CHECK(p1.isInState(1, RUNNING, 1, 3, 3, NA_EVENT));
  }

  // 3 more cpu cycles
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();

  SECTION("Task 4: <cpuEvent()> test 3 additional cpu cycles", "[task4]")
  {
    CHECK(sim.getSystemTime() == 7);
    CHECK(sim.isInState(5, 7, 2, 0, 1, 1, 2, 2, 0));

    // notice p1 has now run 6 time slices
    Process p1 = sim.getProcess(1);
    CHECK(p1.isQuantumExceeded(5));
    CHECK(p1.isInState(1, RUNNING, 1, 6, 6, NA_EVENT));
  }
}
#endif

/**
 * @brief Task 5: test ProcessSimulator <timeout()> member function event
 */
#define task5
#ifdef task5
TEST_CASE("Task 5: task 5 test case section", "[task5]")
{
  /// @brief We use this instantiation of the ProcessSimulator in all
  ///   subsequent test cases below for task 4 tests.
  ProcessSimulator sim(5);

  // create two processes for task 4 testing to begin with
  sim.newEvent();
  sim.newEvent();

  // this test case also expects dispatch() has been called 2 times
  sim.dispatch();
  sim.dispatch();

  // in sim Pid 1 is currently running.  Before starting task 5, these tests
  // assume that 6 cpu event cycles have occurred, and the running processes
  // has exceeded its time slice quantum
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();

  // the running process has currently exceeded its time quantum, time it out
  sim.timeout();

  SECTION("Task 5: <timeout()> basic test of process timeout ", "[task5]")
  {
    Process p1 = sim.getProcess(1);
    CHECK(p1.isInState(1, READY, 1, 6, 0, NA_EVENT));
    CHECK(sim.isCpuIdle());
    CHECK(sim.isInState(5, 7, 2, 0, IDLE, 2, 2, 1, 0));
  }

  // timeout when cpu is idle should do nothing
  sim.timeout();

  SECTION("Task 5: <timeout()> timeout when idle does nothing ", "[task5]")
  {
    Process p1 = sim.getProcess(1);
    CHECK(p1.isInState(1, READY, 1, 6, 0, NA_EVENT));
    CHECK(sim.isCpuIdle());
    CHECK(sim.isInState(5, 7, 2, 0, IDLE, 2, 2, 1, 0));
  }

  // cpu is idle and ready queue is not empty, so we can test
  // dispatch again, should dispatch Pid 2 now since it is as
  // the front of the queue
  sim.dispatch();

  SECTION("Task 5: <timeout()> dispatch process 2 and test doesn't time out ", "[task5]")
  {
    Process p2 = sim.getProcess(2);
    CHECK(p2.isInState(2, RUNNING, 1, 0, 0, NA_EVENT));
    CHECK(not sim.isCpuIdle());
    CHECK(sim.isInState(5, 7, 2, 0, 2, 1, 1, 1, 0));
  }

  // have pid 2 perform 4 cpu cycles
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();

  SECTION("Task 5: <timeout()>  process 2 run 4 cycles, not exceeded time quantum ", "[task5]")
  {
    Process p2 = sim.getProcess(2);
    CHECK(not p2.isQuantumExceeded(5));
    CHECK(p2.isInState(2, RUNNING, 1, 4, 4, NA_EVENT));

    // pid 2 has not yet exceeded its time slice quantum, so timeout should not
    // have any effect at this point
    sim.timeout();
    CHECK(sim.isInState(5, 11, 2, 0, 2, 1, 1, 1, 0));
  }

  // simulate the typical steps we need before and after a cpu cycle
  // dispatch should have no effect since cpu is not idle
  sim.dispatch();

  SECTION("Task 5: <timeout()> dispatch has no effect yet ", "[task5]")
  {
    CHECK(sim.isInState(5, 11, 2, 0, 2, 1, 1, 1, 0));
  }

  // run 5th cpu cycle for pid 2 which will now have reached its time slice
  // quantum
  sim.cpuEvent();

  SECTION("Task 5: <timeout()> cpu event before trying timeout of process 2 ", "[task5]")
  {
    Process p2 = sim.getProcess(2);
    CHECK(p2.isQuantumExceeded(5));
    CHECK(p2.isInState(2, RUNNING, 1, 5, 5, NA_EVENT));
  }

  // and a final test of timeout, pid 2 has reached its quantum so it should
  // now time out
  sim.timeout();

  SECTION("Task 5: <timeout()> timeout process 2 ", "[task5]")
  {
    CHECK(sim.isInState(5, 12, 2, 0, IDLE, 2, 1, 2, 0));
    Process p2 = sim.getProcess(2);
    CHECK(p2.isInState(2, READY, 1, 5, 0, NA_EVENT));
  }

  /**
   * @brief Task 5: test ProcessSimulator test of dispatch/cpu/timeout processing. We are
   * working up towards a full system test.  Simulate 10 or so cpu cycles with
   * the 2 processes.  Current state of sim is 2 processes with both in the
   * ready queue and pid 1 is at the head of the queue.
   */
  // before we begin lets add 1 more process, pid 3, to end of ready queue
  sim.newEvent();

  SECTION("Task 5: dispatch/cpu/timeout system cycles", "[task5]")
  {
    Process p3 = sim.getProcess(3);
    CHECK(p3.isInState(3, READY, 12, 0, 0, NA_EVENT));
    CHECK(sim.isInState(5, 12, 3, 0, IDLE, 3, 1, 3, 0));

    // we simulate 5 time steps (a time slice quantum).  During these
    // cycles Pid 1 should be dispatched, run all 5 of its quantums, and then
    // timeout back to the tail of the ready queue
    Process p1;
    CHECK(sim.isCpuIdle());
    for (int time = 1; time <= 5; time++)
    {
      // first check if we can dispatch a process if the cpu is idle
      sim.dispatch();
      CHECK(not sim.isCpuIdle());
      CHECK(sim.runningProcess() == 1);

      // execute a cpu cycle on the running process
      sim.cpuEvent();
      p1 = sim.getProcess(1);
      CHECK(p1.isInState(1, RUNNING, 1, 6 + time, time, NA_EVENT));

      // now see if process should time out
      // nothing will happen for time 1-4, but at time 5 pid 1 should time out
      sim.timeout();
    }

    // the last timeout of the loop should have caused pid 1 to timeout and be
    // returned to the ready queue, ready queue now has Front: 2 3 1 :Back
    p1 = sim.getProcess(1);
    CHECK(p1.isInState(1, READY, 1, 11, 0, NA_EVENT));
    CHECK(sim.isInState(5, 17, 3, 0, IDLE, 3, 2, 1, 0));

    // simulate 5 more time steps in which pid 2 will be dispatched, run its
    // scheduled time slice quantums, and then time out
    Process p2;
    CHECK(sim.isCpuIdle());
    for (int time = 1; time <= 5; time++)
    {
      // first check if we can dispatch a process if the cpu is idle
      sim.dispatch();
      CHECK(not sim.isCpuIdle());
      CHECK(sim.runningProcess() == 2);

      // execute a cpu cycle on the running process
      sim.cpuEvent();
      p2 = sim.getProcess(2);
      CHECK(p2.isInState(2, RUNNING, 1, 5 + time, time, NA_EVENT));

      // now see if process should time out
      // nothing will happen for time 1-4, but at time 5 pid 2 should time out
      sim.timeout();
    }

    // the last timeout of the loop should have caused pid 2 to timeout and be
    // returned to the ready queue, ready queue now has Front: 3 1 2 :Back
    p2 = sim.getProcess(2);
    CHECK(p2.isInState(2, READY, 1, 10, 0, NA_EVENT));
    CHECK(sim.isInState(5, 22, 3, 0, IDLE, 3, 3, 2, 0));

    // and finally, cause pid 3 to be dispatched and run 1 cpu cycle
    sim.dispatch();
    sim.cpuEvent();
    sim.timeout();
    p3 = sim.getProcess(3);
    CHECK(p3.isInState(3, RUNNING, 12, 1, 1, NA_EVENT));
    CHECK(sim.isInState(5, 23, 3, 0, 3, 2, 1, 2, 0));
  }
}
#endif

/**
 * @brief Task 6: test ProcessSimulator <blockEvent()> member function
 */
#define task6
#ifdef task6
TEST_CASE("Task 6: task 6 test case section", "[task6]")
{
  /// @brief We use this instantiation of the ProcessSimulator in all
  ///   subsequent test cases below for task 6 tests.
  ProcessSimulator sim(5);

  // task 6 expectes/requires all of the following to have occured in the
  // simulation before the tests begin.
  sim.newEvent();
  sim.newEvent();
  sim.dispatch();
  sim.dispatch();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.timeout();
  sim.timeout();
  sim.dispatch();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.dispatch();
  sim.cpuEvent();
  sim.timeout();
  sim.newEvent();

  for (int time = 1; time <= 10; time++)
  {
    sim.dispatch();
    sim.cpuEvent();
    sim.timeout();
  }

  sim.dispatch();
  sim.cpuEvent();
  sim.timeout();
  Process p3 = sim.getProcess(3);
  CHECK(p3.isInState(3, RUNNING, 12, 1, 1, NA_EVENT));
  CHECK(sim.isInState(5, 23, 3, 0, 3, 2, 1, 2, 0));

  // in sim, p3 is currently running, cause it to become blocked on eventId 7
  sim.blockEvent(7);

  SECTION("Task 6: <blockEvent()> basic block", "[task6]")
  {
    Process p3 = sim.getProcess(3);
    CHECK(p3.isWaitingOnEvent(7));
    CHECK(p3.isInState(3, BLOCKED, 12, 1, 0, 7));
    CHECK(sim.isCpuIdle());
    CHECK(sim.isInState(5, 23, 3, 0, IDLE, 2, 1, 2, 1));
  }

  SECTION("Task 6: <blockEvent()> block when cpu idle is error", "[task6]")
  {
    // in this simulation it doesn't make sense to receive a simulation of a
    // blocking event if no process is running.  The blockEvent() process should
    // throw an exception if a blockEvent() is attempted when the cpu is idle. The
    // cpu should currently
    CHECK_THROWS_AS(sim.blockEvent(5), SimulatorException);
  }

  // dispatch another process, process 1 should be at head of queue.
  sim.dispatch();
  sim.cpuEvent();
  sim.cpuEvent();

  SECTION("Task 6: <blockEvent()> setup to block process 1 now", "[task6]")
  {
    Process p1 = sim.getProcess(1);
    CHECK(p1.isInState(1, RUNNING, 1, 13, 2, NA_EVENT));
  }

  // block pid 1
  sim.blockEvent(23);

  SECTION("Task 6: <blockEvent()> block process 1 now", "[task6]")
  {
    Process p1 = sim.getProcess(1);
    CHECK(p1.isInState(1, BLOCKED, 1, 13, 0, 23));
    CHECK(sim.isInState(5, 25, 3, 0, IDLE, 1, 2, 2, 2));
  }

  SECTION("Task 6: <blockEvent()> block when idle again", "[task6]")
  {
    // in this simulation it doesn't make sense to receive a simulation of a
    // blocking event if no process is running.  The blockEvent() process should
    // throw an exception if a blockEvent() is attempted when the cpu is idle. The
    // cpu should currently
    CHECK_THROWS_AS(sim.blockEvent(5), SimulatorException);
  }

  // likewise to simplify things a bit we only allow 1 process to be blocked on
  // a given eventId event type at any time.  the blockEvent() should throw
  // an exception if another process is already blocked waiting on the indicated
  // eventId
  // First we need to dispatch a ready process so we can attempt a bad
  // blockEvent
  sim.dispatch();
  sim.cpuEvent();
  sim.cpuEvent();

  SECTION("Task 6: <blockEvent()> two processes blocked on same event not allowed", "[task6]")
  {
    Process p2 = sim.getProcess(2);
    CHECK(p2.isInState(2, RUNNING, 1, 12, 2, NA_EVENT));

    // now p2 is running, try and block it on eventId 23, which p1 is already
    // blocked and waiting on
    CHECK_THROWS_AS(sim.blockEvent(23), SimulatorException);
  }
}
#endif

/**
 * @brief Task 7: test ProcessSimulator <unblockEvent()> member function
 */
#define task7
#ifdef task7
TEST_CASE("Task 7: task 7 test case section", "[task7]")
{
  /// @brief We use this instantiation of the ProcessSimulator in all
  ///   subsequent test cases below for task 7 tests.
  ProcessSimulator sim(5);

  // task 7 expectes/requires all of the following to have occured in the
  // simulation before the tests begin.
  sim.newEvent();
  sim.newEvent();
  sim.dispatch();
  sim.dispatch();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.timeout();
  sim.timeout();
  sim.dispatch();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.dispatch();
  sim.cpuEvent();
  sim.timeout();
  sim.newEvent();

  for (int time = 1; time <= 10; time++)
  {
    sim.dispatch();
    sim.cpuEvent();
    sim.timeout();
  }

  sim.dispatch();
  sim.cpuEvent();
  sim.timeout();
  Process p3 = sim.getProcess(3);
  CHECK(p3.isInState(3, RUNNING, 12, 1, 1, NA_EVENT));
  CHECK(sim.isInState(5, 23, 3, 0, 3, 2, 1, 2, 0));

  // in sim, p3 is currently running, cause it to become blocked on eventId 7
  sim.blockEvent(7);

  sim.dispatch();
  sim.cpuEvent();
  sim.cpuEvent();
  // block pid 1
  sim.blockEvent(23);

  sim.dispatch();
  sim.cpuEvent();
  sim.cpuEvent();

  SECTION("Task 7: <unblockEvent()> error if unblock occurs and no process is waiting on event", "[task7]")
  {
    CHECK_THROWS_AS(sim.unblockEvent(1), SimulatorException);
    CHECK_THROWS_AS(sim.unblockEvent(15), SimulatorException);
  }

  SECTION("Task 7: <unblockEvent()> setup firs unblock of process 3", "[task7]")
  {
    // ready queue is empty at this point, p2 is running, p1 is blocked on eventId
    // 23 and p3 is blocked on eventId 7.  Just confirm current system state is as
    // we expect
    CHECK(sim.isInState(5, 27, 3, 0, 2, 0, IDLE, IDLE, 2));
  }

  // unblock pid 3 first
  sim.unblockEvent(7);

  SECTION("Task 7: <unblockEvent()> test basic unblock of process 3", "[task7]")
  {
    Process p3 = sim.getProcess(3);
    CHECK(p3.isInState(3, READY, 12, 1, 0, NA_EVENT));
  }

  // unblock process 1
  sim.unblockEvent(23);

  SECTION("Task 7: <unblockEvent()> test basic unblock of process 1", "[task7]")
  {
    Process p1 = sim.getProcess(1);
    CHECK(p1.isInState(1, READY, 1, 13, 0, NA_EVENT));
  }

  // block and unblock pid 2
  sim.blockEvent(16);
  SECTION("Task 7: <unblockEvent()> block  process 2", "[task7]")
  {
    Process p2 = sim.getProcess(2);
    CHECK(p2.isInState(2, BLOCKED, 1, 12, 0, 16));
  }

  sim.unblockEvent(16);
  SECTION("Task 7: <unblockEvent()>  unblock process 2", "[task7]")
  {
    Process p2 = sim.getProcess(2);
    CHECK(p2.isInState(2, READY, 1, 12, 0, NA_EVENT));

    CHECK(sim.isInState(5, 27, 3, 0, IDLE, 3, 3, 2, 0));
  }
}
#endif

/**
 * @brief Task 8: ProcessSimulator <doneEvent()> member function tests
 * system.
 */
#define task8
#ifdef task8
TEST_CASE("Task 8: task 8 test case section", "[task8]")
{
  /// @brief We use this instantiation of the ProcessSimulator in all
  ///   subsequent test cases below for task 8 tests.
  ProcessSimulator sim(5);

  // task 8 expectes/requires all of the following to have occured in the
  // simulation before the tests begin.
  sim.newEvent();
  sim.newEvent();
  sim.dispatch();
  sim.dispatch();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.timeout();
  sim.timeout();
  sim.dispatch();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.dispatch();
  sim.cpuEvent();
  sim.timeout();
  sim.newEvent();

  for (int time = 1; time <= 10; time++)
  {
    sim.dispatch();
    sim.cpuEvent();
    sim.timeout();
  }

  sim.dispatch();
  sim.cpuEvent();
  sim.timeout();
  Process p3 = sim.getProcess(3);
  CHECK(p3.isInState(3, RUNNING, 12, 1, 1, NA_EVENT));
  CHECK(sim.isInState(5, 23, 3, 0, 3, 2, 1, 2, 0));

  // in sim, p3 is currently running, cause it to become blocked on eventId 7
  sim.blockEvent(7);

  sim.dispatch();
  sim.cpuEvent();
  sim.cpuEvent();
  // block pid 1
  sim.blockEvent(23);

  sim.dispatch();
  sim.cpuEvent();
  sim.cpuEvent();

  sim.unblockEvent(7);
  sim.unblockEvent(23);
  sim.blockEvent(16);
  sim.unblockEvent(16);

  // no process is currently running, lets dispatch a process and finish it off
  sim.dispatch();
  sim.cpuEvent();
  sim.cpuEvent();

  SECTION("Task 8: <doneEvent()> process 3", "[task8]")
  {
    CHECK(sim.isInState(5, 29, 3, 0, 3, 2, 1, 2, 0));
  }

  // finish process 3
  sim.doneEvent();

  SECTION("Task 8: <doneEvent()> finish process 3", "[task8]")
  {
    CHECK(sim.isInState(5, 29, 2, 1, IDLE, 2, 1, 2, 0));
  }

  // lets finish a second process
  sim.dispatch();
  sim.cpuEvent();
  sim.cpuEvent();
  sim.cpuEvent();

  SECTION("Task 8: <doneEvent()> dispatch process 1", "[task8]")
  {
    CHECK(sim.isInState(5, 32, 2, 1, 1, 1, 2, 2, 0));
  }

  // finish process 1
  sim.doneEvent();

  SECTION("Task 8: <doneEvent()>  finish process 1", "[task8]")
  {
    CHECK(sim.isInState(5, 32, 1, 2, IDLE, 1, 2, 2, 0));
  }

  SECTION("Task 8: <doneEvent()> error for done to occur when cpu idle", "[task8]")
  {
    // if cpu is idle then doneEvent() does not make sense.  simulator should
    // throw an exception if we attempt to perform done when no process is running
    CHECK(sim.isCpuIdle());
    CHECK_THROWS_AS(sim.doneEvent(), SimulatorException);
  }
}
#endif
