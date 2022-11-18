
# source files in this project (for beautification)
PROJECT_NAME=assg04
assg_src = PagingSystem.cpp \
	   PageReplacementScheme.cpp \
	   ClockPageReplacementScheme.cpp \
	   FifoPageReplacementScheme.cpp

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
${OBJ_DIR}/${PROJECT_NAME}-tests.o: ${INC_DIR}/PagingSystem.hpp ${INC_DIR}/PageReplacementScheme.hpp ${INC_DIR}/FifoPageReplacementScheme.hpp ${INC_DIR}/ClockPageReplacementScheme.hpp ${SRC_DIR}/${PROJECT_NAME}-tests.cpp
${OBJ_DIR}/${PROJECT_NAME}-sim.o: ${INC_DIR}/PagingSystem.hpp ${INC_DIR}/PageReplacementScheme.hpp ${INC_DIR}/FifoPageReplacementScheme.hpp ${INC_DIR}/ClockPageReplacementScheme.hpp ${SRC_DIR}/${PROJECT_NAME}-sim.cpp
${OBJ_DIR}/PagingSystem.o: ${INC_DIR}/PagingSystem.hpp ${INC_DIR}/PageReplacementScheme.hpp ${INC_DIR}/FifoPageReplacementScheme.hpp ${INC_DIR}/ClockPageReplacementScheme.hpp ${SRC_DIR}/PagingSystem.cpp
${OBJ_DIR}/PageReplacementScheme.o: ${INC_DIR}/PageReplacementScheme.hpp ${INC_DIR}/PagingSystem.hpp ${SRC_DIR}/PageReplacementScheme.cpp
${OBJ_DIR}/FifoPageReplacementScheme.o: ${INC_DIR}/FifoPageReplacementScheme.hpp ${INC_DIR}/PageReplacementScheme.hpp ${SRC_DIR}/FifoPageReplacementScheme.cpp
${OBJ_DIR}/ClockPageReplacementScheme.o: ${INC_DIR}/ClockPageReplacementScheme.hpp ${INC_DIR}/PageReplacementScheme.hpp ${SRC_DIR}/ClockPageReplacementScheme.cpp
