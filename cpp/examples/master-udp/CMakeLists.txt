add_executable(master-udp-demo ./main.cpp)
target_link_libraries (master-udp-demo PRIVATE opendnp3)
set_target_properties(master-udp-demo PROPERTIES FOLDER cpp/examples)
install(TARGETS master-udp-demo RUNTIME DESTINATION bin)
