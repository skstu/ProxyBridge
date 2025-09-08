project(ProxyBridge)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DENABLE_LOGGER")
set(CMAKE_CXX_FLAGS_RELEASE
    "${CMAKE_CXX_FLAGS_RELEASE} -DENABLE_LOGGER -DDISABLE_LOGGER_TO_CONSOLE -DLOGGER_DBG_VIEW"
)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO
    "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -DENABLE_LOGGER -DDISABLE_LOGGER_TO_CONSOLE -DLOGGER_DBG_VIEW"
)
set(CMAKE_CXX_FLAGS_MINSIZEREL
    "${CMAKE_CXX_FLAGS_MINSIZEREL} -DENABLE_LOGGER -DDISABLE_LOGGER_TO_CONSOLE -DLOGGER_DBG_VIEW"
)

add_executable(${PROJECT_NAME} main.cc bridge.hpp bridge.cc util.h util.cc
                               proxy_server.rc)

target_link_libraries(${PROJECT_NAME} ${PROXY_LIBRARIES})

target_compile_definitions(${PROJECT_NAME} PUBLIC -DVERSION_GIT=\"${PVERSION}\")

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "server/proxy_server")

if(WIN32)
  if(MSVC)
    # if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS
    # "/SUBSYSTEM:CONSOLE") else() endif()
    set_target_properties(
      ${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS"
                                 WIN32_EXECUTABLE YES)
  else()
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-mwindows"
                                                     WIN32_EXECUTABLE YES)
  endif()
endif()

if(MSVC
   AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.29.30129
   AND CMAKE_VERSION VERSION_GREATER 3.20.3)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
  message(STATUS "${PROJECT_NAME} set cxx standard: C++23")
else()
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
  message(STATUS "${PROJECT_NAME} set cxx standard: C++20")
endif()

if(ANDROID)
  # 查找 Android log 库.
  find_library(log-lib log)

  # target_link_libraries 目标中链接 log 库.
  target_link_libraries(${PROJECT_NAME} ${log-lib})
endif()

if(ENABLE_BUILD_WERROR)
  if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
  elseif(UNIX AND NOT APPLE)
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.1)
      target_compile_options(
        ${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra -Wno-unknown-pragmas
                                -Wno-deprecated)
    else()
      target_compile_options(
        ${PROJECT_NAME}
        PRIVATE -Werror
                -Wall
                -Wextra
                -Wno-unknown-pragmas
                -Wno-deprecated
                -Wno-deprecated-copy
                -Wno-error=deprecated-copy
                -Wno-pessimizing-move)
    endif()
  endif()
endif()
