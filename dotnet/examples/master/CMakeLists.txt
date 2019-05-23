enable_language(CSharp)

add_executable(master-dotnet Program.cs)

set_target_properties(master-dotnet PROPERTIES FOLDER "dotnet/examples")
set_property(TARGET master-dotnet PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION ${DNP3_DOTNET_FRAMEWORK_VERSION})
target_link_libraries(master-dotnet DNP3CLRAdapter)