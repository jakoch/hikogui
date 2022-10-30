#
# LibFuzzer: https://llvm.org/docs/LibFuzzer.html#fuzzer-usage
# VCAsan:    https://github.com/MicrosoftDocs/cpp-docs/blob/main/docs/sanitizers/asan-debugger-integration.md
#

set(FUZZ_LIBRARIES_FOUND NO)

# create an interface library to link the libFuzzer libraries
add_library(clang_fuzzer INTERFACE) 

if(WIN32)
  # Find "clang.exe" (LLVM native) or clang-cl.exe (MS Clang)
  #message(DEBUG "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
  #get_filename_component(CLANG_BIN_DIRECTORY ${CMAKE_CXX_COMPILER} DIRECTORY)
  #message(DEBUG "CLANG_BIN_DIRECTORY: ${CLANG_BIN_DIRECTORY}")
  #set(CLANG_EXE "${CLANG_BIN_DIRECTORY}/clang.exe")
  #message(DEBUG "CLANG_EXE: ${CLANG_EXE}")

  set(CLANG_EXE "E:/tools/LLVM/bin/clang.exe")

  # Find Clang's libraries.
  #
  # Example output (Clang 13)
  #   > clang -print-search-dirs
  #   programs:  =C:\Program Files\LLVM\bin
  #   libraries: =C:\Program Files\LLVM\lib\clang\13.0.0
  execute_process(COMMAND ${CLANG_EXE} -print-search-dirs
    OUTPUT_VARIABLE CLANG_OUTPUT
    ERROR_VARIABLE CLANG_ERROR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULTS_VARIABLE CLANG_RESULTS
  )
  message(DEBUG "CLANG_OUTPUT: ${CLANG_OUTPUT}")
  message(DEBUG "CLANG_ERROR: ${CLANG_ERROR}")
  message(DEBUG "CLANG_RESULTS: ${CLANG_RESULTS}")

  if(NOT CLANG_RESULTS EQUAL 0)
    message(WARNING "Unable to locate Clang Search Directories: ${CLANG_RESULTS}")
  endif()

  string(REGEX MATCH "libraries: =(.+)" _MATCH "${CLANG_OUTPUT}")
  message(DEBUG "CMAKE_MATCH_1: ${CMAKE_MATCH_1}")
  set(CLANG_LIBRARIES_DIR ${CMAKE_MATCH_1})
  message(DEBUG "CLANG_LIBRARIES_DIR: ${CLANG_LIBRARIES_DIR}")

  # We are looking for files in "C:\Program Files\LLVM\lib\clang\13.0.0\lib\windows"
  function(find_clang_library VAR LIBRARY_NAME)
    find_library("_${VAR}" ${LIBRARY_NAME}
      HINTS ${CLANG_LIBRARIES_DIR}
      PATH_SUFFIXES "lib/windows"
      NO_DEFAULT_PATH
    )
    set(${VAR} ${_${VAR}} PARENT_SCOPE)
  endfunction()

  find_clang_library(CLANG_RT_ASAN_PREINIT "clang_rt.asan-preinit-x86_64")
  find_clang_library(CLANG_RT_ASAN "clang_rt.asan-x86_64")
  find_clang_library(CLANG_RT_ASAN_CXX "clang_rt.asan_cxx-x86_64")
  find_clang_library(CLANG_RT_FUZZER "clang_rt.fuzzer-x86_64")
  message(DEBUG "CLANG_RT_ASAN_PREINIT: ${CLANG_RT_ASAN_PREINIT}")
  message(DEBUG "CLANG_RT_ASAN: ${CLANG_RT_ASAN}")
  message(DEBUG "CLANG_RT_ASAN_CXX: ${CLANG_RT_ASAN_CXX}")
  message(DEBUG "CLANG_RT_FUZZER: ${CLANG_RT_FUZZER}")

  if(CLANG_RT_ASAN_PREINIT AND CLANG_RT_ASAN AND CLANG_RT_ASAN_CXX AND CLANG_RT_FUZZER)
    set(FUZZ_LIBRARIES_FOUND YES)
  endif()

  target_link_libraries(clang_fuzzer INTERFACE
    "${CLANG_RT_ASAN_PREINIT}"
    "${CLANG_RT_ASAN}"
    "${CLANG_RT_ASAN_CXX}"
    "${CLANG_RT_FUZZER}"
  )
endif()

if(FUZZ_LIBRARIES_FOUND)
  # see https://github.com/MicrosoftDocs/cpp-docs/blob/main/docs/sanitizers/asan-building.md#compiler
  target_compile_options(clang_fuzzer INTERFACE -fsanitize=fuzzer -fsanitize=address)
endif()

if(FUZZ_LIBRARIES_FOUND)
  message(STATUS "|")
  message(STATUS "[INFO] Fuzz-Testing enabled.")
  message(STATUS "[INFO] Building fuzz tests using LibFuzzer")
  else()
  message(WARNING "[INFO] Can not enable fuzzing. Clang dependencies not found.")
  return()
endif()

#-------------------------------------------------------------------
# Function: add_fuzzer
#-------------------------------------------------------------------

set(FUZZING_SOURCES
  src/fuzzing_helper.hpp
  src/fuzzing_helper.cpp
)

function(add_fuzzer SOURCE)
  get_filename_component(TARGET_NAME ${SOURCE} NAME_WLE)

  add_executable(${TARGET_NAME} src/${SOURCE} ${FUZZING_SOURCES})

  target_link_libraries(${TARGET_NAME} PRIVATE clang_fuzzer hikogui)

  target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/src)
  target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/../../src)

  # The fuzz executable is integrated into the test suite as a test ("fuzz_png.cpp" -> "fuzz_png_run").
  # The fuzz test will only run for a short duration, which is defined by FUZZ_RUNTIME.
  # Each fuzz test reads input data from it's own corpus data folder ("fuzz_png_run" -> "tests/fuzzers/data/corpus/fuzz_png/*").
  # Additionally, when the fuzzer finds new test cases, which cover new code paths, it will add them to the corpus dir.
  #
  # When the Corpus becomes too large, you want to minimize it, while keeping the full path coverage.
  # Run the following command to minimize the old corpus into a new corpus directory:
  #   "./fuzz_png -merge=1 NEW_CORPUS_DIR TOO_LARGE_OR_SPECIAL_CASES_CORPUS_DIR"
  #
  if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/data/corpus/${TARGET_NAME}/" AND NOT IS_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/data/corpus/${TARGET_NAME}/")
    message(FATAL_ERROR "[FUZZER][ERROR] The fuzzer \"${TARGET_NAME}\" has no seed data. The folder \"${CMAKE_CURRENT_LIST_DIR}/data/corpus/${TARGET_NAME}\" is missing.")
  endif()

  set(FUZZ_RUNTIME 10 CACHE STRING "Number of seconds to run fuzz tests during ctest run.")

  add_test(NAME "${TARGET_NAME}_run"
    CONFIGURATIONS Release RelWithDebInfo
    COMMAND ${TARGET_NAME} -max_total_time=${FUZZ_RUNTIME} "${CMAKE_CURRENT_LIST_DIR}/data/corpus/${TARGET_NAME}/"
  )

  set_tests_properties("${TARGET_NAME}_run" PROPERTIES LABELS "Fuzzers")

  target_compile_definitions(${TARGET_NAME} INTERFACE _DISABLE_VECTOR_ANNOTATION)

  show_build_target_properties(${TARGET_NAME})

endfunction()

#-------------------------------------------------------------------
# Build fuzzers                                        (executables)
#-------------------------------------------------------------------

# example
add_fuzzer(fuzz_me.cpp)

# fuzzers for '/src/hikogui/*'
add_fuzzer(fuzz_bigint.cpp)
add_fuzzer(fuzz_url_parser.cpp)

# fuzzers for '/src/hikogui/codec/*'
add_fuzzer(fuzz_base_n.cpp)
add_fuzzer(fuzz_bon8.cpp)

# TODO implement more fuzzers
#add_fuzzer(fuzz_gzip.cpp)
#add_fuzzer(fuzz_inflate.cpp)
#add_fuzzer(fuzz_json.cpp)
#add_fuzzer(fuzz_png.cpp)
#add_fuzzer(fuzz_sha2.cpp)
#add_fuzzer(fuzz_utf.cpp)
#add_fuzzer(fuzz_zlib.cpp)
