file(GLOB files "src/unit-*.cpp")
foreach(file ${files})
    get_filename_component(testcase ${file} NAME_WE)
    add_executable(${testcase} "src/catch.hpp" ${file})
    set_target_properties(${testcase} PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
        COMPILE_DEFINITIONS "$<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>"
        COMPILE_OPTIONS "$<$<CXX_COMPILER_ID:MSVC>:/EHsc;$<$<CONFIG:Release>:/Od>>"
    )

    target_include_directories(${testcase} PRIVATE "src")
    target_link_libraries(${testcase} ${JSON_TARGET_NAME})
    target_compile_definitions(${testcase} PRIVATE CATCH_CONFIG_MAIN)

    add_test(NAME "${testcase}_default"
       COMMAND ${testcase}
       WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

    add_test(NAME "${testcase}_all"
       COMMAND ${testcase} "*"
       WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endforeach()
