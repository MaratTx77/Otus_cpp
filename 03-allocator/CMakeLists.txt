# homework 3
# add_compile_definitions(NDEBUG)
add_executable(allocator allocator.cpp)
target_include_directories(allocator PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(allocator PRIVATE -Wall -Wextra -pedantic -Werror)

set_target_properties(allocator PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

install(TARGETS allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_DESCRIPTION "Otus c++ learning")
set(CPACK_PACKAGE_CONTACT maratsr@ya.ru)
include(CPack)
