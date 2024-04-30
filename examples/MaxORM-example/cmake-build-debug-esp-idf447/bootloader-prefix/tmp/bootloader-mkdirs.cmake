# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/maxime/Documents/GitHub/esp/esp-idf-4.4.7/components/bootloader/subproject"
  "/home/maxime/Documents/GitHub/JygaComponents/MaxORM447/examples/MaxORM-example/cmake-build-debug-esp-idf447/bootloader"
  "/home/maxime/Documents/GitHub/JygaComponents/MaxORM447/examples/MaxORM-example/cmake-build-debug-esp-idf447/bootloader-prefix"
  "/home/maxime/Documents/GitHub/JygaComponents/MaxORM447/examples/MaxORM-example/cmake-build-debug-esp-idf447/bootloader-prefix/tmp"
  "/home/maxime/Documents/GitHub/JygaComponents/MaxORM447/examples/MaxORM-example/cmake-build-debug-esp-idf447/bootloader-prefix/src/bootloader-stamp"
  "/home/maxime/Documents/GitHub/JygaComponents/MaxORM447/examples/MaxORM-example/cmake-build-debug-esp-idf447/bootloader-prefix/src"
  "/home/maxime/Documents/GitHub/JygaComponents/MaxORM447/examples/MaxORM-example/cmake-build-debug-esp-idf447/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/maxime/Documents/GitHub/JygaComponents/MaxORM447/examples/MaxORM-example/cmake-build-debug-esp-idf447/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/maxime/Documents/GitHub/JygaComponents/MaxORM447/examples/MaxORM-example/cmake-build-debug-esp-idf447/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
