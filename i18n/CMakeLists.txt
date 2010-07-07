
set (TRANSLATION_SRCS
	scc_pl.ts
)

# translation
qt4_add_translation (TRANSLATION_FILES ${TRANSLATION_SRCS})
add_custom_target (i18n DEPENDS ${TRANSLATION_FILES})
