function(AddCoverage target)
  find_program(LCOV_PATH lcov REQUIRED)
  find_program(GENHTML_PATH genhtml REQUIRED)
  add_custom_target(coverage-${target}
    COMMAND ${LCOV_PATH} -d ${CMAKE_BINARY_DIR} --zerocounters
    COMMAND $<TARGET_FILE:${target}>
    COMMAND ${LCOV_PATH} -d ${CMAKE_BINARY_DIR} --capture -o coverage.info
    COMMAND ${LCOV_PATH} -r coverage.info '/usr/include/*'
                         '${CMAKE_BINARY_DIR}/_deps/*'
                         -o filtered.info
    COMMAND ${GENHTML_PATH} -o ${CMAKE_BINARY_DIR}/coverage-${target}
                            filtered.info --legend
    COMMAND ${CMAKE_COMMAND} -E copy filtered.info 
            ${CMAKE_BINARY_DIR}/coverage-${target}/coverage.info
    COMMAND ${CMAKE_COMMAND} -E rm -rf coverage.info filtered.info
    WORKING_DIRECTORY $<TARGET_FILE_DIR:${target}>
  )
endfunction()

function(CleanCoverage target)
  add_custom_command(TARGET ${target} PRE_BUILD COMMAND
                     find ${CMAKE_BINARY_DIR} -type f
                     -name '*.gcda' -exec cmake -E rm {} +)
endfunction()

function(InstrumentForCoverage target)
  if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_options(${target} 
                           PRIVATE --coverage -fno-inline)
    target_link_options(${target} PUBLIC --coverage)
  endif()
endfunction()
