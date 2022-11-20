vcpkg_from_github(
  OUT_SOURCE_PATH SOURCE_PATH
  REPO sammot/cyanshaderlib
  REF c06c0b70e148ee5dd6b9147441f3fd80b62430d1
  SHA512 38d6873b385de6d50359c4283731b732fac2fda455cb7d26b8927afb963471324df07f21da0b1d9e4b964852f41348fe0c1ac06f157466f9a887979e39626b78
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