list (APPEND SRCFILES
	appopts.cpp
	asefacevlist.cpp
	asekeys.cpp
	asematerial.cpp
	asemesh.cpp
	aseparser.cpp
	aseprint.cpp
	asevlist.cpp
	namelist.cpp
)

list (APPEND HDRFILES
	appopts.h
	asefacevlist.h
	asekeys.h
	asematerial.h
	asemesh.h
	aseparser.h
	aseprint.h
	asevlist.h
	namelist.h
)

include_directories (${RVGL_PATH})

add_executable (ase2w ase2w.cpp ${SRCFILES} ${HDRFILES})
add_dependencies (ase2w rvgl)
target_link_libraries (ase2w rvgl)

add_executable (ase2prm ase2prm.cpp ${SRCFILES} ${HDRFILES})
add_dependencies (ase2prm rvgl)
target_link_libraries (ase2prm rvgl)

add_executable (ase2taz ase2taz.cpp ${SRCFILES} ${HDRFILES})
add_dependencies (ase2taz rvgl)
target_link_libraries (ase2taz rvgl)

add_executable (ase2vis ase2vis.cpp ${SRCFILES} ${HDRFILES})
add_dependencies (ase2vis rvgl)
target_link_libraries (ase2vis rvgl)
