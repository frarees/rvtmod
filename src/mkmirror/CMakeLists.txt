list (APPEND SRCFILES
	mkmirror.cpp
)

include_directories (${RVGL_PATH})

add_executable (mkmirror ${SRCFILES})
add_dependencies (mkmirror rvgl)
target_link_libraries (mkmirror rvgl)
