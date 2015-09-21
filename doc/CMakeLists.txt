cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

find_package(Doxygen)
if(NOT DOXYGEN_FOUND)
    message(STATUS "Doxygen Not found, documentation will not be generated")
    return()
endif(NOT DOXYGEN_FOUND)


if(NOT DOXYGEN_DOT_FOUND)
    message(STATUS "Dot Not found, documentation will not contain graphs")
endif(NOT DOXYGEN_DOT_FOUND)

# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/doc/img DESTINATION ${CMAKE_DOC_OUTPUT_DIR})

configure_file(${CMAKE_SOURCE_DIR}/cmake/Doxyfile.in ${CMAKE_DOC_OUTPUT_DIR}/Doxyfile @ONLY)

add_custom_target(doc
		${DOXYGEN_EXECUTABLE} ${CMAKE_DOC_OUTPUT_DIR}/Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating API documentation with Doxygen" VERBATIM
)
