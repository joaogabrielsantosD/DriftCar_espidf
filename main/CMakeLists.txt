set(ALL_LIBRARIES DriftCar
    SOC
    LEDs
    ACC)

set(LIBRARIES_DriftCar_SCRS lib/DriftCar/DriftCar.c)
set(LIBRARIES_SOC_SCRS lib/SOC/SOC.c)
set(LIBRARIES_LEDs_SCRS lib/LEDs/LEDs.c)
set(LIBRARIES_ACC_SCRS lib/ACC/ACC.c)

set(LIBRARIES_SCRS)
set(LIBRARIES_INCLUDE)
set(LIBRARIES_REQUIRE)
foreach(libname IN LISTS ALL_LIBRARIES)
    if(LIBRARIES_${libname}_SCRS)
        list(APPEND LIBRARIES_SCRS ${LIBRARIES_${libname}_SCRS})
    endif()

    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/lib/${libname})
        list(APPEND LIBRARIES_INCLUDE lib/${libname})
        list(APPEND LIBRARIES_REQUIRE ${libname})
    endif()
endforeach()

set(srcs ${LIBRARIES_SCRS} src/main.c)
set(includedirs ${LIBRARIES_INCLUDE} include)
set(requires ${LIBRARIES_REQUIRE} MX1508 ahrs mpu9250 Servo DEBUG_SYSTEM)
set(priv_requires vfs nvs_flash espcoredump driver esp_timer esp_netif esp_wifi esp_adc)

idf_component_register(SRCS ${srcs} INCLUDE_DIRS ${includedirs} REQUIRES ${requires} PRIV_REQUIRES ${priv_requires})
