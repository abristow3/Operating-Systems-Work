
# source files in this project (for beautification)
PROJECT_NAME=assg02
assg_src = Process.cpp \
	   ProcessSimulator.cpp \
	   ProcessState.cpp

test_src = ${PROJECT_NAME}-tests.cpp \
	   ${assg_src}

sim_src  = ${PROJECT_NAME}-sim.cpp \
	   ${assg_src}

# template files, list all files that define template classes
# or functions and should not be compiled separately (template
# is included where used)
template-files =

# assignment description documentation
assg_doc = ${PROJECT_NAME}.pdf

# common targets and variables used for all assignments/projects
include include/Makefile.inc

# assignment header file specific dependencies
${OBJ_DIR}/${PROJECT_NAME}-tests.o: ${SRC_DIR}/${PROJECT_NAME}-tests.cpp ${INC_DIR}/Process.hpp ${INC_DIR}/ProcessSimulator.hpp ${INC_DIR}/ProcessState.hpp
${OBJ_DIR}/${PROJECT_NAME}-sim.o: ${SRC_DIR}/${PROJECT_NAME}-sim.cpp ${INC_DIR}/ProcessSimulator.hpp
${OBJ_DIR}/Process.o: ${INC_DIR}/Process.hpp ${INC_DIR}/ProcessState.hpp ${SRC_DIR}/Process.cpp
${OBJ_DIR}/ProcessSimulator.o: ${INC_DIR}/ProcessSimulator.hpp ${INC_DIR}/Process.hpp ${INC_DIR}/ProcessState.hpp ${SRC_DIR}/ProcessSimulator.cpp
${OBJ_DIR}/ProcessState.o: ${INC_DIR}/ProcessState.hpp ${SRC_DIR}/ProcessState.cpp
