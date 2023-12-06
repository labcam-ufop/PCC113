# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/esp-idf/components/bootloader/subproject"
  "C:/PCC113/PCC113/sample_semaph2/build/bootloader"
  "C:/PCC113/PCC113/sample_semaph2/build/bootloader-prefix"
  "C:/PCC113/PCC113/sample_semaph2/build/bootloader-prefix/tmp"
  "C:/PCC113/PCC113/sample_semaph2/build/bootloader-prefix/src/bootloader-stamp"
  "C:/PCC113/PCC113/sample_semaph2/build/bootloader-prefix/src"
  "C:/PCC113/PCC113/sample_semaph2/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/PCC113/PCC113/sample_semaph2/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/PCC113/PCC113/sample_semaph2/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
