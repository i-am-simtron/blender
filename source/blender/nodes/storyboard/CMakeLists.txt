# SPDX-License-Identifier: GPL-2.0-or-later

set(INC
  .
  ..
  ../intern
  ../../editors/include
  ../../blenkernel
  ../../blenlib
  ../../blentranslation
  ../../bmesh
  ../../depsgraph
  ../../functions
  ../../imbuf
  ../../makesdna
  ../../makesrna
  ../../render
  ../../windowmanager
  ../../../../intern/guardedalloc
  # RNA_prototypes.h
  ${CMAKE_BINARY_DIR}/source/blender/makesrna
)


set(SRC
  node_storyboard_exec.cc
  node_storyboard_tree.cc
  node_storyboard_util.cc

  node_storyboard_util.hh
)

set(LIB
  bf_functions
  bf_nodes
)


if(WITH_PYTHON)
  list(APPEND INC
    ../../python
  )
  list(APPEND INC_SYS
    ${PYTHON_INCLUDE_DIRS}
  )
  list(APPEND LIB
    ${PYTHON_LINKFLAGS}
    ${PYTHON_LIBRARIES}
  )
  add_definitions(-DWITH_PYTHON)
endif()

if(WITH_TBB)
  list(APPEND INC_SYS
    ${TBB_INCLUDE_DIRS}
  )
  add_definitions(-DWITH_TBB)
  if(WIN32)
    # TBB includes Windows.h which will define min/max macros
    # that will collide with the stl versions.
    add_definitions(-DNOMINMAX)
  endif()
endif()

if(WITH_IMAGE_OPENEXR)
  add_definitions(-DWITH_OPENEXR)
endif()

blender_add_lib(bf_nodes_storyboard "${SRC}" "${INC}" "${INC_SYS}" "${LIB}")

if(WITH_UNITY_BUILD)
  set_target_properties(bf_nodes_storyboard PROPERTIES UNITY_BUILD ON)
  set_target_properties(bf_nodes_storyboard PROPERTIES UNITY_BUILD_BATCH_SIZE 10)
endif()

# RNA_prototypes.h
add_dependencies(bf_nodes_storyboard bf_rna)
