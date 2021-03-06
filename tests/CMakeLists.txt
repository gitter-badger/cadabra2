cmake_minimum_required(VERSION 2.8)

project(CadabraTests)

message("-- Configuring self-tests")

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure)

set(PYTHONPATH ${CMAKE_SOURCE_DIR}/core:${CMAKE_SOURCE_DIR}/core/packages:${CMAKE_BINARY_DIR}/core)
message("-- Set PYTHONPATH for testing to ${PYTHONPATH}:$ENV{PYTHONPATH}")

if (!WIN32)
  add_test(comparison     test_comparison)
endif()


# Notebooks with built-in tests.
#
set(CNBIN   ${CMAKE_SOURCE_DIR}/examples)
set(CDBOUT  ${CMAKE_BINARY_DIR}/tests)
set(DC      ${CMAKE_SOURCE_DIR}/client_server/cadabra2html.cc ${CMAKE_SOURCE_DIR}/client_server/DataCell.hh ${CMAKE_SOURCE_DIR}/client_server/DataCell.cc)
set(NBTESTS
  quickstart
  beginners
  tensor_monomials
  for_previous_users
  converge
  fierz
  gamma_matrix_algebra
  poincare_algebra
  string_states
  bianchi_identities
  schwarzschild
  kaluza_klein
  scalar_manipulations
  )
foreach(NBTEST ${NBTESTS})
  add_custom_command(OUTPUT  ${CDBOUT}/${NBTEST}.cdb
	 DEPENDS ${CNBIN}/${NBTEST}.cnb ${DC}
	 COMMAND cadabra2cadabra
	 ARGS    ${CNBIN}/${NBTEST}.cnb ${CDBOUT}/${NBTEST}.cdb
	 COMMENT "Creating ${NBTEST} notebook...")
  add_custom_target("${NBTEST}_test" ALL echo -n DEPENDS ${CDBOUT}/${NBTEST}.cdb)
  add_test(${NBTEST}  ${CMAKE_SOURCE_DIR}/core/cadabra2 ${CDBOUT}/${NBTEST}.cdb)
  set_tests_properties(${NBTEST}      PROPERTIES ENVIRONMENT "PYTHONPATH=${PYTHONPATH}:$ENV{PYTHONPATH}")
endforeach()

# The main test suite.
#
set(RTESTS
  packages
  algebra
  components
  factor
  field_theory
  gamma_paper
  fierz
  selecting
  kaluza_klein
  paper
  basic
  numerical
  substitute
  canonicalise
  derivative
  relativity
  symmetry
  vary
  young
  )
foreach(RTEST ${RTESTS})
  add_test(${RTEST}  ${CMAKE_SOURCE_DIR}/core/cadabra2 ${CMAKE_SOURCE_DIR}/tests/${RTEST}.cdb)
  # We need to set the Python path so that we pick up the correct cadabra2.so Python module
  # even if we did not do 'make install' yet.
  set_tests_properties(${RTEST}      PROPERTIES ENVIRONMENT "PYTHONPATH=${PYTHONPATH}:$ENV{PYTHONPATH}")
endforeach()

# This does not work properly, and on OSX it fails because we do not
# use any other include settings, unless we hard-code the path to
# where we expect gmpxx to be if Homebrew installed it.
if(APPLE)
   add_definitions("-I/usr/local/include")
endif()
find_library(GMP_LIB gmp REQUIRED)
find_library(GMPXX_LIB gmpxx REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/core ${GMPXX_LIB_INCLUDE_DIRS})

if (!WIN32)
  add_executable(test_comparison test_comparison.cc)
  target_link_libraries(test_comparison cadabra2)
endif()
