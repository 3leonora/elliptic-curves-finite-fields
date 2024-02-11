load(
    ":cc_toolchain_config.bzl",
    "cc_toolchain_config",
    "clang_prefix",
    "system_includes",
)

package(default_visibility = ["//visibility:public"])

filegroup(name = "empty")

cc_toolchain_suite(
    name = "clang_suite",
    toolchains = {
        "k8": ":k8_toolchain",
        "arm64": ":arm64_toolchain",
    },
)

cc_toolchain(
    name = "k8_toolchain",
    toolchain_identifier = "k8-toolchain",
    toolchain_config = ":k8_toolchain_config",
    all_files = ":empty",
    compiler_files = ":empty",
    dwp_files = ":empty",
    linker_files = ":empty",
    objcopy_files = ":empty",
    strip_files = ":empty",
    supports_param_files = 0,
)

cc_toolchain_config(name = "k8_toolchain_config")

cc_toolchain(
    name = "arm64_toolchain",
    toolchain_identifier = "arm64-toolchain",
    toolchain_config = ":arm64_toolchain_config",
    all_files = ":empty",
    compiler_files = ":empty",
    dwp_files = ":empty",
    linker_files = ":empty",
    objcopy_files = ":empty",
    strip_files = ":empty",
    supports_param_files = 0,
)

cc_toolchain_config(name = "arm64_toolchain_config")

clang_prefix(
    name = "clang_prefix",
    build_setting_default = "",
)

system_includes(
    name = "system_includes",
    build_setting_default = "",
)

platform(
    name = "x64_windows-clang-cl",
    constraint_values = [
        "@platforms//cpu:x86_64",
        "@platforms//os:windows",
        "@bazel_tools//tools/cpp:clang-cl",
    ],
)
