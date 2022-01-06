# function(COPY_LOCAL_INCLUDES header_files component)
set(header_files ${CMAKE_ARGV3})
set(component ${CMAKE_ARGV4})
set(QTZ_LOCAL_INCLUDES_DIR ${CMAKE_ARGV5})

file(MAKE_DIRECTORY "${QTZ_LOCAL_INCLUDES_DIR}/qtz/${component}")
file(COPY ${header_files} DESTINATION "${QTZ_LOCAL_INCLUDES_DIR}/qtz/${component}/" FOLLOW_SYMLINK_CHAIN)
