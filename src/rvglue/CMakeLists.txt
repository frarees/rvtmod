list (APPEND SRCFILES
	appopts.cpp
	colormod.cpp
	errdesc.cpp
	findfile.cpp
	finf.cpp
	fixbridge.cpp
	fixpipe.cpp
	input.cpp
	legomod.cpp
	modlist.cpp
	optkeep.cpp
	parsedcolor.cpp
	parsedmod.cpp
	parser.cpp
	readprm.cpp
	simplemods.cpp
	tazmod.cpp
	texdef.cpp
	texfloor.cpp
	texmap.cpp
	texwall.cpp
	wf.cpp
	wmodlist.cpp
	
	main.cpp
)

list (APPEND HDRFILES
	appopts.h
	colormod.h
	errdesc.h
	findfile.h
	finf.h
	fixbridge.h
	fixpipe.h
	input.h
	keys.h
	legomod.h
	meshmod.h
	modlist.h
	optkeep.h
	parsedcolor.h
	parsedmod.h
	parser.h
	readprm.h
	simplemods.h
	tazmod.h
	texdef.h
	texfloor.h
	texmap.h
	texwall.h
	wf.h
	wmodlist.h
)

include_directories (${RVGL_PATH})

if (WIN32)
	SET(GLUE_DIRS _GLUE_USE_WIN_DIRS)
else ()
	SET(GLUE_DIRS _GLUE_USE_POSIX_DIRS)
endif ()
add_definitions (-D${GLUE_DIRS})

add_executable (rvglue ${SRCFILES} ${HDRFILES})
add_dependencies (rvglue rvgl)
target_link_libraries (rvglue rvgl)
