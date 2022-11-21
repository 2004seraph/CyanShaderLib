vcpkg_from_github(
  OUT_SOURCE_PATH SOURCE_PATH
  REPO sammot/cyanshaderlib
  REF 2bff866c500ab541a2a52bb24bd44ae15aac5d39
  SHA512 f6e79722a20530ec5787460401f99641cde72258d0ec4b9e351f71d51ce43bd2d468a74534afb7b31bc183b5fb88f01f61b7d204e563679e873b219ca9b4b10c
  HEAD_REF main
)

vcpkg_configure_cmake(
  SOURCE_PATH "${SOURCE_PATH}"
  PREFER_NINJA
)
vcpkg_install_cmake()
vcpkg_fixup_cmake_targets()

#file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

file(
  INSTALL "${SOURCE_PATH}/LICENSE"
  DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}"
  RENAME copyright
)