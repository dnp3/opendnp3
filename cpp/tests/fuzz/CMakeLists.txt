
add_executable (fuzzdecoder ./onefile.cpp ./fuzzdecoder.cpp)
target_link_libraries (fuzzdecoder PRIVATE opendnp3)
set_target_properties(fuzzdecoder PROPERTIES FOLDER cpp/tests/fuzzing)
clang_format(fuzzdecoder)
clang_tidy(fuzzdecoder)

add_executable (fuzzoutstation ./onefile.cpp ./fuzzoutstation.cpp)
target_link_libraries (fuzzoutstation PRIVATE opendnp3)
set_target_properties(fuzzoutstation PROPERTIES FOLDER cpp/tests/fuzzing)
clang_format(fuzzoutstation)
clang_tidy(fuzzoutstation)

add_executable (fuzzmaster ./onefile.cpp ./fuzzmaster.cpp)
target_link_libraries (fuzzmaster PRIVATE opendnp3)
set_target_properties(fuzzmaster PROPERTIES FOLDER cpp/tests/fuzzing)
clang_format(fuzzmaster)
clang_tidy(fuzzmaster)
