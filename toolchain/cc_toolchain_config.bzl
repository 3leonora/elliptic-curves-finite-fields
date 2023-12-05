load("@bazel_tools//tools/build_defs/cc:action_names.bzl", "ACTION_NAMES")
load("@bazel_tools//tools/cpp:cc_toolchain_config_lib.bzl",
    "feature",
    "flag_group",
    "flag_set",
    "tool_path",
)

all_link_actions = [
    ACTION_NAMES.cpp_link_executable,
    ACTION_NAMES.cpp_link_dynamic_library,
    ACTION_NAMES.cpp_link_nodeps_dynamic_library,
]

def _impl(ctx):
    
    tool_paths = [ tool_path(name = name, path = path) for name, path in (
        ("gcc", "/usr/bin/clang"),
        ("ld", "/usr/bin/ld"),
        ("ar", "/usr/bin/ar"),
        ("cpp", "/usr/bin/cpp"),
        ("gcov", "/usr/bin/gcov"),
        ("nm", "/usr/bin/nm"),
        ("objdump", "/usr/bin/objdump"),
        ("strip", "/usr/bin/strip"),
        )
    ]

    features = [
        feature(
            name = "default_linker_flags",
            enabled = True,
            flag_sets = [
                flag_set(
                    actions = all_link_actions,
                    flag_groups = ([
                        flag_group(
                            flags = [
                                "-lstdc++",
                                "-lm",
                            ],
                        ),
                    ]),
                ),
            ],
        ),
    ]

    return cc_common.create_cc_toolchain_config_info(
        cxx_builtin_include_directories = [
            "/usr/lib64/clang/16/include",
            "/usr/include",
        ],
        ctx = ctx,
        features = features,
        toolchain_identifier = "local",
        host_system_name = "local",
        target_system_name = "local",
        target_cpu = "k8",
        target_libc = "unknown",
        compiler = "clang",
        abi_version = "unknown",
        abi_libc_version = "unknown",
        tool_paths = tool_paths, # NEW
    )


cc_toolchain_config = rule(
    implementation = _impl,
    attrs = {},
    provides = [CcToolchainConfigInfo],
)