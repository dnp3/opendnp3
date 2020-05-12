set(asiotests_headers
    ./mocks/MockIO.h
    ./mocks/MockTCPClientHandler.h
    ./mocks/MockTCPPair.h
    ./mocks/MockTCPServer.h
)

set(asiotests_src
    ./main.cpp

    ./TestStrandExecutor.cpp
    ./TestTCPClientServer.cpp

    ./mocks/MockIO.cpp
    ./mocks/MockTCPClientHandler.cpp
    ./mocks/MockTCPPair.cpp
)

set(asiotests_tls_headers
    ./tls/mocks/MockTLSClientHandler.h
    ./tls/mocks/MockTLSPair.h
    ./tls/mocks/MockTLSServer.h
)

set(asiotests_tls_src
    ./tls/TestTLSClientServer.cpp

    ./tls/mocks/MockTLSPair.cpp
)

set(all_sources ${asiotests_headers} ${asiotests_src})
if(DNP3_TLS)
    list(APPEND all_sources ${asiotests_tls_headers} ${asiotests_tls_src})
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${all_sources}
)

add_executable(asiotests
    ${all_sources}
)
target_compile_features(asiotests PRIVATE cxx_std_14)
target_link_libraries(asiotests PRIVATE catch dnp3mocks opendnp3 ser4cpp exe4cpp asio)
target_include_directories(asiotests PRIVATE ./ ../../lib/src)
set_target_properties(asiotests PROPERTIES FOLDER cpp/tests)
add_test(NAME asiotests COMMAND asiotests)

if(DNP3_TLS)
    target_link_libraries(asiotests PRIVATE OpenSSL::SSL)
    target_compile_definitions(asiotests PRIVATE OPENDNP3_USE_TLS)
    
    configure_file(./certs/self_signed/entity1_cert.pem ./certs/self_signed/entity1_cert.pem COPYONLY)
    configure_file(./certs/self_signed/entity1_key.pem ./certs/self_signed/entity1_key.pem COPYONLY)
    configure_file(./certs/self_signed/entity2_cert.pem ./certs/self_signed/entity2_cert.pem COPYONLY)
    configure_file(./certs/self_signed/entity2_key.pem ./certs/self_signed/entity2_key.pem COPYONLY)
    configure_file(./certs/ca_chain/entity1_cert.pem ./certs/ca_chain/entity1_cert.pem COPYONLY)
    configure_file(./certs/ca_chain/entity1_key.pem ./certs/ca_chain/entity1_key.pem COPYONLY)
    configure_file(./certs/ca_chain/entity2_cert.pem ./certs/ca_chain/entity2_cert.pem COPYONLY)
    configure_file(./certs/ca_chain/entity2_key.pem ./certs/ca_chain/entity2_key.pem COPYONLY)
    configure_file(./certs/ca_chain/ca_cert.pem ./certs/ca_chain/ca_cert.pem COPYONLY)
endif()

clang_format(asiotests)
clang_tidy(asiotests)
