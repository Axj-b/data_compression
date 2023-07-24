cmake_minimum_required(VERSION 3.0.0)
project(TestCompressing VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()


file( GLOB sources_cxx
      *.cc *.cpp *.cxx
  )
file( GLOB headers_cxx
      *.hh *.hpp *.hxx
  )
file( GLOB sources_c
      *.c
  )
file( GLOB headers_c
      *.h

      )
     
  
add_executable(TestCompressing 


${sources_cxx}
${headers_cxx}
${headers_c}
${sources_c}
)
#add_subdirectory(miniz)
#target_link_libraries(TestCompressing miniz)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
