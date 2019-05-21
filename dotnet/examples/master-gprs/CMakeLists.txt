enable_language(CSharp)

add_executable(master-dotnet-gprs Program.cs)

set_target_properties(master-dotnet-gprs PROPERTIES FOLDER "dotnet/examples")
set_property(TARGET master-dotnet-gprs PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION ${CLR_VERSION})
target_link_libraries(master-dotnet-gprs DNP3CLRAdapter)