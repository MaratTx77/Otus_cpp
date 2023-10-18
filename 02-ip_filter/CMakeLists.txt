
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(ip_filter ip_filter.cpp)

    target_include_directories(ip_filter PRIVATE "${CMAKE_BINARY_DIR}")
    target_compile_options(ip_filter PRIVATE -Wall -Wextra -pedantic -Werror)

    target_link_libraries(ip_filter ${Boost_LIBRARIES})
endif()

if(WIN32 AND MINGW)
    target_link_libraries(ip_filter ws2_32)
endif()

set_target_properties(ip_filter PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)

install(TARGETS ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_DESCRIPTION "Otus c++ learning")
set(CPACK_PACKAGE_CONTACT maratsr@ya.ru)
include(CPack)