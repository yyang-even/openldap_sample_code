cmake_minimum_required(VERSION 2.8.12.2)

project (openldap_sample_code)

###################################################
# gcc
###################################################
# Using C++14
set(CMAKE_CXX_FLAGS       "${CMAKE_CXX_FLAGS}       -std=c++1y")
set(CMAKE_CXX_FLAGS       "${CMAKE_CXX_FLAGS}       -g")
# Turn warning about any return statement with no return value in a function whose return type is not void
set(CMAKE_CXX_FLAGS       "${CMAKE_CXX_FLAGS}       -Werror=return-type")

include_directories ("${PROJECT_SOURCE_DIR}")

###################################################
# Options
###################################################
# define option explicitly for pretty ccmake/cmake-gui
option(BUILD_SHARED_LIBS "Build libraries as .dll's/.so's" true)
mark_as_advanced(CLEAR BUILD_SHARED_LIBS)# mark un-advanced


###################################################
set(OPENLDAP_LIBRARIES
    ldap
    lber
)

add_executable(set_debug_level set_debug_level.cpp)
target_link_libraries(set_debug_level
    ${OPENLDAP_LIBRARIES}
)

add_executable(synchronous_anonymous_search synchronous_anonymous_search.cpp)
target_link_libraries(synchronous_anonymous_search
    ${OPENLDAP_LIBRARIES}
)

add_executable(synch_search_with_filter synch_search_with_filter.cpp)
target_link_libraries(synch_search_with_filter
    ${OPENLDAP_LIBRARIES}
)

add_executable(synch_search_with_attributes synch_search_with_attributes.cpp)
target_link_libraries(synch_search_with_attributes
    ${OPENLDAP_LIBRARIES}
)

add_executable(synchronous_bind_search synchronous_bind_search.cpp)
target_link_libraries(synchronous_bind_search
    ${OPENLDAP_LIBRARIES}
)
