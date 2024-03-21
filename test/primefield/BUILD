cc_test(
    name = "primefield_test",
    size = "small",
    srcs = glob(["test_*.cpp"]),
    deps = [
        "@com_google_googletest//:gtest_main",
        "@clockwork//:modular_arithmetic",
        "@clockwork//:montgomery_arithmetic",
	    "//src/libs:primefield",
    ],
)
