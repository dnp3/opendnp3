add_executable(master-gprs-demo ./main.cpp ./ExampleListenCallbacks.h ./ExampleListenCallbacks.cpp)
target_link_libraries (master-gprs-demo PRIVATE opendnp3)
set_target_properties(master-gprs-demo PROPERTIES FOLDER cpp/examples)
install(TARGETS master-gprs-demo RUNTIME DESTINATION bin)
