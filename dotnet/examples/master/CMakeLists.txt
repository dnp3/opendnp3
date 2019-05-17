enable_language(CSharp)

add_executable(master-dotnet Program.cs)

set_target_properties(master-dotnet PROPERTIES FOLDER "dotnet/examples")
set_property(TARGET master-dotnet PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION ${CLR_VERSION})
target_link_libraries(master-dotnet DNP3CLRAdapter)