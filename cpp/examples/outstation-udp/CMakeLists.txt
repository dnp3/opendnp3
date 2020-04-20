add_executable(outstation-udp-demo ./main.cpp)
target_link_libraries (outstation-udp-demo PRIVATE opendnp3)
set_target_properties(outstation-udp-demo PROPERTIES FOLDER cpp/examples)
install(TARGETS outstation-udp-demo RUNTIME DESTINATION bin)
