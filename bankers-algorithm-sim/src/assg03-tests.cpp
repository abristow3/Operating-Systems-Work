/** @file assg03-tests.cpp
 * @brief Unit tests for Resouce Allocation Denial (Banker's algorithm)
 *   system tests.
 *
 * @author Student Name
 * @note   cwid: 123456
 * @date   Summer 2022
 * @note   ide:  g++ 8.2.0 / GNU Make 4.2.1
 *
 * Unit tests for assignment 03, the Resource Allocation Denial
 * simulation.  We test the needed functions to implement handling the
 * loading of system state, modifying state, and determing if a state
 * is safe or not to make the allow/deny decision.
 */
#include "SimulatorException.hpp"
#include "State.hpp"
#include "catch.hpp"

using namespace std;

/**
 * @brief test State loadState() functionality
 */
TEST_CASE("Test State loadState() functionality", "[task0]")
{
  State s;
  CHECK(s.getNumProcesses() == 0);
  CHECK(s.getNumResources() == 0);

  // try reading a nonexistent file, should throw exception
  CHECK_THROWS_AS(s.loadState("simfiles/bogus-file-name.sim"), SimulatorException);

  // try reading our actual test file
  s.loadState("simfiles/state-01.sim");
  CHECK(s.getNumProcesses() == 4);
  CHECK(s.getNumResources() == 3);

  string state01String = "Claim matrix C\n"
                         "    R0  R1  R2  \n"
                         "P0  3   2   2   \n"
                         "P1  6   1   3   \n"
                         "P2  3   1   4   \n"
                         "P3  4   2   2   \n"
                         "\n"
                         "Allocation matrix A\n"
                         "    R0  R1  R2  \n"
                         "P0  1   0   0   \n"
                         "P1  6   1   2   \n"
                         "P2  2   1   1   \n"
                         "P3  0   0   2   \n"
                         "\n"
                         "Need matrix C-A\n"
                         "    R0  R1  R2  \n"
                         "P0  2   2   2   \n"
                         "P1  0   0   1   \n"
                         "P2  1   0   3   \n"
                         "P3  4   2   0   \n"
                         "\n"
                         "Resource vector R\n"
                         "    R0  R1  R2  \n"
                         "    9   3   6   \n"
                         "\n"
                         "Available vector V\n"
                         "    R0  R1  R2  \n"
                         "    0   1   1   \n"
                         "\n";

  // cout << s;
  CHECK(s.tostring() == state01String);
}

/**
 * @brief Task 1: needsAreMet() member function tests
 */
#define task1
#ifdef task1
TEST_CASE("Task 1: needsAreMet() member function", "[task1]")
{
  // load the state-01 again for tests
  State s1;
  s1.loadState("simfiles/state-01.sim");

  // we will make our own currentAvailable vector, initially it is
  // the same as the available vector
  int currentAvailable1[] = {0, 1, 1};

  SECTION("Task 1: state 01: Only P1 needs can be met initially", "[task1]")
  {
    // only P1 can have its needs met for this current available
    CHECK_FALSE(s1.needsAreMet(0, currentAvailable1));
    CHECK(s1.needsAreMet(1, currentAvailable1));
    CHECK_FALSE(s1.needsAreMet(2, currentAvailable1));
    CHECK_FALSE(s1.needsAreMet(3, currentAvailable1));
  }

  // another test, given currentAvailable1 of 3,2,2
  // then P0 and P1 should be true (needs can be met)
  currentAvailable1[0] = 3;
  currentAvailable1[1] = 2;
  currentAvailable1[2] = 2;

  SECTION("Task 1: state 01: P0 and P1 can be met", "[task1]")
  {
    CHECK(s1.needsAreMet(0, currentAvailable1));
    CHECK(s1.needsAreMet(1, currentAvailable1));
    CHECK_FALSE(s1.needsAreMet(2, currentAvailable1));
    CHECK_FALSE(s1.needsAreMet(3, currentAvailable1));
  }

  // test all process needs can be met
  currentAvailable1[0] = 4;
  currentAvailable1[1] = 2;
  currentAvailable1[2] = 3;

  SECTION("Task 1: state 01: all processes are now eligible", "[task1]")
  {
    CHECK(s1.needsAreMet(0, currentAvailable1));
    CHECK(s1.needsAreMet(1, currentAvailable1));
    CHECK(s1.needsAreMet(2, currentAvailable1));
    CHECK(s1.needsAreMet(3, currentAvailable1));
  }

  // test no process needs can be met (max of each need)
  currentAvailable1[0] = 1;
  currentAvailable1[1] = 5;
  currentAvailable1[2] = 0;

  SECTION("Task 1: state 01: no needs can be met", "[task1]")
  {

    CHECK_FALSE(s1.needsAreMet(0, currentAvailable1));
    CHECK_FALSE(s1.needsAreMet(1, currentAvailable1));
    CHECK_FALSE(s1.needsAreMet(2, currentAvailable1));
    CHECK_FALSE(s1.needsAreMet(3, currentAvailable1));
  }

  // load the state-03 to test with different number of
  // processes and resources
  State s3;
  s3.loadState("simfiles/state-03.sim");

  // we will make our own currentAvailable1 vector, initially it is
  // the same as the available vector
  int currentAvailable3[] = {6, 3, 5, 4};

  SECTION("Task 1: state 03: test with different number of processes and "
          "resources, P1, P3 and P4 candidates",
    "[task1]")
  {
    CHECK_FALSE(s3.needsAreMet(0, currentAvailable3));
    CHECK(s3.needsAreMet(1, currentAvailable3));
    CHECK_FALSE(s3.needsAreMet(2, currentAvailable3));
    CHECK(s3.needsAreMet(3, currentAvailable3));
    CHECK(s3.needsAreMet(4, currentAvailable3));
    CHECK_FALSE(s3.needsAreMet(5, currentAvailable3));
  }

  // if P1 runs, current available is now updated and
  // P1, P2, P3, P4, P5 are all candidates now (but not P0)
  currentAvailable3[0] = 6;
  currentAvailable3[1] = 4;
  currentAvailable3[2] = 6;
  currentAvailable3[3] = 5;

  SECTION("Task 1: state 03: P1-P5 now all candidates but not P0", "[task1]")
  {
    CHECK_FALSE(s3.needsAreMet(0, currentAvailable3));
    CHECK(s3.needsAreMet(1, currentAvailable3));
    CHECK(s3.needsAreMet(2, currentAvailable3));
    CHECK(s3.needsAreMet(3, currentAvailable3));
    CHECK(s3.needsAreMet(4, currentAvailable3));
    CHECK(s3.needsAreMet(5, currentAvailable3));
  }

  // if P2 then runs, current available is now updated and
  // all of P0, P1, P2, P3, P4, P5 are candidates
  currentAvailable3[0] = 10;
  currentAvailable3[1] = 5;
  currentAvailable3[2] = 6;
  currentAvailable3[3] = 7;

  SECTION("Task 1: state 03: all processes now candidates", "[task1]")
  {
    CHECK(s3.needsAreMet(0, currentAvailable3));
    CHECK(s3.needsAreMet(1, currentAvailable3));
    CHECK(s3.needsAreMet(2, currentAvailable3));
    CHECK(s3.needsAreMet(3, currentAvailable3));
    CHECK(s3.needsAreMet(4, currentAvailable3));
    CHECK(s3.needsAreMet(5, currentAvailable3));
  }

  // set current available for 1 resource to not be met for each process
  currentAvailable3[0] = 6;
  currentAvailable3[1] = 3;
  currentAvailable3[2] = 2;
  currentAvailable3[3] = 0;

  SECTION("Task 1: state 03: no processes are candidates", "[task1]")
  {
    CHECK_FALSE(s3.needsAreMet(0, currentAvailable3));
    CHECK_FALSE(s3.needsAreMet(1, currentAvailable3));
    CHECK_FALSE(s3.needsAreMet(2, currentAvailable3));
    CHECK_FALSE(s3.needsAreMet(3, currentAvailable3));
    CHECK_FALSE(s3.needsAreMet(4, currentAvailable3));
    CHECK_FALSE(s3.needsAreMet(5, currentAvailable3));
  }
}
#endif

/**
 * @brief Task 2: findCandidateProcess() member function tests
 */
#define task2
#ifdef task2
TEST_CASE("Task 2: findCandidateProcess() member function", "[task2]")
{
  // load the state-01 again for tests
  State s1;
  s1.loadState("simfiles/state-01.sim");

  // initialy for the state-01 file, P1 is the first process whose
  // would be a candidate given the current available resources
  // we will make our own currentAvailable vector, initially it is
  // the same as the available vector
  int currentAvailable1[] = {0, 1, 1};

  // all process are not yet compeleted for first tests
  bool completed1[] = {false, false, false, false};

  int candidateProcess;

  SECTION("Task 2: state 01: P1 is first candidate", "[task2]")
  {
    candidateProcess = s1.findCandidateProcess(completed1, currentAvailable1);
    CHECK(candidateProcess == 1);
  }

  // if P1 is now marked (but available is not changed), no
  // other process is a candidate, so findCandidate should
  // fail
  completed1[1] = true;

  SECTION("Task 2: state 01: P1 complete now no candidates", "[task2]")
  {
    candidateProcess = s1.findCandidateProcess(completed1, currentAvailable1);
    CHECK(candidateProcess == NO_CANDIDATE);
  }

  // if P1 resources were returned, the new current available
  // would be
  currentAvailable1[0] = 6;
  currentAvailable1[1] = 2;
  currentAvailable1[2] = 3;

  // which means actually all remainig processes needs can be met,
  // the findCandidate should return the first it finds, P0
  SECTION("Task 2: state 01: P0 now first candidate", "[task2]")
  {
    candidateProcess = s1.findCandidateProcess(completed1, currentAvailable1);
    CHECK(candidateProcess == 0);
  }

  // to complete this simulation, P0 resources would be returned,
  // but this won't make a difference, if P0 is marked as complete
  // we should now determine P2 is a canddate
  completed1[0] = true;

  SECTION("Task 2: state 01: P2 now first candidate", "[task2]")
  {
    candidateProcess = s1.findCandidateProcess(completed1, currentAvailable1);
    CHECK(candidateProcess == 2);
  }

  // to finish, if P2 is now marked, then P3 is final candidate
  completed1[2] = true;

  SECTION("Task 2: state 01: P3 now final candidate", "[task2]")
  {
    candidateProcess = s1.findCandidateProcess(completed1, currentAvailable1);
    CHECK(candidateProcess == 3);
  }

  // when everything is marked completed,there should be no
  // candidates left
  completed1[3] = true;

  SECTION("Task 2: state 01: all complete so no candidate again", "[task2]")
  {
    candidateProcess = s1.findCandidateProcess(completed1, currentAvailable1);
    CHECK(candidateProcess == NO_CANDIDATE);
  }

  // check no process is complete, but still no candidate because
  // not enough resources for any process to complete
  completed1[0] = false;
  completed1[1] = false;
  completed1[2] = false;
  completed1[3] = false;

  currentAvailable1[0] = 0;
  currentAvailable1[1] = 5;
  currentAvailable1[2] = 0;

  SECTION("Task 2: state 01: no candidate because not enough available for any "
          "process",
    "[task2]")
  {
    candidateProcess = s1.findCandidateProcess(completed1, currentAvailable1);
    CHECK(candidateProcess == NO_CANDIDATE);
  }

  // only P3 is a candidate because it is only process that
  // doesn't need any R2
  currentAvailable1[0] = 4;

  SECTION("Task 2: state 01: P3 a candidate again because doesn't need R2", "[task2]")
  {

    candidateProcess = s1.findCandidateProcess(completed1, currentAvailable1);
    CHECK(candidateProcess == 3);
  }

  // load the state-03 to test with differerent number of processes and
  // resources
  State s3;
  s3.loadState("simfiles/state-03.sim");

  // initialy for the state 3 the current available is as follows for the 4
  // resources
  int currentAvailable3[] = {6, 3, 5, 4};

  // all process are not yet compeleted for first tests
  bool completed3[] = {false, false, false, false, false, false};

  // initially P1, P3 and P4 all can meet needs, P1 should be first to be
  // returned
  SECTION("Task 2: state 03: P1 is first candidate", "[task2]")
  {
    candidateProcess = s3.findCandidateProcess(completed3, currentAvailable3);
    CHECK(candidateProcess == 1);
  }

  // simulate releasing P1 resources and marking it complete, P1 has allocated
  // [0 1 1 1]
  completed3[1] = true;
  currentAvailable3[1] = 4;
  currentAvailable3[2] = 5;
  currentAvailable3[3] = 5;

  // Now P1 is not a candidate because marked as completed, P2, P3, P4 and P5
  // should all be candidates
  SECTION("Task 2: state 03: P2 is first candidate not yet completed", "[task2]")
  {
    candidateProcess = s3.findCandidateProcess(completed3, currentAvailable3);
    CHECK(candidateProcess == 2);
  }

  // release P2 resources and mark complete, P2 has allocated [4 1 0 2]
  completed3[2] = true;
  currentAvailable3[0] = 10;
  currentAvailable3[1] = 5;
  currentAvailable3[2] = 6;
  currentAvailable3[3] = 7;

  // Now P1 and P2 are marked, everything remaing P0, P3, P4, P5 are now
  // candidates P0 should be returned first at this point
  SECTION("Task 2: state 03: P0 is first candidate now", "[task2]")
  {
    candidateProcess = s3.findCandidateProcess(completed3, currentAvailable3);
    CHECK(candidateProcess == 0);
  }

  // no need to release more resources at this point, if we mark P0, the P3, P4
  // and P5 are candidates.  Mark P0 and each process in turn and see that find
  // candidate returns expected next process
  completed3[0] = true;
  SECTION("Task 2: state 03: P3 is first candidate now", "[task2]")
  {
    candidateProcess = s3.findCandidateProcess(completed3, currentAvailable3);
    CHECK(candidateProcess == 3);
  }

  completed3[3] = true;
  SECTION("Task 2: state 03: P4 is first candidate now", "[task2]")
  {
    candidateProcess = s3.findCandidateProcess(completed3, currentAvailable3);
    CHECK(candidateProcess == 4);
  }

  completed3[4] = true;
  SECTION("Task 2: state 03: P5 is first candidate now", "[task2]")
  {
    candidateProcess = s3.findCandidateProcess(completed3, currentAvailable3);
    CHECK(candidateProcess == 5);
  }

  // all processes have now been marked as completed.  find candidate should now
  // return no candidate process
  completed3[5] = true;
  SECTION("Task 2: state 03: all are completed so no more candidates", "[task2]")
  {
    candidateProcess = s3.findCandidateProcess(completed3, currentAvailable3);
    CHECK(candidateProcess == NO_CANDIDATE);
  }
}
#endif

/**
 * @brief Task 3: releaseAllocatedResources() member function tests
 */
#define task3
#ifdef task3
TEST_CASE("Task 3: releaseAllocatedResources() member function", "[task3]")
{
  // load the state-01 again for tests
  State s1;
  s1.loadState("simfiles/state-01.sim");

  // initialy for the state-01 file, P1 is the first process whose
  // would be a candidate given the current available resources
  // we will make our own currentAvailable vector, initially it is
  // the same as the available vector
  int currentAvailable1[] = {0, 1, 1};

  // all process are not yet compeleted for first tests
  bool completed1[] = {false, false, false, false};

  // we are simulating the isSafe() mechanism more fully here
  // We use findCandidate, then we will release the
  // resources of the candidate.  This is basically the steps
  // that need to happen in a loop in the isSafe() method...
  int candidateProcess;
  candidateProcess = s1.findCandidateProcess(completed1, currentAvailable1);

  CHECK(candidateProcess == 1);

  // release the process allocations
  s1.releaseAllocatedResources(candidateProcess, currentAvailable1);
  // and check the currentAvailable are as we would expect now
  CHECK(currentAvailable1[0] == 6);
  CHECK(currentAvailable1[1] == 2);
  CHECK(currentAvailable1[2] == 3);

  // continue simulation of isSafe() steps.  P1 is now complete, and
  // currentAvailable have been updated, so find next candidate
  // process, which will be P0 and release its resources
  completed1[candidateProcess] = true;
  candidateProcess = s1.findCandidateProcess(completed1, currentAvailable1);
  CHECK(candidateProcess == 0);

  s1.releaseAllocatedResources(candidateProcess, currentAvailable1);
  CHECK(currentAvailable1[0] == 7);
  CHECK(currentAvailable1[1] == 2);
  CHECK(currentAvailable1[2] == 3);

  // mark P0 as complete, its resources were already released, and
  // find next candidate which will be P2 and release its allocations
  completed1[candidateProcess] = true;
  candidateProcess = s1.findCandidateProcess(completed1, currentAvailable1);

  CHECK(candidateProcess == 2);
  s1.releaseAllocatedResources(candidateProcess, currentAvailable1);
  CHECK(currentAvailable1[0] == 9);
  CHECK(currentAvailable1[1] == 3);
  CHECK(currentAvailable1[2] == 4);

  // only P3 left, find, mark and release its resources
  // final available should be equal to the system totalAvailable
  completed1[candidateProcess] = true;
  candidateProcess = s1.findCandidateProcess(completed1, currentAvailable1);

  CHECK(candidateProcess == 3);
  s1.releaseAllocatedResources(candidateProcess, currentAvailable1);
  CHECK(currentAvailable1[0] == 9);
  CHECK(currentAvailable1[1] == 3);
  CHECK(currentAvailable1[2] == 6);

  // final test, there are plenty of resources available but all
  // processes are complete, so findCandidate should fail
  completed1[candidateProcess] = true;
  candidateProcess = s1.findCandidateProcess(completed1, currentAvailable1);

  CHECK(candidateProcess == NO_CANDIDATE);

  // load the state-03 to test with different number of processes and resources
  State s3;
  s3.loadState("simfiles/state-03.sim");

  // initialy for the state-03 the following are the currently available for the
  // 4 resources
  int currentAvailable3[] = {6, 3, 5, 4};

  // all process are not yet compeleted for first tests
  bool completed3[] = {false, false, false, false, false, false};

  // we are simulating the isSafe() mechanism more fully here
  // We use findCandidate, then we will release the
  // resources of the candidate. Initially P1, P3 and P4 are candidates,
  // should get P1 first
  candidateProcess = s3.findCandidateProcess(completed3, currentAvailable3);
  CHECK(candidateProcess == 1);

  // release the process P1 allocations [0 1 1 1]
  s3.releaseAllocatedResources(candidateProcess, currentAvailable3);
  // and check the currentAvailable are as we would expect now
  CHECK(currentAvailable3[0] == 6);
  CHECK(currentAvailable3[1] == 4);
  CHECK(currentAvailable3[2] == 6);
  CHECK(currentAvailable3[3] == 5);

  // mark P1 as completed, now candidates are P2, P3, P4 and P5 and P2 is
  // returned
  completed3[1] = true;
  candidateProcess = s3.findCandidateProcess(completed3, currentAvailable3);
  CHECK(candidateProcess == 2);

  // release the process P2 allocations [4 1 0 2]
  s3.releaseAllocatedResources(candidateProcess, currentAvailable3);
  // and check the currentAvailable are as we would expect now
  CHECK(currentAvailable3[0] == 10);
  CHECK(currentAvailable3[1] == 5);
  CHECK(currentAvailable3[2] == 6);
  CHECK(currentAvailable3[3] == 7);

  // mark P2 as completed, now candidates are P0, P3, P4 and P5 and P0 is
  // returned
  completed3[2] = true;
  candidateProcess = s3.findCandidateProcess(completed3, currentAvailable3);
  CHECK(candidateProcess == 0);

  // release the process P0 allocations [2 0 2 1]
  s3.releaseAllocatedResources(candidateProcess, currentAvailable3);
  // and check the currentAvailable are as we would expect now
  CHECK(currentAvailable3[0] == 12);
  CHECK(currentAvailable3[1] == 5);
  CHECK(currentAvailable3[2] == 8);
  CHECK(currentAvailable3[3] == 8);

  // mark P0 as completed, now candidates are P3, P4 and P5 and P3 is returned
  completed3[0] = true;
  candidateProcess = s3.findCandidateProcess(completed3, currentAvailable3);
  CHECK(candidateProcess == 3);

  // release the process P3 allocations [1 0 0 1]
  s3.releaseAllocatedResources(candidateProcess, currentAvailable3);
  // and check the currentAvailable are as we would expect now
  CHECK(currentAvailable3[0] == 13);
  CHECK(currentAvailable3[1] == 5);
  CHECK(currentAvailable3[2] == 8);
  CHECK(currentAvailable3[3] == 9);

  // mark P3 as completed, now candidates are P4 and P5 and P4 is returned
  completed3[3] = true;
  candidateProcess = s3.findCandidateProcess(completed3, currentAvailable3);
  CHECK(candidateProcess == 4);

  // release the process P4 allocations [1 1 0 0]
  s3.releaseAllocatedResources(candidateProcess, currentAvailable3);
  // and check the currentAvailable are as we would expect now
  CHECK(currentAvailable3[0] == 14);
  CHECK(currentAvailable3[1] == 6);
  CHECK(currentAvailable3[2] == 8);
  CHECK(currentAvailable3[3] == 9);

  // mark P4 as completed, now candidates are P5 and P5 is returned
  completed3[4] = true;
  candidateProcess = s3.findCandidateProcess(completed3, currentAvailable3);
  CHECK(candidateProcess == 5);

  // release the process P5 allocations [1 0 1 1], all resources released so
  // should end up with total resources R [15 6 9 10]
  s3.releaseAllocatedResources(candidateProcess, currentAvailable3);
  // and check the currentAvailable are as we would expect now
  CHECK(currentAvailable3[0] == 15);
  CHECK(currentAvailable3[1] == 6);
  CHECK(currentAvailable3[2] == 9);
  CHECK(currentAvailable3[3] == 10);

  // mark P5 as complete, all processes now completed
  completed3[5] = true;
  candidateProcess = s3.findCandidateProcess(completed3, currentAvailable3);
  CHECK(candidateProcess == NO_CANDIDATE);
}
#endif

/**
 * @brief Task 4: isSafe() member function tests
 */
#define task4
#ifdef task4
TEST_CASE("Task 4: isSafe() member function", "[task4]")
{
  // load the state-01 again for tests
  // state 01 is safe, so check it
  State s;
  SECTION("Task 4: test state 01 is safe", "[task4]")
  {
    s.loadState("simfiles/state-01.sim");
    CHECK(s.isSafe());
  }

  // load the state-02
  // state 02 is unsafe, so check it
  SECTION("Task 4: test state 02 is NOT safe", "[task4]")
  {
    s.loadState("simfiles/state-02.sim");
    CHECK_FALSE(s.isSafe());
  }

  // load the state-03
  // state 03 is safe, so check it
  SECTION("Task 4: test state 03 is safe", "[task4]")
  {
    s.loadState("simfiles/state-03.sim");
    CHECK(s.isSafe());
  }

  // load the state-04
  // state 04 is unsafe, so check it
  SECTION("Task 4: test state 04 is NOT safe", "[task4]")
  {
    s.loadState("simfiles/state-04.sim");
    CHECK_FALSE(s.isSafe());
  }

  // load the state-05
  // state 05 is unsafe, so check it
  // This test checks for implementations that do not correctly detect if 1 or more processes
  // are initially candidates, but not all processes are candidates
  SECTION("Task 4: test state 05 is NOT safe", "[task4]")
  {
    s.loadState("simfiles/state-05.sim");
    CHECK_FALSE(s.isSafe());
  }
}
#endif
