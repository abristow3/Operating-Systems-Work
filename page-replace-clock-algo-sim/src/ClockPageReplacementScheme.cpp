/** @file ClockPageReplacementScheme.hpp
 * @brief ClockPageReplacementScheme class implementation file
 *
 * @author Student Name
 * @note   cwid: 123456
 * @date   Fall 2019
 * @note   ide:  g++ 8.2.0 / GNU Make 4.2.1
 *
 * Implementation file for our ClockPageReplacementScheme class.  This concrete
 * child class implements a clock page replacement scheme for the
 * PagingSystem simulator
 */
#include "ClockPageReplacementScheme.hpp"
#include <iomanip>
#include <sstream>
#include <string>

/**
 * @brief constructor
 *
 * The basic constructor is called by the PagingSystem when setting up/creating
 * the paging scheme to use for a simulation.  We keep track of the paging system
 * instance we are associated with, so we can call the paging system to get
 * needed information to make replacment decisions.
 *
 * @param sys The associated PagingSystem system instance that this scheme
 *   is associated with.
 */
ClockPageReplacementScheme::ClockPageReplacementScheme(PagingSystem* sys)
  : PageReplacementScheme(sys)
{
  // make sure we reset the fifo scheme to an initial state when first
  // creating the instnace
  resetScheme();
}

/**
 * @brief destructor
 *
 * Clean up any dynamically allocated memory when we exit scope.
 */
ClockPageReplacementScheme::~ClockPageReplacementScheme()
{
  // not described in assignment, but if you dynamically allocate
  // memory, you should delete/deallocate it in the destructor here
}

/**
 * @brief reset scheme
 *
 * Reset this paging scheme to beginning/initial state
 */
void ClockPageReplacementScheme::resetScheme()
{
  int size = sys->getMemorySize();

  this->framePointer = 0;
  this->useBit = new bool[size];

  // set all values to true initially
  for (int i = 0; i < size; i++)
  {
    this->useBit[i] = 1;
  }
}

/**
 * @brief record page hit
 *
 * For the clock replacement scheme, the use bit should be set
 * anytime a page is loaded, or a page hit occurs.  We set
 * use bit 1 here whenever a page hit refere is made.
 *
 * @param frame The frame number of the frame that was just
 *   hit by a page reference.  Set the related use bit to 0
 *   for this frame.
 */
void ClockPageReplacementScheme::pageHit(FrameNumber frame)
{
  this->useBit[frame] = 1;
}

/**
 * @brief get scheme status
 *
 * This function returns a string with a representation of the current
 * status of this page replacement scheme.  This is used as a hook
 * by the simulator so we can see inside of a page replacement scheme
 * and get a sense of its internal state and decision making process.
 * For clock, we display the memory contents and indicate where the frame
 * pointer is currently pointing, and we give an indicate of the current
 * use bit setting.
 *
 * @returns string Returns a string representation of the current status
 *   of the clock page replacement algorithm.
 */
string ClockPageReplacementScheme::getSchemeStatus()
{
  stringstream out;

  // iterate through all memory frames, displaying their content
  // Task 5: need to access the sys pointer to get the current memory size here
  for (FrameNumber frame = 0; frame < sys->getMemorySize(); frame++)
  {
    out << "frame[" << setfill('0') << setw(3) << frame << "] ";

    // if frame is actually currently empty, give more human readable
    // description of the content
    int pageNumber = sys->peekMemory(frame);
    if (pageNumber == 0)
    {
      out << setfill(' ') << setw(5) << "EMPTY";
    }
    // otherwise just show the page number loaded in this frame
    else
    {
      out << setfill(' ') << setw(5) << pageNumber;
    }

    out << " use = " << useBit[frame];

    // indicate current location of frame pointer
    if (frame == framePointer)
    {
      out << " <-- framePointer";
    }
    out << endl;
  }

  // return the resulting string
  return out.str();
}

/**
 * @brief make replacement decision
 *
 * This method is called by the paging simulator whenever a
 * page replacement decision is needed.  The fifo scheme
 * keeps track of the next frame in memory using a simple
 * frame pointer, and replaces the next page in fifo
 * (round robin) fashion.  The memory frames are treated as
 * a circular buffer, so if we wrap around the end of memory,
 * we need to circle back to frame 0.
 *
 * @returns FrameNumber the frame number selected to
 *   be replaced by the fifo scheme.
 */
FrameNumber ClockPageReplacementScheme::makeReplacementDecision()
{
  // while usebit at framepointer is true
  while (useBit[framePointer] == true)
  {
    // set false and increment
    useBit[framePointer] = false;
    framePointer = (framePointer + 1) % sys->getMemorySize();
  }

  // store framenumber that will be replaced and set usebit at that frame to true
  FrameNumber frameToReplace = framePointer;
  useBit[frameToReplace] = 1;

  // increment framepointer by one again
  framePointer = (framePointer + 1) % sys->getMemorySize();

  // return the frame selected for replacement
  return frameToReplace;
}
