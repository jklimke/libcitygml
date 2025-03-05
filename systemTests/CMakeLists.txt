include(FetchContent)

if (NOT ${LIBCITYGML_STATIC_CRT})
	set(gtest_force_shared_crt ON)
endif()

set(BUILD_GMOCK OFF)

FetchContent_Declare(
  gtest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2)
FetchContent_MakeAvailable(gtest)

add_executable(citygmlSystemTests
	systemTestsMain.cpp
	FileReadTests.cpp
)

target_include_directories(citygmlSystemTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../sources/include)

target_link_libraries(citygmlSystemTests PRIVATE citygml gtest)

install(TARGETS citygmlSystemTests RUNTIME DESTINATION ${BIN_INSTALL_DIR})
