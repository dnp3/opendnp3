add_executable(decoder ./main.cpp)
target_link_libraries (decoder PRIVATE opendnp3)
set_target_properties(decoder PROPERTIES FOLDER cpp/examples)
install(TARGETS decoder RUNTIME DESTINATION bin)
