vcpkg_from_github(
  OUT_SOURCE_PATH SOURCE_PATH
  REPO sammot/cyanshaderlib
  REF f4afb00eefc0da01fa95251f6e6f3b0cc053b717
  SHA512 d593dc6b66162c2a2a7364b97075f67f3e0de35af2814173a2c191d2ed340f2c3cb5386d1439f858b80ce5581e5f47b86549c209d0b65d6bffd31fde6864263c
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