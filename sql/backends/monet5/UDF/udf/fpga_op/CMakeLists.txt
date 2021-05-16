
add_library(fpga_op SHARED fpga_op.c)
target_include_directories (fpga_op PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS
        fpga_op
        LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/monetdb5
        COMPONENT server
        ARCHIVE
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/monetdb5
        PUBLIC_HEADER
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/monetdb
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})