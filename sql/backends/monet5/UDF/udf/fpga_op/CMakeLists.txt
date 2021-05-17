#cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
find_package(Vitis)

# Platform
set(DSA_NAME "xilinx_u280_xdma_201920_3" CACHE STRING "DSA string for v++/xocc.")
#set(DSA_NAME "xilinx-u50-gen3x16-xdma-dev" CACHE STRING "DSA string for v++/xocc.")

#set(HOST_NAME "hbm_bw" CACHE STRING "Host executable name.")
set(KERNEL_NAME "krnl_fpga_op" CACHE STRING "Host executable name.")

set(DB_DIR "~/Workspace/fpl20ext/test_db" CACHE PATH "Root path of db, the fpga bin (.xclbin) will be copied to.")

# Common src and head that comes with Xilinx VITIS example
set(OCL_COMMON_SRC ${CMAKE_CURRENT_SOURCE_DIR}/common/includes/xcl2/xcl2.cpp)
set(OCL_COMMON_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/common/includes/xcl2)

##FIXME: should VIVADO_INCLUDE be added in to FindVitis?
set(CXXFLAGS -I${Vitis_INCLUDE_DIRS} -I{OCL_COMMON_INCLUDE})
set(LDFLAGS -L${Vitis_LIBRARIES} -lOpenCL -pthread)
set(HOST_SRC ${OCL_COMMON_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/src/fpga_op.cpp)
set(KERNEL_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/${KERNEL_NAME}.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

include_directories(SYSTEM ${Vitis_INCLUDE_DIRS} ${OCL_COMMON_INCLUDE})
## Host
#add_executable(${HOST_NAME} ${HOST_SRC})
#target_link_libraries(${HOST_NAME} ${Vitis_LIBRARIES} pthread)

add_library(fpga_op SHARED ${HOST_SRC})
target_link_libraries(fpga_op ${Vitis_LIBRARIES} pthread)
target_include_directories (fpga_op PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

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

#FIXME: only support one kernel now
function(binary_container_build TARGET)
    set(TEMP_DIR ${CMAKE_BINARY_DIR}/_x.${TARGET}.${DSA_NAME})
    set(VPP_FLAGS -t ${TARGET} --platform ${DSA_NAME} --save-temps)
    if(NOT ${TARGET} EQUAL "hw")
        set(VPP_FLAGS ${VPP_FLAGS} -g)
    endif()

    add_custom_command(
        OUTPUT ${TEMP_DIR}/${KERNEL_NAME}.xo
        COMMENT "Compile kernel code (.xo)."
        COMMAND ${Vitis_COMPILER} ${VPP_FLAGS} -c -k ${KERNEL_NAME} --temp_dir ${TEMP_DIR}
                -I ${CMAKE_CURRENT_SOURCE_DIR}/src
                -o ${TEMP_DIR}/${KERNEL_NAME}.xo
                ${KERNEL_SRC}
        DEPENDS ${KERNEL_SRC})
    set(BUILD_DIR ${CMAKE_BINARY_DIR}/build_dir.${TARGET}.${DSA_NAME})

    # check if kernel config file (.cfg) exist
    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${KERNEL_NAME}.cfg)
        set(VPP_LDFLAGS_KERNEL --config ${CMAKE_CURRENT_SOURCE_DIR}/${KERNEL_NAME}.cfg)
    endif()

    add_custom_command(
        OUTPUT ${BUILD_DIR}/${KERNEL_NAME}.xclbin
        COMMENT "Link kernel code (.xclbin)"
        COMMAND ${Vitis_COMPILER} ${VPP_FLAGS} -l ${VPP_LDFLAGS} --temp_dir ${TEMP_DIR} ${VPP_LDFLAGS_KERNEL}
                -o ${BUILD_DIR}/${KERNEL_NAME}.link.xclbin ${TEMP_DIR}/${KERNEL_NAME}.xo
        COMMAND ${Vitis_COMPILER} -p ${BUILD_DIR}/${KERNEL_NAME}.link.xclbin -t ${TARGET} --platform ${DSA_NAME}
                --package.out_dir ${CMAKE_BINARY_DIR}/package.${TARGET} -o ${BUILD_DIR}/${KERNEL_NAME}.xclbin
        DEPENDS ${TEMP_DIR}/${KERNEL_NAME}.xo)
endfunction()

set(BUILD_MODE "sw_emu" "hw_emu" "hw")
foreach(mode IN LISTS BUILD_MODE)
    message(STATUS "Add MODE ${mode}")
    binary_container_build(${mode})
endforeach()


add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/emconfig.json
    COMMENT "Generating emconfig.json file for hardware emulation."
    COMMAND ${VITIS_ROOT}/bin/emconfigutil --platform ${DSA_NAME})

add_custom_target(sw_emu
    COMMENT "Build for software simulation."
    COMMAND cp ${CMAKE_BINARY_DIR}/build_dir.sw_emu.${DSA_NAME}/${KERNEL_NAME}.xclbin ${DB_DIR}/
    DEPENDS ${CMAKE_BINARY_DIR}/emconfig.json
            ${CMAKE_BINARY_DIR}/build_dir.sw_emu.${DSA_NAME}/${KERNEL_NAME}.xclbin)
add_custom_target(hw_emu
    COMMENT "Build for hardware simulation."
    COMMAND cp ${CMAKE_BINARY_DIR}/build_dir.hw_emu.${DSA_NAME}/${KERNEL_NAME}.xclbin ${DB_DIR}/
    DEPENDS ${CMAKE_BINARY_DIR}/emconfig.json
            ${CMAKE_BINARY_DIR}/build_dir.hw_emu.${DSA_NAME}/${KERNEL_NAME}.xclbin)
add_custom_target(hw
    COMMENT "Build hardware."
    COMMAND cp ${CMAKE_BINARY_DIR}/build_dir.hw.${DSA_NAME}/${KERNEL_NAME}.xclbin ${DB_DIR}/
    DEPENDS ${CMAKE_BINARY_DIR}/build_dir.hw.${DSA_NAME}/${KERNEL_NAME}.xclbin)
