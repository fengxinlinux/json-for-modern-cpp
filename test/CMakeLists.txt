include_directories("thirdparty/catch")

#############################################################################
# unit tests
#############################################################################

if(COVERALLS)
    file(GLOB files "src/unit-*.cpp")
    set(testcase "test-all")
    add_executable(${testcase} "thirdparty/catch/catch.hpp" "src/unit.cpp" ${files})
    set_target_properties(${testcase} PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
        COMPILE_DEFINITIONS "$<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>"
        COMPILE_OPTIONS "$<$<CXX_COMPILER_ID:MSVC>:/EHsc;$<$<CONFIG:Release>:/Od>>"
    )

    target_link_libraries(${testcase} ${JSON_TARGET_NAME})

    if (NOT MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Weffc++ -Wcast-align -Wcast-qual -Wno-ctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=5 -Wswitch -Wundef -Wno-unused -Wnon-virtual-dtor -Wreorder -Wdeprecated -Wno-float-equal")
    endif()

    add_test(NAME "${testcase}_all"
      COMMAND ${testcase} "*"
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    set_tests_properties("${testcase}_all" PROPERTIES LABELS "all")
else()
    file(GLOB files "src/unit-*.cpp")
    foreach(file ${files})
        get_filename_component(file_basename ${file} NAME_WE)
        string(REGEX REPLACE "unit-([^$]+)" "test-\\1" testcase ${file_basename})

        add_executable(${testcase} "thirdparty/catch/catch.hpp" ${file})
        set_target_properties(${testcase} PROPERTIES
            CXX_STANDARD 11
            CXX_STANDARD_REQUIRED ON
            COMPILE_DEFINITIONS "$<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>"
            COMPILE_OPTIONS "$<$<CXX_COMPILER_ID:MSVC>:/EHsc;$<$<CONFIG:Release>:/Od>>"
        )

        target_link_libraries(${testcase} ${JSON_TARGET_NAME})
        target_compile_definitions(${testcase} PRIVATE CATCH_CONFIG_MAIN)

        if (NOT MSVC)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Weffc++ -Wcast-align -Wcast-qual -Wno-ctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=5 -Wswitch -Wundef -Wno-unused -Wnon-virtual-dtor -Wreorder -Wdeprecated -Wno-float-equal")
        endif()

        add_test(NAME "${testcase}_default"
          COMMAND ${testcase}
          WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        )
        set_tests_properties("${testcase}_default" PROPERTIES LABELS "default")

        add_test(NAME "${testcase}_all"
          COMMAND ${testcase} "*"
          WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        )
        set_tests_properties("${testcase}_all" PROPERTIES LABELS "all")

    endforeach()
endif()


#############################################################################
# fuzzer tests
#############################################################################

add_executable("parse_json_fuzzer" "src/fuzzer-driver_afl.cpp" "src/fuzzer-parse_json.cpp")
set_target_properties("parse_json_fuzzer" PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    COMPILE_DEFINITIONS "$<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>"
    COMPILE_OPTIONS "$<$<CXX_COMPILER_ID:MSVC>:/EHsc;$<$<CONFIG:Release>:/Od>>"
)
target_link_libraries("parse_json_fuzzer" ${JSON_TARGET_NAME})

add_executable("parse_cbor_fuzzer" "src/fuzzer-driver_afl.cpp" "src/fuzzer-parse_cbor.cpp")
set_target_properties("parse_cbor_fuzzer" PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    COMPILE_DEFINITIONS "$<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>"
    COMPILE_OPTIONS "$<$<CXX_COMPILER_ID:MSVC>:/EHsc;$<$<CONFIG:Release>:/Od>>"
)
target_link_libraries("parse_cbor_fuzzer" ${JSON_TARGET_NAME})

add_executable("parse_msgpack_fuzzer" "src/fuzzer-driver_afl.cpp" "src/fuzzer-parse_msgpack.cpp")
set_target_properties("parse_msgpack_fuzzer" PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    COMPILE_DEFINITIONS "$<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>"
    COMPILE_OPTIONS "$<$<CXX_COMPILER_ID:MSVC>:/EHsc;$<$<CONFIG:Release>:/Od>>"
)
target_link_libraries("parse_msgpack_fuzzer" ${JSON_TARGET_NAME})
