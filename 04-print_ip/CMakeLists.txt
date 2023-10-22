add_executable(print_ip print_ip.cpp)
target_include_directories(print_ip PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(print_ip PRIVATE -Wall -Wextra -pedantic -Werror)

# test for homework 4
add_executable(print_ip_test print_ip_test.cpp)
target_link_libraries(print_ip_test gtest_main)
target_compile_options(print_ip_test PRIVATE -Wall -Wextra -pedantic -Werror)

set_target_properties(print_ip print_ip_test PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

include(GoogleTest)

install(TARGETS print_ip RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_DESCRIPTION "Otus c++ learning")
set(CPACK_PACKAGE_CONTACT maratsr@ya.ru)
include(CPack)

add_test(print_ip_test print_ip_test)