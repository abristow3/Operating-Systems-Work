/** @file SchedulingSystem.cpp
 * @brief SchedulingSystem class implementations
 *
 * @author Student Name
 * @note   cwid: 123456
 * @date   Summer 2022
 * @note   ide:  g++ 8.2.0 / GNU Make 4.2.1
 *
 * Implementation file for our SchedulingSystem class.  The
 * SchedulingSystem class defines the framework for a process
 * scheduling based system.  It supports reading in a table of process
 * arrival and service time information from a file or generating a
 * random set of process arrivals.  This system keeps track of whether
 * a job is currently running or not, if the current job has finished
 * (it has run its service time to completion), and helps with
 * preemption of running processes if the process scheduling policy is
 * a preemptive policy.  This class works with a SchedulingPolicy
 * object, which is the object that actually makes the scheduling
 * decisions when needed.
 */
#include "SchedulingSystem.hpp"
#include "FCFSSchedulingPolicy.hpp"
#include "SimulatorException.hpp"
#include <climits>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

/**
 * @brief SchedulingSystem default constructor
 *
 * Default constructor for a SchedulingSystem simulation.  When beginning
 * a simulation, the most basic information will be what process scheduling
 * policy is to be used for the simulation.
 *
 * We default to a FCFS scheduling policy if none is specified when
 * the simulation starts.
 */
SchedulingSystem::SchedulingSystem()
{
  // create a FCFS scheduling policy by default
  this->policy = new FCFSSchedulingPolicy();

  // make sure we are mutually associated, that the policy knows
  // they are working with this system.
  this->policy->setSchedulingSystem(this);

  // 1 time initializations go here
  process = nullptr;

  // make sure system is setup for a new simulation
  resetSystem();
}

/**
 * @brief SchedulingSystem policy
 *
 * Alternative constructor for a SchedulingSystem simulation.
 * You can specify a different strategy other than the default
 * strategy by creating your own SchedulingStrategy instance
 * (dynamically) and passing a pointer to the instance
 * into the constructor to be used by the simulation.
 *
 * @param policy A pointer to the policy that should be
 *   used by this simulation.
 */
SchedulingSystem::SchedulingSystem(SchedulingPolicy* policy)
{
  // remember which policy we are tol to use for the simulations
  this->policy = policy;

  // make sure we are mutually associated, that the policy knows
  // they are working with this system.
  this->policy->setSchedulingSystem(this);

  // 1 time initializations go here
  process = nullptr;

  // make sure system is setup for a new simulation
  resetSystem();
}

/**
 * @brief SchedulingSystem destructor
 *
 * Destroy the SchedulingSystem instance.  Free up any dynamically
 * allocated memory.
 */
SchedulingSystem::~SchedulingSystem()
{
  // delete scheduling policy if we have one
  if (policy)
  {
    delete policy;
  }

  // delete process table
  if (process)
  {
    delete[] process;
  }
}

/**
 * @brief reset system simulation
 *
 * Make sure the simulation is correctly initialized to start
 * a new simulation.
 */
void SchedulingSystem::resetSystem()
{
  // set initial state of simulation variables
  systemTime = 0;
  cpu = IDLE;
  schedule = "";

  // clear out process table if it exists
  if (process)
  {
    delete[] process;
  }
  process = nullptr;
  numProcesses = 0;

  // make sure our policy resets as well
  policy->resetPolicy();
}

/** @brief Gets the system time
 *
 *
 * @returns int current system time
 */
int SchedulingSystem::getSystemTime() const
{
  return this->systemTime;
}

/** @brief Gets the number of processes
 *
 *
 * @returns int containing the total number of processes.
 */
int SchedulingSystem::getNumProcesses() const
{
  return this->numProcesses;
}

/** @brief check if CPU is IDLE or not
 *
 *
 * @returns bool  * If the CPU is IDLE then
 * the method returns true, otherwise we return false
 */
bool SchedulingSystem::isCpuIdle() const
{
  if (this->cpu == IDLE)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/** @brief get the name of the current running process.
 *
 * Calls isCpuUdle() to get the current state of the CPU. If it returns
 * IDLE, then we return the string "IDLE", otherwise we return the
 * process.name
 *
 * @returns string which contains IDLE if CPU state is IDLE, or the
 * process name as a string.
 */
string SchedulingSystem::getRunningProcessName() const
{
  string idle = "IDLE";

  if (this->isCpuIdle())
  {
    return idle;
  }
  else
  {
    return process[this->cpu].name;
  }
}

/** @brief check if all processes are done
 * Loops over each process in the process table and check if
 * the process is done. Breaks the loop and returns false if
 * on the first process found that is not done.
 *
 * @returns Returns false if any process in the process table is not done.
 * Otherwise return true since all processes are done.
 */
bool SchedulingSystem::allProcessesDone() const
{
  Process* processTable = this->getProcessTable();

  for (int i = 0; i < this->getNumProcesses(); i++)
  {
    if (!processTable[i].done)
    {
      return false;
    }
  }
  return true;
}

/** @brief dispatch the CPU if it is IDLE
 *
 * Checks if the CPU is IDLE and gets the next PID to dispatch.
 * If the process has never ran then we update the start time
 * to the current system time .
 *
 */
void SchedulingSystem::dispatchCpuIfIdle()
{
  if (this->isCpuIdle())
  {
    this->cpu = policy->dispatch();

    if (process[this->cpu].startTime == NOT_STARTED)
    {
      process[this->cpu].startTime = this->getSystemTime();
    }
  }
}

/** @brief check if process is finished running
 *
 * Checks if the CPU is IDLE or the process used time is less than the ervice time and
 * immediatley returns if so. Otherwise endTime is recorded as the current system time,
 * the process is marked as done, and the CPU is set back to IDLE.
 *
 */
void SchedulingSystem::checkProcessFinished()
{
  if (cpu == IDLE)
  {
    return;
  }
  else if (process[this->cpu].usedTime < process[this->cpu].serviceTime)
  {
    return;
  }
  else
  {
    process[this->cpu].endTime = this->systemTime;
    process[this->cpu].done = true;
    cpu = IDLE;
  }
}

/** @brief get pid of running process
 *
 * Returns the process identifier (pid) of the process
 * that is currently running.  This pid can be the
 * special IDLE pid flag if there is currently no process
 * running on the cpu.
 *
 * @returns Pid Returns the process identifier (pid) of the
 *   current running process, or the IDLE flag if the cpu
 *   is currently idle.
 */
Pid SchedulingSystem::getRunningPid() const
{
  return cpu;
}

/** @brief get process table
 *
 * Get a handle/pointer to the simulation process table. This
 * is needed information by some policies, so they can look up
 * service times and other information about the processes.
 *
 * @returns Process* returns an array of Process objects.
 */
Process* SchedulingSystem::getProcessTable() const
{
  return process;
}

/** @brief final results table
 *
 * Calculate the final results and format as a table.
 * We stream the table into a string and return this as
 * the final result of the function.
 *
 * @returns string A string representation of the final
 *   results for the simulation formatted as a table.
 */
string SchedulingSystem::finalResultsTable() const
{
  stringstream out;

  // output a header first
  out << setw(4) << left << "Name"
      << " " << setw(4) << left << "Arrv"
      << " " << setw(4) << left << "T_s"
      << " " << setw(4) << left << "Strt"
      << " " << setw(4) << left << "End"
      << " " << setw(4) << left << "T_r"
      << " " << setw(8) << left << "T_r / T_s" << endl;

  // stream the results table to the string stream
  for (Pid pid = 0; pid < numProcesses; pid++)
  {
    Process p = process[pid];
    int turnaroundTime = p.endTime - p.arrivalTime;
    double trtsRatio = double(turnaroundTime) / double(p.serviceTime);

    out << setw(4) << left << p.name << " " << setw(4) << left << p.arrivalTime << " " << setw(4) << left << p.serviceTime << " " << setw(4)
        << left << p.startTime << " " << setw(4) << left << p.endTime << " " << setw(4) << left << turnaroundTime << " " << setw(8) << left
        << setprecision(4) << fixed << trtsRatio << endl;
  }

  // return the results as a string
  return out.str();
}

/** @brief final schedule
 *
 * Return the resulting final schedule history we
 * saw of scheduled processes running for this simulation.
 *
 * @returns string A string representation of the final
 *   schedule of process executions.
 */
string SchedulingSystem::finalSchedule() const
{
  return schedule;
}

/**
 * @brief load process table from file
 *
 * Load a table of process arrival and service time information
 * from th eindicated file.  The first line of the file indicates
 * the total number of processes in the file.  This is used
 * to dynamically allocate our table of process information objects.
 * Then the process information are read from the file and
 * put into the table for use by the simulation.  Each
 * process's information is on 1 line of the file, and is of the
 * format:
 *
 *      Name ArrivalTime ServiceTime
 *
 * It is expected that Name can be represented as a string, and ArrivalTime
 * and ServiceTime are values that can be interpreted as integers.
 *
 * @param simfilename The name of the file to open and read the process
 *   information from.
 */
void SchedulingSystem::loadProcessTable(string simfilename)
{
  ifstream processfile(simfilename);

  // if we can't open file, abort and let the user know problem
  if (not processfile.is_open())
  {
    stringstream msg;
    msg << "<SchedulingSystem::loadProcessTable> File not found, could not open"
        << " process table file: " << simfilename << endl;
    throw SimulatorException(msg.str());
  }

  // (re)set simulator variables so we start simulation from beginning
  // using new page stream and no old page references are in memory
  resetSystem();

  // determine the total number of processes in the table
  processfile >> numProcesses;
  if ((numProcesses < 0) or (numProcesses > MAX_PROCESSES))
  {
    stringstream msg;
    msg << "<SchedulingSystem::loadPageStream> Invalid number of processes"
        << " in simulation: " << numProcesses << endl;
    throw SimulatorException(msg.str());
  }

  // dynamically allocate array to hold the process table,
  // freeing up old table if needed
  if (process != NULL)
  {
    delete[] process;
  }
  process = new Process[numProcesses];

  // load the simulated process table into our process array
  int pid = 0;
  while ((not processfile.eof()) and (pid < numProcesses))
  {
    // load the process information
    processfile >> process[pid].name >> process[pid].arrivalTime >> process[pid].serviceTime;

    // initialize other variables to initial states
    process[pid].pid = pid;
    process[pid].startTime = NOT_STARTED;
    process[pid].endTime = NOT_STARTED;
    process[pid].usedTime = 0;
    process[pid].remainingTime = process[pid].serviceTime;
    process[pid].done = false;

    // increment process identifier in preparation for reading
    // the next process
    pid++;

    // extract newlines from end of file
    processfile >> ws;
  }

  // final sanity check, if we didn't load in all processes or
  // if we didn't yet reach the end of the file as we were expecting
  // then throw an exception
  if ((pid != numProcesses) or (not processfile.eof()))
  {
    stringstream msg;
    msg << "<SchedulingSystem::loadPageStream> Error, did not see expected"
        << " number of processes in process table, expected: " << numProcesses << " received: " << pid << endl;
    throw SimulatorException(msg.str());
  }
}

/**
 * @brief generate random process table
 *
 * Generate a random process table of processes and their arrival
 * times and service times.  It is useful for large scale statistical
 * analysis to generate random sets of process arrival information.
 * This method generates processes with a given probability of
 * arriving at each time step, and with a uniform service time and in
 * the range from 1 up to the maxServiceTime (defaults to 6). This
 * method also accepts a random seed. Use the const SEED_TIME to have
 * the random page table use the current system time to seed a
 * completely random page stream.
 *
 * @param numProcesses The number of processes to randomly create.  We keep
 *   creating random processes until we have generated this requested
 *   number of processes.
 * @param arrivalProbability This probability governs how probable a
 *   process is to arrive at each time step.
 * @param maxServiceTime The maximum serviceTime for the randomly generated
 *   processes.  We generate a serviceTime with uniform probability for each
 *   process in the range [1, maxServiceTime]
 * @param seed=SEED_TIME The seed to use for the random page table
 *   generation.  Default is the global constant SEED_TIME, which if
 *   given will cause the function to use the current system time
 *   to seed the random number generator.  Set this to some other value
 *   to generate a particular known random page table in order
 *   to have a repeatable simulation result.
 */
void SchedulingSystem::generateRandomProcessTable(int numProcesses, double arrivalProbability, int maxServiceTime, int seed)
{
  // first set random seed, use provided seed value, or use current
  // system time to generate a new stream if given SEED_TIME as the seed.
  if (seed == SEED_TIME)
  {
    seed = time(0);
  }
  srand(seed);

  // (re)set simulator variables so we start simulation from beginning
  // using new page stream and no old page references are in memory
  resetSystem();

  // allocate a new pageReference block of memory, freeing up any
  // previous created pageReference stream
  if (process != NULL)
  {
    delete[] process;
  }
  this->numProcesses = numProcesses;
  process = new Process[numProcesses];

  // keep generating random processes until we have generated the asked for
  // numProcesses
  int pid = 0;
  int time = 0;
  while (pid < numProcesses)
  {
    // check if a process arrives at the current time
    // r is a uniform random number from 0.0 to 1.0
    double r = ((double)rand() / (INT_MAX));
    if (r < arrivalProbability)
    {
      // the name of the process
      ostringstream nameStream;
      nameStream << "P" << pid;
      string name = nameStream.str();

      // the service time needs to be a random number from 1 to maxServiceTime
      int serviceTime = (rand() % maxServiceTime) + 1;

      // enter the new process into the table
      process[pid].pid = pid;
      process[pid].name = name;
      process[pid].arrivalTime = time;
      process[pid].serviceTime = serviceTime;

      // initialize other variables to initial states
      process[pid].startTime = NOT_STARTED;
      process[pid].endTime = NOT_STARTED;
      process[pid].usedTime = 0;
      process[pid].remainingTime = process[pid].serviceTime;
      process[pid].done = false;

      // increment the process identifier for next process we create
      pid++;
    }

    // increment arrival time for next check of process arriving
    time++;
  }
}

/** process table as string
 *
 * Return a representation of the simulated processes as a string.
 * The table is represented as a list of tuples with
 * (ProcessName, arrivalTime, serviceTime) information given for each
 * process in the process table.  This function is mainly used
 * for debugging and testing.
 *
 * @returns string Returns a string representation of the processes
 *   in our process table that will be used in the simulation.
 */
string SchedulingSystem::processTableToString() const
{
  stringstream out;

  // iterate over process table and build output string stream
  out << "[ ";
  for (Pid pid = 0; pid < numProcesses; pid++)
  {
    out << "(" << process[pid].name << " " << process[pid].arrivalTime << " " << process[pid].serviceTime << ") ";
  }
  out << "]";

  // convert to string and return
  return out.str();
}

/**
 * @brief check new arrivals
 *
 * Check if a process (or multiple processes) arrive at the
 * current system time.  We basically have to notify our policy
 * of arrivals, so they can add them to their list of ready
 * and waiting processes, waiting to be scheduled on the cpu.
 */
void SchedulingSystem::checkProcessArrivals()
{
  // search process table to see if any process has an
  // arrival time of right now.  If so, notify the policy
  // of the newly arrived process.
  for (Pid pid = 0; pid < numProcesses; pid++)
  {
    if (process[pid].arrivalTime == systemTime)
    {
      policy->newProcess(pid);
    }
  }
}

/**
 * @brief check if a process did arrive
 *
 * Convenience method for some policies.  If a process does
 * arrive at the current time, return true so that policies that
 * preempt on process arrivales cand ask for a preemption.
 *
 * @returns bool True if a process did arrive at the current system
 *   time, false if not.
 */
bool SchedulingSystem::didProcessArrive() const
{
  // search process table to see if a process is arriving at
  // the current time
  for (Pid pid = 0; pid < numProcesses; pid++)
  {
    if (process[pid].arrivalTime == systemTime)
    {
      return true;
    }
  }

  // otherwise no process is arriving at the current system time
  return false;
}

/**
 * @brief simulate cpu cycle
 *
 * Simulate the execution of the cpu.  Increment the system
 * time, and statistics for the current running process, if
 * there is one.
 */
void SchedulingSystem::simulateCpuCycle()
{
  // increment system time
  systemTime++;

  // if a process is running, increment its statistics and record
  // the history of its execution
  if (cpu != IDLE)
  {
    process[cpu].usedTime++;
    process[cpu].remainingTime--;
    schedule += (process[cpu].name + "  ");
  }
  else // record cpu idle during this time period
  {
    schedule += "I  ";
  }
}

/**
 * @brief process preemption
 *
 * Check if process needs to be preempted.  Preemption is a policy
 * decision.  So we send our policy a message each cycle to have
 * it determine if process should preempt or not. The policy will
 * return true if the process should be preempted, and false otherwise.
 * It is up to the policy to keep track of and determine if/when
 * preemption should happen.  For example for time slice based
 * systems, the policy should set a countdown timer and return true
 * when it reaches 0.  For preemption on new arrival, the policy
 * should make a note when a new process arrives, and do a preemption
 * at that point.
 */
void SchedulingSystem::checkProcessPreemption()
{
  // if cpu is idle, nothing to check
  if (cpu == IDLE)
  {
    return;
  }

  // if the policy says we need to preempt, then remove the
  // process from the cpu and make the cpu idle.
  if (policy->preempt())
  {
    cpu = IDLE;
  }
}

/**
 * @brief update process data
 *
 * Update all current ready/running processes statistics and
 * information.  Primarly, if a process is currently in the system,
 * and is not done, then update its wait time and response ratio
 * calculations.  These in particular are useful for HRRN
 * scheduling policies to have.
 */
void SchedulingSystem::updateProcessStatistics()
{
  // search all processes in the process table
  for (Pid pid = 0; pid < numProcesses; pid++)
  {
    // if the process has started but is not yet done, update
    // its information
    if ((systemTime >= process[pid].arrivalTime) and (not process[pid].done))
    {
      // update the wait time for this process
      process[pid].waitTime = systemTime - process[pid].arrivalTime;

      // update this processes response ratio
      process[pid].responseRatio = float(process[pid].serviceTime + process[pid].waitTime) / float(process[pid].serviceTime);
    }
  }
}

/**
 * @brief run simulation
 *
 * Run a full simulation of a process scheduling system.  Before
 * calling this method, make sure a valid page table for the
 * simulation has been loaded or created, and make sure you have set
 * the process scheduling policy you wish to simulation.
 *
 * This method will simulate system time passing, starting at 1,
 * until all processes have arrived and have completely finished running.
 *
 * @param verbose If true we will display information about state of
 *   system to standard output after each system time step.  Default
 *   values is false, do not display this information.
 *
 * @throws Throws an exception if no process table has been (pre)loaded
 *   and is ready for the simulation to use.
 */
void SchedulingSystem::runSimulation(bool verbose)
{
  // check that a process table is loaded and ready to
  // simulate first before beginning
  if ((numProcesses <= 0) or (process == NULL))
  {
    stringstream msg;
    msg << "<SchedulingSystem::runSimulation> Error, you must load or"
        << " generate a process table first before running"
        << " a simulation" << endl;
    throw SimulatorException(msg.str());
  }

  // simulate time passing, process new arrivals and ask the scheduling policy
  // to make scheduling decisions.  We keep running the simulation until
  // all processes in the process table are done
  string schedule = "";
  while (not allProcessesDone())
  {
    cout << "runSimulation()> systemTime: " << systemTime << endl;

    // check for new arrivals at this time step so can notify
    // our scheduling policy to add new processs they are managing
    checkProcessArrivals();

    // determine if our policy wants to preempt the current running process
    checkProcessPreemption();

    // at beginning of time time step, check if cpu is idle and try and
    // schedule and dispatch a process if we can
    dispatchCpuIfIdle();

    // simulate process running for 1 clock cycle
    simulateCpuCycle();

    // determine if process has finished
    checkProcessFinished();

    // update data/statistics of process after each clock cycle so information
    // is up to date for scheduling policies to use
    updateProcessStatistics();
  }

  // Display scheduling simulation results if asked too
  if (verbose)
  {
    // display a header of systemTime and the final schedule
    for (int time = 0; time <= systemTime; time++)
    {
      cout << setw(2) << left << time << " ";
    }
    cout << endl;
    cout << "  " << finalSchedule() << endl << endl;

    // display final job statistics as a table
    cout << finalResultsTable() << endl;
  }
}
