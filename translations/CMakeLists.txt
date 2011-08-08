
set (TRANSLATIONS_SRCS
	qt_pl.ts
	scc_pl.ts
)

# translation
qt4_add_translation (TRANSLATIONS_FILES ${TRANSLATIONS_SRCS})
add_custom_target (translations DEPENDS ${TRANSLATIONS_FILES})
