# - Find Log4cpp
# Find the native LOG4CPP includes and library
#
#  LOG4CPP_INCLUDE_DIRS - where to find LOG4CPP.h, etc.
#  LOG4CPP_LIBRARIES   - List of libraries when using LOG4CPP.
#  LOG4CPP_FOUND       - True if LOG4CPP found.
 
# 判断是否已经包含log4cpp
if (LOG4CPP_INCLUDE_DIR)
  set(LOG4CPP_FIND_QUIETLY TRUE)
endif ()
 
# 查找头文件位置
# PATH_SUFFIXES 路径后缀，正常安装的log4cpp位于系统路径/log4cpp/文件夹下
find_path(LOG4CPP_INCLUDE_DIR
  NAMES Category.hh
  # 可以通过以下命令来手动制定查找路径
  # PATHS /usr/local/include
  PATH_SUFFIXES log4cpp
  DOC "Log4cpp include directories"
)
 
# 查找库文件位置
find_library(LOG4CPP_LIBRARY
  NAMES log4cpp
  DOC "Log4cpp library"
)
 
# 同时找到头文件位置和库文件位置时给相关变量赋值
if (LOG4CPP_INCLUDE_DIR AND LOG4CPP_LIBRARY)
  set(LOG4CPP_FOUND TRUE)
  set(LOG4CPP_LIBRARIES ${LOG4CPP_LIBRARY})
  set(LOG4CPP_INCLUDE_DIRS ${LOG4CPP_INCLUDE_DIR})
else ()
  set(LOG4CPP_FOUND FALSE)
  message(WARNING "LOG4CPP not found")
endif ()
 
# 打印一些错误信息
if (LOG4CPP_FOUND)
  if (NOT LOG4CPP_FIND_QUIETLY)
    message(STATUS "Found LOG4CPP: ${LOG4CPP_LIBRARIES}")
  endif ()
else ()
  if (LOG4CPP_FIND_REQUIRED)
    message(STATUS "Looked for LOG4CPP libraries named ${LOG4CPPS_NAMES}.")
    message(FATAL_ERROR "Could NOT find LOG4CPP library")
  endif ()
endif ()
 
# 这个选项不是很懂，貌似是给cmake gui用的
mark_as_advanced(
  LOG4CPP_LIBRARIES
  LOG4CPP_INCLUDE_DIRS
)
