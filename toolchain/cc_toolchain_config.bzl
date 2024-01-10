load("@bazel_tools//tools/build_defs/cc:action_names.bzl", "ACTION_NAMES")
load("@bazel_tools//tools/cpp:cc_toolchain_config_lib.bzl",
    "feature",
    "flag_group",
    "flag_set",
    "tool_path",
)


ToolchainPrefixInfo = provider(doc = "", fields = ["prefix"])

SystemIncludesInfo = provider(doc = "", fields = ["dirlist"])

k8_clang_prefix = None

arm_clang_prefix = None


all_link_actions = [
    ACTION_NAMES.cpp_link_executable,
    ACTION_NAMES.cpp_link_dynamic_library,
    ACTION_NAMES.cpp_link_nodeps_dynamic_library,
]

def _impl(ctx):

    # Note to self: ctx.var contain TARGET_CPU which might be handy later.    
    prefix = ctx.attr._clang_prefix[ToolchainPrefixInfo].prefix
    system_includes = ctx.attr._system_includes[SystemIncludesInfo].dirlist

    gcc_path = "/usr/bin/gcc"
    if prefix:
        gcc_path = "%s/bin/clang" % prefix

    tool_paths = [ tool_path(name = name, path = path) for name, path in (
        ("gcc", gcc_path),
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
        cxx_builtin_include_directories = system_includes + [
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
        tool_paths = tool_paths,
    )


cc_toolchain_config = rule(
    implementation = _impl,
    attrs = {
        "_clang_prefix": attr.label(default = ":clang_prefix"),
        "_system_includes": attr.label(default = ":system_includes")
    },
    provides = [CcToolchainConfigInfo],
)


def _clang_prefix_impl(ctx):
    clang_prefix = ctx.build_setting_value
    if len(clang_prefix) > 0 and clang_prefix[0] != '/':
        fail("Clang prefix not an absolute path: " + clang_prefix)
    return ToolchainPrefixInfo(prefix = clang_prefix)


clang_prefix = rule(
    implementation = _clang_prefix_impl,
    build_setting = config.string(flag = True),
)


def _system_includes_impl(ctx):
    dirs = ctx.build_setting_value.split(":")
    return SystemIncludesInfo(dirlist = dirs)


system_includes = rule(
    implementation = _system_includes_impl,
    build_setting = config.string(flag = True),
)
    
    