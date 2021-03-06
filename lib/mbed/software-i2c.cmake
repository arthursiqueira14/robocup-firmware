# Software I2C allows non-standard I2C pins to be used for interfacing with I2C devices

ExternalProject_Add(swi2c_library
    HG_REPOSITORY       https://developer.mbed.org/users/p3p/code/SoftwareI2C
    HG_TAG              2:8670e78c4b63
    CONFIGURE_COMMAND   ""
    BUILD_COMMAND       ""
    INSTALL_COMMAND     ""
    UPDATE_COMMAND      ""
)
set_target_properties(swi2c_library PROPERTIES EXCLUDE_FROM_ALL TRUE)

# the directory to include for linking in with the firm-lib library
ExternalProject_Get_Property(swi2c_library SOURCE_DIR)

# the source files that will be added to firm-lib
set(SWI2C_SRC ${SOURCE_DIR}/SoftwareI2C.cpp)

# add the external project's path/src info into the accessory library lists
set(MBED_ASSEC_LIBS_INCLUDES    ${MBED_ASSEC_LIBS_INCLUDES} ${SOURCE_DIR}   )
set(MBED_ASSEC_LIBS_SRCS        ${MBED_ASSEC_LIBS_SRCS}     ${SWI2C_SRC}    )
set(MBED_ASSEC_LIBS_DEPENDS     ${MBED_ASSEC_LIBS_DEPENDS}  swi2c_library   )
