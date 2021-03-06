# additional target to perform clang-format run, requires clang-format
# get all project files
#file(GLOB_RECURSE ALL_SOURCE_FILES *.c *.cpp *.h)
#foreach (SOURCE_FILE ${ALL_SOURCE_FILES})
#	string(FIND ${SOURCE_FILE} ${PROJECT_SOURCE_DIR} PROJECT_TRDPARTY_DIR_FOUND)
#	if (NOT ${PROJECT_TRDPARTY_DIR_FOUND} EQUAL -1)
#		list(REMOVE_ITEM ALL_SOURCE_FILES ${SOURCE_FILE})
#	endif ()
#endforeach ()

file(GLOB_RECURSE ALL_SOURCE_FILES *.c *.cpp *.h)

set (EXCLUDE_DIR "/third_party/")
foreach (TMP_PATH ${ALL_SOURCE_FILES})
    string (FIND ${TMP_PATH} ${EXCLUDE_DIR} EXCLUDE_DIR_FOUND)
    if (NOT ${EXCLUDE_DIR_FOUND} EQUAL -1)
        list (REMOVE_ITEM ALL_SOURCE_FILES ${TMP_PATH})
    endif ()
endforeach(TMP_PATH)


add_custom_target(
	clangformat
	COMMAND clang-format
	-style=Google
	-i
	${ALL_SOURCE_FILES}
	)
