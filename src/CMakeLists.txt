add_library(gstcmlnsrc SHARED
  plugin.cpp
  cmlnsrc.cpp
)

target_link_libraries(gstcmlnsrc ${PKG_LIBRARIES})