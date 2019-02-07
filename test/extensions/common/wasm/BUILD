licenses(["notice"])  # Apache 2

load(
    "//bazel:envoy_build_system.bzl",
    "envoy_cc_test",
    "envoy_package",
)

envoy_package()

envoy_cc_test(
    name = "wasm_test",
    srcs = ["wasm_test.cc"],
    data = [
        "//test/extensions/common/wasm/test_data:modules",
    ],
    external_deps = ["abseil_optional"],
    deps = [
        "//source/common/event:dispatcher_lib",
        "//source/common/stats:isolated_store_lib",
        "//source/common/stats:stats_lib",
        "//source/extensions/common/wasm:config",
        "//source/extensions/common/wasm:wasm_lib",
        "//test/test_common:environment_lib",
        "//test/test_common:simulated_time_system_lib",
    ],
)

envoy_cc_test(
    name = "config_test",
    srcs = ["config_test.cc"],
    data = [
        "//test/extensions/common/wasm/test_data:modules",
    ],
    deps = [
        "//include/envoy/registry",
        "//source/common/stats:isolated_store_lib",
        "//source/extensions/common/wasm:config",
        "//source/extensions/common/wasm:wasm_lib",
        "//source/server:wasm_config_lib",
        "//test/mocks/event:event_mocks",
        "//test/mocks/thread_local:thread_local_mocks",
        "//test/test_common:environment_lib",
        "@envoy_api//envoy/config/common/wasm/v2:wasm_cc",
    ],
)
