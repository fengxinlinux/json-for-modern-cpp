option(UseValgrind "Run the unit tests with Valgrind" OFF)
if(UseValgrind)
    set(test_prefix "valgrind --error-exitcode=1 --leak-check=full")
else()
    set(test_prefix "")
endif()


file(GLOB files "src/unit-*.cpp")
foreach(file ${files})
    get_filename_component(file_basename ${file} NAME_WE)
    string(REGEX REPLACE "unit-([^$]+)" "test-\\1" testcase ${file_basename})

    add_executable(${testcase} "src/catch.hpp" ${file})
    set_target_properties(${testcase} PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
        COMPILE_DEFINITIONS "$<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>"
        COMPILE_OPTIONS "$<$<CXX_COMPILER_ID:MSVC>:/EHsc;$<$<CONFIG:Release>:/Od>>"
    )

    target_link_libraries(${testcase} ${JSON_TARGET_NAME})
    target_compile_definitions(${testcase} PRIVATE CATCH_CONFIG_MAIN)

    set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -Weffc++ -Wcast-align -Wcast-qual -Wno-ctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=5 -Wswitch -Wundef -Wno-unused -Wnon-virtual-dtor -Wreorder -Wdeprecated -Wno-float-equal")

    add_test(NAME "${testcase}_default"
      COMMAND ${test_prefix} ${testcase}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    set_tests_properties("${testcase}_default" PROPERTIES LABELS "default")

    add_test(NAME "${testcase}_all"
      COMMAND ${test_prefix} ${testcase} "*"
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    set_tests_properties("${testcase}_all" PROPERTIES LABELS "all")

endforeach()
