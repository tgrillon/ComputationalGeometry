find_package(Doxygen REQUIRED)

include(FetchContent)
FetchContent_Declare(
  doxygen-awesome-css
  GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css.git
  GIT_TAG v2.3.1
)
FetchContent_MakeAvailable(doxygen-awesome-css)

# --- Macros must be defined before use ---
macro(UseDoxygenAwesomeCss)
  set(DOXYGEN_GENERATE_TREEVIEW     YES)
  set(DOXYGEN_HAVE_DOT              YES)
  set(DOXYGEN_DOT_IMAGE_FORMAT      svg)
  set(DOXYGEN_DOT_TRANSPARENT       YES)
  set(DOXYGEN_HTML_EXTRA_STYLESHEET
      ${doxygen-awesome-css_SOURCE_DIR}/doxygen-awesome.css)
endmacro()

macro(UseDoxygenAwesomeExtensions)
  set(DOXYGEN_HTML_EXTRA_FILES
    ${DOXYGEN_HTML_EXTRA_FILES}
    ${doxygen-awesome-css_SOURCE_DIR}/doxygen-awesome-darkmode-toggle.js
    ${doxygen-awesome-css_SOURCE_DIR}/doxygen-awesome-fragment-copy-button.js
    ${doxygen-awesome-css_SOURCE_DIR}/doxygen-awesome-paragraph-link.js
    ${doxygen-awesome-css_SOURCE_DIR}/doxygen-awesome-interactive-toc.js
  )

  execute_process(COMMAND doxygen -w html header.html footer.html style.css
                  WORKING_DIRECTORY ${PROJECT_BINARY_DIR})

  # Cross-platform header injection
  file(READ "${PROJECT_BINARY_DIR}/header.html" HEADER_CONTENTS)
  file(READ "${PROJECT_SOURCE_DIR}/cmake/doxygen_extra_headers" EXTRA_HEADERS)
  string(REPLACE "</head>" "${EXTRA_HEADERS}</head>" HEADER_CONTENTS "${HEADER_CONTENTS}")
  file(WRITE "${PROJECT_BINARY_DIR}/header.html" "${HEADER_CONTENTS}")

  set(DOXYGEN_HTML_HEADER ${PROJECT_BINARY_DIR}/header.html)
endmacro()

# --- Main Doxygen function ---
function(Doxygen target input_dirs source_root)
  set(NAME "doxygen-${target}")
  
  # Build the input paths
  set(INPUT_PATHS "")
  foreach(dir ${input_dirs})
    list(APPEND INPUT_PATHS "${source_root}/${dir}")
  endforeach()
  
  # Add README.md as input
  list(APPEND INPUT_PATHS "${source_root}/README.md")
  
  set(DOXYGEN_GENERATE_HTML YES)
  set(DOXYGEN_HTML_OUTPUT ${PROJECT_BINARY_DIR}/docs)
  set(DOXYGEN_PROJECT_NUMBER ${PROJECT_VERSION})
  set(DOXYGEN_SOURCE_BROWSER YES)
  set(DOXYGEN_INLINE_SOURCES YES)
  
  # Set README.md as mainpage
  set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${source_root}/README.md")
  
  # Extract all symbols
  set(DOXYGEN_EXTRACT_ALL YES)
  set(DOXYGEN_EXTRACT_PRIVATE YES)
  set(DOXYGEN_EXTRACT_STATIC YES)
  
  # Enable grouping for modules
  set(DOXYGEN_ENABLE_PREPROCESSING YES)
  
  UseDoxygenAwesomeCss()
  UseDoxygenAwesomeExtensions()
  
  message(STATUS "Documentation inputs: ${INPUT_PATHS}")
  
  doxygen_add_docs(
    ${NAME}
    ${INPUT_PATHS}
    COMMENT "Generate HTML documentation for ${target}"
  )
endfunction()
