list (APPEND SRCFILES
	axisbox.cpp
	box.cpp
	collapsedmesh.cpp
	helpfunc.cpp
	lookup.cpp
	matrix3.cpp
	mesh.cpp
	ncpwriter.cpp
	polygon.cpp
	polymod.cpp
	print.cpp
	prmwriter.cpp
	rvcolor.cpp
	surface.cpp
	vector.cpp
	vertex.cpp
	world.cpp
	wwriter.cpp
)

list (APPEND HDRFILES
	axisbox.h
	box.h
	collapsedmesh.h
	helpfunc.h
	lookup.h
	matrix3.h
	mesh.h
	ncpwriter.h
	polygon.h
	polymod.h
	print.h
	prmwriter.h
	rvbasetypes.h
	rvcolor.h
	rvtypes.h
	surface.h
	vector.h
	vertex.h
	world.h
	wwriter.h
)

add_library (rvgl STATIC ${SRCFILES} ${HDRFILES})