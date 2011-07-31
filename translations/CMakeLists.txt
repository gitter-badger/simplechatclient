
set (TRANSLATION_SRCS
	qt_pl.ts
	scc_pl.ts
)

# translation
qt4_add_translation (TRANSLATION_FILES ${TRANSLATION_SRCS})
add_custom_target (translations DEPENDS ${TRANSLATION_FILES})
