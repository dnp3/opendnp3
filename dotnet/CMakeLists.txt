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

set(CPACK_NUGET_PACKAGE_DESCRIPTION_SUMMARY "OpenDNP3 is the de facto reference implementation of IEEE-1815 (DNP3), a standards-based SCADA protocol.")
set(CPACK_NUGET_PACKAGE_COPYRIGHT "Green Energy Corp, Automatak LLC, and contributors")
set(CPACK_NUGET_PACKAGE_LICENSEURL "https://github.com/dnp3/opendnp3/blob/develop/LICENSE")
