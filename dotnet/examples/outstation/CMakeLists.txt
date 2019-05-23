enable_language(CSharp)

add_executable(outstation-dotnet Program.cs)

set_target_properties(outstation-dotnet PROPERTIES FOLDER "dotnet/examples")
set_property(TARGET outstation-dotnet PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION ${DNP3_DOTNET_FRAMEWORK_VERSION})
target_link_libraries(outstation-dotnet DNP3CLRAdapter)