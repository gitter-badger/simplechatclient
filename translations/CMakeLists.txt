
set (TRANSLATION_SRCS
	qt_pl.ts
	scc_pl.ts
)

set (TRANSLATION_QM
	qt_pl.qm
	scc_pl.qm
)

# translation
qt5_add_translation (TRANSLATION_FILES ${TRANSLATION_SRCS})
add_custom_target (translations DEPENDS ${TRANSLATION_FILES})

# translations
add_dependencies (scc translations)

# install
if (UNIX AND NOT APPLE)
	install (FILES ${TRANSLATION_QM} DESTINATION ${SCC_DATA_DIR}/translations)
endif (UNIX AND NOT APPLE)
