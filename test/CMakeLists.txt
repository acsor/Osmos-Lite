# TO-DO Set an output file extension for executable files
add_executable(
	utest
	$<TARGET_OBJECTS:test-main> $<TARGET_OBJECTS:test-particle-system>
)
target_link_libraries(utest Parsys)

add_library(test-main OBJECT test-main.cpp)
add_library(test-particle-system OBJECT test-particle-system.cpp)
