add_executable(outstation-tls-demo ./main.cpp)
target_link_libraries (outstation-tls-demo PRIVATE opendnp3)
set_target_properties(outstation-tls-demo PROPERTIES FOLDER cpp/examples/tls)
install(TARGETS outstation-tls-demo RUNTIME DESTINATION bin)
