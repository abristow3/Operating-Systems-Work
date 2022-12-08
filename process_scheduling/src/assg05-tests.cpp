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
// kludge so we can look inside of private SchedulingSystem things to do testing
#define private public

#include "SchedulingSystem.hpp"
#include "SimulatorException.hpp"
#include "catch.hpp"

using namespace std;

/**
 * @brief Initial tests of SchedulingSystem process table loading and creation
 */
TEST_CASE("Setup 1: Initial tests of loading and constructing process table", "[setup1]")
{
  // a basic scheduling system simulator that defaults to FCFS scheduling policy
  SchedulingSystem sim;

  // test bad file name exceptions
  CHECK_THROWS_AS(sim.loadProcessTable("simfiles/bogus.sim"), SimulatorException);

  // test loading simulated page reference streams from file
  sim.loadProcessTable("simfiles/process-table-01.sim");
  CHECK(sim.processTableToString() == "[ (A 0 3) (B 2 6) (C 4 4) (D 6 5) (E 8 2) ]");

  sim.loadProcessTable("simfiles/process-table-02.sim");
  CHECK(sim.processTableToString() == "[ (A 0 3) (B 1 5) (C 3 2) (D 9 5) (E 12 5) ]");

  sim.generateRandomProcessTable(10, 0.33, 10, 42);
  CHECK(sim.processTableToString() == "[ (P0 0 1) (P1 3 9) (P2 6 4) (P3 9 5) (P4 10 3) (P5 11 9) (P6 15 10) "
                                      "(P7 23 8) (P8 26 1) (P9 27 2) ]");
}

/**
 * @brief Task 1: Implement basic SchedulingSystem accessor methods
 */
#define task1_1
#ifdef task1_1
TEST_CASE("Task 1: implement getSystemTime() accessor method", "[task1]")
{
  SECTION("Task 1: system time is initialized to 0", "[task1]")
  {
    SchedulingSystem sim;

    CHECK(sim.getSystemTime() == 0);
  }
}
#endif

#define task1_2
#ifdef task1_2
TEST_CASE("Task 1: implement getNumProcesses() accessor method", "[task1]")
{
  SECTION("Task 1: number of processes is initialized to 0", "[task1]")
  {
    SchedulingSystem sim;
    CHECK(sim.getNumProcesses() == 0);
  }

  SECTION("Task 1: number of processes in table 01 is 5", "[task1]")
  {
    SchedulingSystem sim;
    sim.loadProcessTable("simfiles/process-table-01.sim");
    CHECK(sim.getNumProcesses() == 5);
  }

  SECTION("Task 1: number of processes in table 02 is 5", "[task1]")
  {
    SchedulingSystem sim;
    sim.loadProcessTable("simfiles/process-table-02.sim");
    CHECK(sim.getNumProcesses() == 5);
  }

  SECTION("Task 1: randomly generate 10 processes", "[task1]")
  {
    SchedulingSystem sim;
    sim.generateRandomProcessTable(10, 0.33, 10, 42);
    CHECK(sim.getNumProcesses() == 10);
  }
}
#endif

#define task1_3
#ifdef task1_3
TEST_CASE("Task 1: implement isCpuIdle() accessor method", "[task1]")
{
  SECTION("Task 1: cpu is initially IDLE in simulations ", "[task1]")
  {
    SchedulingSystem sim;

    CHECK(sim.isCpuIdle());
  }
}
#endif

#define task1_4
#ifdef task1_4
TEST_CASE("Task 1: implement getRunningProcessName() accessor method", "[task1]")
{
  SECTION("Task 1: cpu is initially IDLE", "[task1]")
  {
    SchedulingSystem sim;

    CHECK(sim.getRunningProcessName() == "IDLE");
  }
}
#endif

/**
 * @brief Task 2: Implement allProcessesDone() member method
 */
#define task2
#ifdef task2
TEST_CASE("Task 2: Implement allProcessesDone() member method", "[task2]")
{
  SECTION("Task 2: initially all processes are not yet done", "[task2]")
  {
    SchedulingSystem sim;
    sim.loadProcessTable("simfiles/process-table-01.sim");
    CHECK_FALSE(sim.allProcessesDone());
  }

  SECTION("Task 2: even if some processes are finished, still not all done", "[task2]")
  {
    SchedulingSystem sim;
    sim.loadProcessTable("simfiles/process-table-01.sim");
    sim.process[0].done = true;
    sim.process[2].done = true;
    sim.process[3].done = true;
    CHECK_FALSE(sim.allProcessesDone());
  }

  SECTION("Task 2: but when all processes are done, should detect", "[task2]")
  {
    SchedulingSystem sim;
    sim.loadProcessTable("simfiles/process-table-01.sim");
    sim.process[0].done = true;
    sim.process[1].done = true;
    sim.process[2].done = true;
    sim.process[3].done = true;
    sim.process[4].done = true;
    CHECK(sim.allProcessesDone());
  }
}
#endif

/**
 * @brief Task 3: Implement dispatchCpuIfIdle() member method
 */
#define task3
#ifdef task3
TEST_CASE("Task 3: Implement dispatchCpuIfIdle() member method", "[task3]")
{
  SECTION("Task 3: test dispatch with FCFS default policy", "[task3]")
  {
    // a basic scheduling system simulator that defaults to using FCFS
    // scheduling policy
    SchedulingSystem sim;

    // load a simulation process table from file
    sim.loadProcessTable("simfiles/process-table-01.sim");
    CHECK(sim.getSystemTime() == 0);
    CHECK(sim.isCpuIdle());
    CHECK(sim.getRunningProcessName() == "IDLE");
    CHECK(sim.getNumProcesses() == 5);
    CHECK_FALSE(sim.allProcessesDone());
    CHECK(sim.processTableToString() == "[ (A 0 3) (B 2 6) (C 4 4) (D 6 5) (E 8 2) ]");

    // dispatch the first process, incidentally we can now check isCpuIdle and
    // getRunningProcessName more thoroughly
    sim.checkProcessArrivals();
    CHECK(sim.process[0].startTime == NOT_STARTED);
    sim.dispatchCpuIfIdle();
    CHECK_FALSE(sim.isCpuIdle());
    CHECK(sim.getRunningProcessName() == "A");
    CHECK(sim.process[0].startTime == 0);
  }

  SECTION("Task 3: dispatch called when cpu is not idle should do nothing", "[task3]")
  {
    // a basic scheduling system simulator that defaults to using FCFS
    // scheduling policy
    SchedulingSystem sim;

    // load a simulation process table from file
    sim.loadProcessTable("simfiles/process-table-01.sim");

    sim.checkProcessArrivals();
    CHECK(sim.process[0].startTime == NOT_STARTED);
    sim.dispatchCpuIfIdle();
    CHECK_FALSE(sim.isCpuIdle());
    CHECK(sim.getRunningProcessName() == "A");
    CHECK(sim.process[0].startTime == 0);

    sim.dispatchCpuIfIdle();
    CHECK_FALSE(sim.isCpuIdle());
    CHECK(sim.getRunningProcessName() == "A");
    CHECK(sim.process[0].startTime == 0);
  }

  SECTION("Task 3: make cpu idle and dispatch another process to test dispatching", "[task3]")
  {
    // a basic scheduling system simulator that defaults to using FCFS
    // scheduling policy
    SchedulingSystem sim;

    // load a simulation process table from file
    sim.loadProcessTable("simfiles/process-table-01.sim");

    sim.checkProcessArrivals();
    CHECK(sim.process[0].startTime == NOT_STARTED);
    sim.dispatchCpuIfIdle();
    CHECK_FALSE(sim.isCpuIdle());
    CHECK(sim.getRunningProcessName() == "A");
    CHECK(sim.process[0].startTime == 0);

    sim.dispatchCpuIfIdle();
    CHECK_FALSE(sim.isCpuIdle());
    CHECK(sim.getRunningProcessName() == "A");

    sim.cpu = IDLE;
    CHECK(sim.isCpuIdle());
    // at time 2 process B arrives
    sim.systemTime = 2;
    sim.checkProcessArrivals();
    CHECK(sim.process[1].startTime == NOT_STARTED);
    sim.dispatchCpuIfIdle();
    CHECK_FALSE(sim.isCpuIdle());
    CHECK(sim.getRunningProcessName() == "B");
    CHECK(sim.process[1].startTime == 2);
  }

  SECTION("Task 3: check start time recorded correctly if process dispatched "
          "multiple times",
    "[task3]")
  {
    // a basic scheduling system simulator that defaults to using FCFS
    // scheduling policy
    SchedulingSystem sim;

    // load a simulation process table from file
    sim.loadProcessTable("simfiles/process-table-01.sim");

    // process A arraves at time 0
    sim.checkProcessArrivals();
    // artifically set system time to 2 and dispatch the process
    sim.systemTime = 2;
    sim.dispatchCpuIfIdle();
    CHECK_FALSE(sim.isCpuIdle());
    CHECK(sim.getRunningProcessName() == "A");
    CHECK(sim.process[0].startTime == 2);

    // artifically make cpu idle, and have process A arrive again, but set
    // system time to 4
    sim.cpu = IDLE;
    sim.systemTime = 0;
    sim.checkProcessArrivals();
    // artifically set system time to 4 and dispatch the process a second time
    sim.systemTime = 4;
    sim.dispatchCpuIfIdle();
    CHECK_FALSE(sim.isCpuIdle());
    CHECK(sim.getRunningProcessName() == "A");
    // startTime should only be updated on first dispatch
    CHECK(sim.process[0].startTime == 2);
  }
}
#endif

/**
 * @brief Task 4: Implement checkProcessFinished() member method
 */
#define task4
#ifdef task4
/**
 * @brief Task 4: Test detecting process finished.  With this method,
 *   we can test full cycles of FCFS policy, as we have all
 *   of new arrivals, dispatching, simulating cpu and finishing
 *   processes done.
 */
TEST_CASE("Task 4: Implement checkProcessFinished() member method, final test", "[task4]")
{
  SECTION("Task 4: test that process A is finished correctly for table 01 FCFS", "[task4]")
  {
    // a basic scheduling system simulator that defaults to using FCFS
    // scheduling policy
    SchedulingSystem sim;

    // load a simulation process table from file
    sim.loadProcessTable("simfiles/process-table-01.sim");
    CHECK(sim.getSystemTime() == 0);
    CHECK(sim.isCpuIdle());
    CHECK(sim.getRunningProcessName() == "IDLE");
    CHECK(sim.getNumProcesses() == 5);

    // check new arrivals and dispatch, so that first process A is
    // scheduled to run on the CPU
    sim.checkProcessArrivals();
    sim.dispatchCpuIfIdle();

    CHECK_FALSE(sim.isCpuIdle());
    CHECK(sim.getRunningProcessName() == "A");

    // process A has a service time of 3, so if we run 3 cycles, the
    // system should end up with A finished and the cpu idle again
    for (int time = 0; time < 3; time++)
    {
      sim.checkProcessArrivals();
      sim.dispatchCpuIfIdle();
      sim.simulateCpuCycle();
    }
    // process should still be on cpu untill we call checkProcessFinished
    CHECK(sim.getSystemTime() == 3);
    CHECK_FALSE(sim.isCpuIdle());
    CHECK(sim.getRunningProcessName() == "A");

    // call checkProcessFinished to finish off process A
    sim.checkProcessFinished();
    CHECK(sim.isCpuIdle());
    CHECK(sim.getRunningProcessName() == "IDLE");
    CHECK(sim.process[0].startTime == 0);
    CHECK(sim.process[0].endTime == 3);
    CHECK(sim.process[0].done);
  }

  SECTION("Task 4: test that process B is finished correctly for table 01 FCFS", "[task4]")
  {
    // a basic scheduling system simulator that defaults to using FCFS
    // scheduling policy
    SchedulingSystem sim;

    // load a simulation process table from file
    sim.loadProcessTable("simfiles/process-table-01.sim");

    // process A runs from 0 to 3, B from 3 to 9 in table 01
    // simulate full cycles for 8 time cycles
    // we are calling check process done after each cpu cycle now, so also
    // testing it correctly does nothing when process is not done yet
    for (int time = 0; time < 8; time++)
    {
      sim.checkProcessArrivals();
      sim.dispatchCpuIfIdle();
      sim.simulateCpuCycle();
      sim.checkProcessFinished();
    }

    // process A should have run and be done, but B is not yet done and is still
    // running at this point
    CHECK(sim.getSystemTime() == 8);
    CHECK_FALSE(sim.isCpuIdle());
    CHECK(sim.getRunningProcessName() == "B");
    CHECK(sim.process[0].startTime == 0);
    CHECK(sim.process[0].endTime == 3);
    CHECK(sim.process[0].done);
    CHECK(sim.process[1].startTime == 3);
    CHECK(sim.process[1].endTime == NOT_STARTED);
    CHECK_FALSE(sim.process[1].done);

    // one more cycle should cause process B to finish
    sim.checkProcessArrivals();
    sim.dispatchCpuIfIdle();
    sim.simulateCpuCycle();
    sim.checkProcessFinished();

    CHECK(sim.getSystemTime() == 9);
    CHECK(sim.isCpuIdle());
    CHECK(sim.getRunningProcessName() == "IDLE");
    CHECK(sim.process[0].startTime == 0);
    CHECK(sim.process[0].endTime == 3);
    CHECK(sim.process[0].done);
    CHECK(sim.process[1].startTime == 3);
    CHECK(sim.process[1].endTime == 9);
    CHECK(sim.process[1].done);
  }
}
#endif

/**
 * @brief Task 5: Test full runSimulation() using FCFS.
 */
#define task5
#ifdef task5
/**
 */
TEST_CASE("Task 5: Test SchedulingSystem runSimulation()", "[task5]")
{
  SECTION("Task 5: Test SchedulingSystem runSimulation() FCFS on table 01", "[task5]")
  {
    // a basic scheduling system simulator that defaults to using FCFS
    // scheduling policy
    SchedulingSystem sim;

    // load a simulation process table from file
    sim.loadProcessTable("simfiles/process-table-01.sim");

    // run the simulation to completion with default FCFS
    // policy using the first process table simulation
    sim.runSimulation(false);
    CHECK(sim.isCpuIdle());
    CHECK(sim.allProcessesDone());
    CHECK(sim.getSystemTime() == 20);
    CHECK(sim.finalSchedule() == "A  A  A  B  B  B  B  B  B  C  C  C  C  D  D  D  D  D  E  E  ");
    CHECK(sim.finalResultsTable() == "Name Arrv T_s  Strt End  T_r  T_r / T_s\n"
                                     "A    0    3    0    3    3    1.0000  \n"
                                     "B    2    6    3    9    7    1.1667  \n"
                                     "C    4    4    9    13   9    2.2500  \n"
                                     "D    6    5    13   18   12   2.4000  \n"
                                     "E    8    2    18   20   12   6.0000  \n");
  }

  SECTION("Task 5: Test SchedulingSystem runSimulation() FCFS on table 02", "[task5]")
  {
    // a basic scheduling system simulator that defaults to using FCFS
    // scheduling policy
    SchedulingSystem sim;

    // load a simulation process table from file
    sim.loadProcessTable("simfiles/process-table-02.sim");

    // run the simulation to completion with default FCFS
    // policy using the first process table simulation
    sim.runSimulation(false);
    CHECK(sim.isCpuIdle());
    CHECK(sim.allProcessesDone());
    CHECK(sim.getSystemTime() == 20);
    CHECK(sim.finalSchedule() == "A  A  A  B  B  B  B  B  C  C  D  D  D  D  D  E  E  E  E  E  ");
    CHECK(sim.finalResultsTable() == "Name Arrv T_s  Strt End  T_r  T_r / T_s\n"
                                     "A    0    3    0    3    3    1.0000  \n"
                                     "B    1    5    3    8    7    1.4000  \n"
                                     "C    3    2    8    10   7    3.5000  \n"
                                     "D    9    5    10   15   6    1.2000  \n"
                                     "E    12   5    15   20   8    1.6000  \n");
  }
}
#endif
