enable_language(CSharp)

add_executable(master-dotnet-gprs-tls Program.cs)

set_target_properties(master-dotnet-gprs-tls PROPERTIES FOLDER "dotnet/examples")
set_property(TARGET master-dotnet-gprs-tls PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION ${DNP3_DOTNET_FRAMEWORK_VERSION})
target_link_libraries(master-dotnet-gprs-tls DNP3CLRAdapter)