set(TRANSPORT_BASE_HDRS  
	librta_About.h 
	)

set(TRANSPORT_COMMON_HDRS
	common/transport.h 
	common/ccnx_TransportConfig.h 
	common/transport_Message.h 
	common/transport_MetaMessage.h 
	common/ccnx_StackConfig.h 
	common/ccnx_ConnectionConfig.h 
	)

set(TRANSPORT_TEST_TOOLS_HDRS
	test_tools/bent_pipe.h		
	test_tools/traffic_tools.h
  	)

set(TRANSPORT_RTA_HDRS
    transport_rta/rta_Transport.h
	)

set(TRANSPORT_RTA_COMMANDS_HDRS
	transport_rta/commands/rta_CommandCloseConnection.h
	transport_rta/commands/rta_CommandCreateProtocolStack.h
	transport_rta/commands/rta_CommandDestroyProtocolStack.h
	transport_rta/commands/rta_Command.h
	transport_rta/commands/rta_CommandOpenConnection.h
	transport_rta/commands/rta_CommandTransmitStatistics.h
	)

set(TRANSPORT_RTA_CONFIG_HDRS
	transport_rta/config/config_ApiConnector.h 
	transport_rta/config/config_Codec_Tlv.h 
	transport_rta/config/config_CryptoCache.h 
	transport_rta/config/config_FlowControl_Vegas.h 
	transport_rta/config/config_Forwarder_Local.h 
	transport_rta/config/config_Forwarder_Metis.h 
	transport_rta/config/config_InMemoryVerifier.h 
	transport_rta/config/config_ProtocolStack.h 
	transport_rta/config/config_PublicKeySignerPkcs12Store.h 
	transport_rta/config/config_Signer.h 
	transport_rta/config/config_SymmetricKeySignerFileStore.h 
	transport_rta/config/config_TestingComponent.h
	)

set(COMMON_SRCS   
	librta_About.c 
	common/transport.c 
	common/ccnx_TransportConfig.c 
	common/transport_Message.c 
	common/transport_MetaMessage.c 
    common/ccnx_StackConfig.c 
    common/ccnx_ConnectionConfig.c
)

set(RTA_CORE_SRCS  
	transport_rta/core/rta_ComponentStats.c 
	transport_rta/core/rta_Component.c 
	transport_rta/core/rta_Connection.c 
	transport_rta/core/rta_ConnectionTable.c 
	transport_rta/core/rta_Framework.c 
	transport_rta/core/rta_Framework_Commands.c 
	transport_rta/core/rta_Framework_Services.c 
	transport_rta/core/rta_Framework_Threaded.c 
	transport_rta/core/rta_Framework_NonThreaded.c 
	transport_rta/core/rta_Logger.c 
	transport_rta/core/rta_ProtocolStack.c 
	transport_rta/rta_Transport.c 
	test_tools/bent_pipe.c 
	test_tools/traffic_tools.c
	)

set(RTA_COMMANDS_SRCS
    transport_rta/commands/rta_Command.c 
    transport_rta/commands/rta_CommandCloseConnection.c 
    transport_rta/commands/rta_CommandCreateProtocolStack.c 
    transport_rta/commands/rta_CommandDestroyProtocolStack.c 
    transport_rta/commands/rta_CommandOpenConnection.c 
    transport_rta/commands/rta_CommandTransmitStatistics.c
	)


set(RTA_CONFIG_SRCS  
	transport_rta/config/config_ApiConnector.c 
	transport_rta/config/config_Codec_Tlv.c 
	transport_rta/config/config_FlowControl_Vegas.c 
	transport_rta/config/config_Forwarder_Local.c 
	transport_rta/config/config_Forwarder_Metis.c 
	transport_rta/config/config_TestingComponent.c 
	transport_rta/config/config_InMemoryVerifier.c 
	transport_rta/config/config_ProtocolStack.c 
	transport_rta/config/config_PublicKeySignerPkcs12Store.c 
	transport_rta/config/config_Signer.c 
	transport_rta/config/config_SymmetricKeySignerFileStore.c
	)

set(RTA_CONNECTORS_SRCS  
	transport_rta/connectors/connector_Api.c 
	transport_rta/connectors/rta_ApiConnection.c 
	transport_rta/connectors/connector_Forwarder_Local.c 
	transport_rta/connectors/connector_Forwarder_Metis.c
	)

set(RTA_COMPONENTS_SRCS  
	transport_rta/components/codec_Signing.c 
	transport_rta/components/component_Codec_Tlv.c 
	transport_rta/components/Flowcontrol_Vegas/component_Vegas.c  
	transport_rta/components/Flowcontrol_Vegas/vegas_Session.c  
	transport_rta/components/component_Testing.c
	)

set(TRANSPORT_RTA_SOURCE_FILES
  ${RTA_COMPONENTS_SRCS}
  ${RTA_CONNECTORS_SRCS}
  ${RTA_CONFIG_SRCS}
  ${RTA_COMMANDS_SRCS}
  ${RTA_CORE_SRCS}
  ${COMMON_SRCS}
  )

add_library(ccnx_transport_rta         STATIC ${TRANSPORT_RTA_SOURCE_FILES})
add_library(ccnx_transport_rta.shared  SHARED ${TRANSPORT_RTA_SOURCE_FILES})
set_target_properties(ccnx_transport_rta.shared PROPERTIES 
  C_STANDARD 99
  SOVERSION 1 
  VERSION 1.0
  OUTPUT_NAME ccnx_transport_rta )

set(libccnx_transport_rta_libraries
  ccnx_transport_rta
  ccnx_transport_rta.shared
  )

foreach(lib ${libccnx_transport_rta_libraries})
  install(TARGETS ${lib} LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
  set_property(TARGET ${lib} PROPERTY C_STANDARD 99)
  set_property(TARGET ${lib} PROPERTY INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")
endforeach()

install(FILES ${TRANSPORT_BASE_HDRS} DESTINATION include/ccnx/transport )
install(FILES ${TRANSPORT_COMMON_HDRS} DESTINATION include/ccnx/transport/common )
install(FILES ${TRANSPORT_TEST_TOOLS_HDRS} DESTINATION include/ccnx/transport/test_tools )
install(FILES ${TRANSPORT_RTA_HDRS} DESTINATION include/ccnx/transport/transport_rta )
install(FILES ${TRANSPORT_RTA_CONFIG_HDRS} DESTINATION include/ccnx/transport/transport_rta/config )
install(FILES ${TRANSPORT_RTA_COMMANDS_HDRS} DESTINATION include/ccnx/transport/transport_rta/commands )
	
add_subdirectory(common/test)
add_subdirectory(transport_rta/test)
add_subdirectory(transport_rta/commands/test)
add_subdirectory(transport_rta/components/test)
add_subdirectory(transport_rta/config/test)
add_subdirectory(transport_rta/connectors/test)
add_subdirectory(transport_rta/core/test)
