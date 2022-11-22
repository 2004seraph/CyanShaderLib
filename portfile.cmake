vcpkg_from_github(
  OUT_SOURCE_PATH SOURCE_PATH
  REPO sammot/cyanshaderlib
  REF 4af6fcdcda1dfc3681e8c1ff3a7dc4984e0ba98e
  SHA512 fa4bc52889cff100fbd536bd929d474e2b4abda75049bfcbeaf0f13249ec0d365c899c391aa18c12e6e038b1a240012f8392254b099e41accde5461d124183a7
  HEAD_REF main
)

vcpkg_configure_cmake(
  SOURCE_PATH "${SOURCE_PATH}"
  PREFER_NINJA
)
vcpkg_install_cmake()
vcpkg_fixup_cmake_targets()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

file(
  INSTALL "${SOURCE_PATH}/LICENSE"
  DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}"
  RENAME copyright
)