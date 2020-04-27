add_executable(master-demo ./main.cpp)
target_link_libraries (master-demo PRIVATE opendnp3)
set_target_properties(master-demo PROPERTIES FOLDER cpp/examples)
install(TARGETS master-demo RUNTIME DESTINATION bin)
