#idf_component_register(SRCS "MaxDB.cpp" "DBObject.cpp" "JsonQuery.cpp" "HttpDBSynch.cpp"
#                    INCLUDE_DIRS "include" "nlohmann" "SQLiteCPP" "SQLite_Esp_Port" "zlib" REQUIRES "arduino-esp32" "fatfs" )

cmake_minimum_required(VERSION 3.5)

set(SQLITE_CPP_SRCS
        SQLiteCPP/Backup.cpp
        SQLiteCPP/Column.cpp
        SQLiteCPP/Database.cpp
        SQLiteCPP/Exception.cpp
        SQLiteCPP/Statement.cpp
        SQLiteCPP/Transaction.cpp
)

set(SQLITE_ESP_PORT_SRCS
        SQLite_Esp_Port/esp32.cpp
        SQLite_Esp_Port/shox96_0_2.cpp
        SQLite_Esp_Port/sqlite3.c
        SQLite_Esp_Port/unishox1.c
)

set(MaxORM_SRCS
        MaxDB.cpp
        DBObject.cpp
        JsonQuery.cpp
        HttpDBSynch.cpp
)
set(ZLib_SRCS
        zlib/adler32.c
        zlib/compress.c
        zlib/crc32.c
        zlib/deflate.c
        #        gzclose.c
        #        gzlib.c
        #        gzread.c
        #        gzwrite.c
        zlib/infback.c
        zlib/inffast.c
        zlib/inflate.c
        zlib/inftrees.c
        zlib/trees.c
        zlib/uncompr.c
        zlib/zutil.c
)

set(includedirs
        SQLiteCPP/.
        SQLite_Esp_Port/.
        zlib/.
        include/.
        nlohmann/.)
#set(EXTRA_COMPONENT_DIRS ${CMAKE_CURRENT_LIST_DIR}/nlohmann ${CMAKE_CURRENT_LIST_DIR}/zlib)


set(srcs ${SQLITE_CPP_SRCS} ${SQLITE_ESP_PORT_SRCS} ${MaxORM_SRCS} ${ZLib_SRCS})

set(requires "arduino-esp32" "fatfs"  )

if(DEFINED CONFIG_USING_PS_RAM)
    add_definitions(
            -DSPIRAM
            -DSPIRAM_SUPPORT
            -DBOARD_HAS_PSRAM
            -mfix-esp32-psram-cache-issue
            -DCONFIG_MBEDTLS_DYNAMIC_BUFFER=1
            -DCONFIG_SPIRAM_CACHE_WORKAROUND=1
            -DCONFIG_SPIRAM_TRY_ALLOCATE_WIFI_LWIP
            -DCONFIG_SPIRAM_MALLOC_ALWAYSINTERNAL=4096
    )
endif()
#if(DEFINED CONFIG_DB_USE_SDCARD)
#    set(requires "arduino" "fatfs")
##    list(APPEND ${requires} "fatfs")
#else()
#    set(requires arduino)
#endif()

idf_component_register(INCLUDE_DIRS ${includedirs} SRCS ${srcs} REQUIRES ${requires})

