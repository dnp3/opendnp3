add_executable(master-gprs-tls-demo ./main.cpp)
target_link_libraries (master-gprs-tls-demo PRIVATE opendnp3)
set_target_properties(master-gprs-tls-demo PROPERTIES FOLDER cpp/examples/tls)
install(TARGETS master-gprs-tls-demo RUNTIME DESTINATION bin)
