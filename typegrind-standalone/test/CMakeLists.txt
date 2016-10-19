
set(CLANG_TYPEGRIND_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
set(CLANG_TYPEGRIND_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/..")

configure_lit_site_cfg(
  ${CMAKE_CURRENT_SOURCE_DIR}/lit.site.cfg.in
  ${CMAKE_CURRENT_BINARY_DIR}/lit.site.cfg
  )

set(CLANG_TYPEGRIND_TEST_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(CLANG_TYPEGRIND_TEST_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}")
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/typegrind.json.in
  ${CMAKE_CURRENT_BINARY_DIR}/typegrind.json
  )

set(CLANG_TYPEGRIND_TEST_DEPS
  # Base line deps.
  FileCheck count not

  # Individual tools we test.
  clang-typegrind
  )

add_lit_testsuite(check-clang-typegrind "Running the Clang Typegrind's regression tests"
  ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${CLANG_TYPEGRIND_TEST_DEPS}
  )
set_target_properties(check-clang-typegrind PROPERTIES FOLDER "Clang Typegrind's tests")



