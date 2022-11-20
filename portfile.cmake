vcpkg_from_github(
  OUT_SOURCE_PATH SOURCE_PATH
  REPO sammot/cyanshaderlib
  REF c06c0b70e148ee5dd6b9147441f3fd80b62430d1
  SHA512 5c648915b5d6f19c4bbf2b5569d763bbcc3f0f6927014604905458c8accc7a7a43fbab42e101b5661a4e100751ef9fd967881b5de91738427e3cefbfdbd3590a
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