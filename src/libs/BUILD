cc_library(
    name = "primefield",
    srcs = [
        "primefield/uint128.cpp",
    ],
    hdrs = glob(["primefield/*.hpp"]),
    # The includes below makes it so that clients of this library
    # can skip the src/libs prefix when including header files
    # from this library (hdrs argument)
    includes = [""],
    linkopts = select({
        "@bazel_tools//src/conditions:windows_x64": [
            # For __int128 division support in clang/Windows we need __udivti3
            # Note: add library search path according to bazelrc.win
            "/defaultlib:clang_rt.builtins-x86_64.lib",
        ],
    }),
    visibility = ["//visibility:public"],
)

cc_library(
    name = "profiling",
    srcs = glob(["profiling/*.cpp"]),
    hdrs = glob(["profiling/*.hpp"]),
    includes = [""],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "schedule",
    srcs = glob(["schedule/*.cpp"]),
    hdrs = glob(["schedule/*.hpp"]),
    includes = [""],
    visibility = ["//visibility:public"],
)
