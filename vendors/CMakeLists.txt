project(defer.hpp.vendors)

set(DOCTEST_NO_INSTALL ON CACHE INTERNAL "")
add_subdirectory(doctest)
set_target_properties(doctest_with_main PROPERTIES FOLDER defer.hpp.vendors)
