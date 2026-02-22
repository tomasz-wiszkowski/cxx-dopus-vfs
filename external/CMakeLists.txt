cmake_minimum_required(VERSION 3.20)

project(OpusSDK VERSION 1.0 LANGUAGES C CXX)

# Download and extract Directory Opus SDK
set(DOPUS_SDK_DIR "${PROJECT_SOURCE_DIR}/dopus_sdk")
set(DOPUS_SDK_ZIP "${PROJECT_SOURCE_DIR}/opus_sdk.zip")
set(DOPUS_SDK_URL "https://cdn2.gpsoft.com.au/files/Misc/opus_sdk.zip")
set(DOPUS_INCLUDE_DIR ${DOPUS_SDK_DIR}/headers)

if(NOT EXISTS "${DOPUS_SDK_DIR}/headers")
    message(STATUS "Downloading Directory Opus SDK...")

    file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}")

    if(NOT EXISTS "${DOPUS_SDK_ZIP}")
        file(DOWNLOAD "${DOPUS_SDK_URL}" "${DOPUS_SDK_ZIP}"
             SHOW_PROGRESS
             STATUS DOWNLOAD_STATUS)
        list(GET DOWNLOAD_STATUS 0 DOWNLOAD_RESULT)
        if(NOT DOWNLOAD_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to download Directory Opus SDK")
        endif()
    endif()

    message(STATUS "Extracting Directory Opus SDK...")
    file(ARCHIVE_EXTRACT INPUT "${DOPUS_SDK_ZIP}" DESTINATION "${DOPUS_SDK_DIR}")
endif()

add_library(OpusSDK::headers INTERFACE IMPORTED GLOBAL)
set_target_properties(OpusSDK::headers PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${DOPUS_INCLUDE_DIR}"
)
