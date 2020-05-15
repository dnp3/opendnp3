string(REGEX REPLACE "[^0-9]" "" framework_folder_name ${DNP3_DOTNET_FRAMEWORK_VERSION})
string(PREPEND "net" framework_folder_name)

add_subdirectory(./CLRInterface)
add_subdirectory(./CLRAdapter)

if(DNP3_EXAMPLES)
    add_subdirectory(./examples/master)
    add_subdirectory(./examples/master-gprs)
    add_subdirectory(./examples/master-gprs-tls)
    add_subdirectory(./examples/outstation)
endif()

# CMake NuGet generation support
set(CPACK_NUGET_PACKAGE_DESCRIPTION_SUMMARY "OpenDNP3 is the de facto reference implementation of IEEE-1815 (DNP3), a standards-based SCADA protocol.")
set(CPACK_NUGET_PACKAGE_COPYRIGHT "Green Energy Corp, Automatak LLC, and contributors")
set(CPACK_NUGET_PACKAGE_LICENSEURL "https://github.com/dnp3/opendnp3/blob/develop/LICENSE")

# Manual NuGet generation (to support both x86 and x64)
configure_file(./nuget/.nuspec nuget/.nuspec @ONLY)
configure_file(./nuget/build/opendnp3.props nuget/build/opendnp3.props COPYONLY)
configure_file(./nuget/build/opendnp3.targets nuget/build/opendnp3.targets COPYONLY)
