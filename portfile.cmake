vcpkg_from_github(
  OUT_SOURCE_PATH SOURCE_PATH
  REPO sammot/cyanshaderlib
  REF 5513ac796c289992e93fceb8f8e8d18c0a5fa6a0
  SHA512 959e99dedf60252e95422d5b24de8102f57fccb0765a4550fbefb13dc33b07a9c8357b74a5389907474e2bc60ed2af5be37533f7c45628545ea0604da28d809e
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