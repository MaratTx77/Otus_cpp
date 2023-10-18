# homework 1
add_executable(helloworld_cli helloworld.cpp)
target_link_libraries(helloworld_cli PRIVATE common_lib)
target_include_directories(helloworld_cli PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(helloworld_cli PRIVATE -Wall -Wextra -pedantic -Werror)

# test for homework 1
add_executable(helloworld_test helloworld_test.cpp)
target_link_libraries(helloworld_test common_lib)
target_link_libraries(helloworld_test gtest_main)
target_compile_options(helloworld_test PRIVATE -Wall -Wextra -pedantic -Werror)

set_target_properties(helloworld_cli helloworld_test PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)

include(GoogleTest)

install(TARGETS helloworld_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_DESCRIPTION "Otus c++ learning")
set(CPACK_PACKAGE_CONTACT maratsr@ya.ru)
include(CPack)

add_test(helloworld_test helloworld_test)