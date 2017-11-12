MESSAGE(“In include CMAKELIST”)

#
# Descend into subdirs to build libraries...
add_subdirectory(database)
add_subdirectory(match)
add_subdirectory(record)
add_subdirectory(spectogram)
