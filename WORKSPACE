load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")


http_archive(
  name = "com_google_googletest",
  urls = ["https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip"],
  strip_prefix = "googletest-1.14.0",
  sha256 = "1f357c27ca988c3f7c6b4bf68a9395005ac6761f034046e9dde0896e3aba00e4",
)

http_archive(
  name = "clockwork",
  urls = ["https://github.com/hurchalla/modular_arithmetic/archive/refs/tags/v1.1.0.zip"],
  strip_prefix = "modular_arithmetic-1.1.0",
  sha256 = "76cbd4e30f398a2b622d16b6c6d161e0cceb7e6c5f1e4c09f804dc2d62aaf803",
  build_file = "@//3pp:BUILD.clockwork"
)

http_archive(
  name = "hurchalla_util",
  urls = ["https://github.com/hurchalla/util/archive/8186d90df8eaf1e5b6e7980f52556334e72c05f8.zip"],
  strip_prefix = "util-8186d90df8eaf1e5b6e7980f52556334e72c05f8",
  sha256 = "a865fa78d524367b28397775e629bb27b591ba45342c0b3b3bce9d85a9610268",
  build_file = "@//3pp:BUILD.hurchalla_util",
  patches = [
    "@//3pp:patch.hurchalla_util",
  ],
  patch_args = ["-p1"],
)
