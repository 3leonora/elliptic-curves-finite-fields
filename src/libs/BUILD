cc_library(
    name = "primefield",
    visibility = ["//visibility:public"],
    srcs = [
        "primefield/uint128.cpp",
    ],
    hdrs = [
        "primefield/traits.hpp",
        "primefield/exception.hpp",
        "primefield/inverse.hpp",
        "primefield/uint128.hpp",
        "primefield/montgomery.hpp",
    ],
    # The includes below makes it so that clients of this library
    # can skip the src/libs prefix when including header files
    # from this library (hdrs argument)
    includes = [""],
    linkopts = select({
        "@bazel_tools//src/conditions:windows_x64": [
            # For __int128 division support in clang/Windows we need __udivti3
            # Note: add library search path according to bazelrc.win
            "/defaultlib:clang_rt.builtins-x86_64.lib",
        ]
    })
)


