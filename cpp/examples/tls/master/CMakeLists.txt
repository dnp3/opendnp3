add_executable(master-tls-demo ./main.cpp)
target_link_libraries (master-tls-demo PRIVATE opendnp3)
set_target_properties(master-tls-demo PROPERTIES FOLDER cpp/examples/tls)
install(TARGETS master-tls-demo RUNTIME DESTINATION bin)
