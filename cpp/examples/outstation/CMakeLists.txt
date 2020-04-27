add_executable(outstation-demo ./main.cpp)
target_link_libraries (outstation-demo PRIVATE opendnp3)
set_target_properties(outstation-demo PROPERTIES FOLDER cpp/examples)
install(TARGETS outstation-demo RUNTIME DESTINATION bin)
