
add_rules("mode.debug", "mode.release")
set_config("arch", "x86")
set_config("vs_runtime", "MD")
set_config("shared", "true")

add_requires("vcpkg::openssl", {configs = {shared = true, vs_runtime = "MD"}, alias = "openssl"})
--add_requires("vcpkg::boost", {configs = {shared = true, vs_runtime = "MD"}, alias = "boost"})

option("mode.RuntimeLibrary")
    if(is_mode("release")) then
        add_cxflags("-MD", {force = true})
    else
        add_cxflags("-MDd", {force = true})
    end

target("3DesTest")
    set_kind("binary")
    add_options("mode.RuntimeLibrary")
    set_symbols("debug")
    
    add_includedirs("3des")
    add_files("3des/*.cpp")
    add_packages("openssl", "vcpkg::boost 1.78.0")

    -- on_load(function (target)
    --     import("lib.detect.find_package")
    --     target:add(find_package("openssl"))
    --     --target:add(find_packages("vcpkg::openssl", "boost"))
    -- end)

-- 传递变量
-- option("foo")
-- set_default(true)
-- set_description("Enable Foo")
-- set_configvar("FOO_ENABLE", 1) -- 或者传递true，启用FOO_ENABLE变量
-- set_configvar("FOO_STRING", "foo")

-- target("test")
--     add_configfiles("config.h.in")

--     -- 如果启用foo选项 -> 天剑 FOO_ENABLE 和 FOO_STRING 定义
--     add_options("foo") 