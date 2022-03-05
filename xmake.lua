if is_os("windows") then
    set_config("arch", "x86")
    if(is_mode("release")) then
        set_config("vs_runtime", "MD")
        add_cxflags("-MD", {force = true})
        set_installdir("build/windows/x86/release")
    else
        set_config("vs_runtime", "MDd")
        add_cxflags("-MDd", {force = true})
        set_installdir("build/windows/x86/debug")
    end
else
    add_cxflags("-MD", {force = true})
end

add_requires("vcpkg::openssl", {configs = {shared = true, vs_runtime = "MD"}, alias = "openssl"})

target("3DesTest")
    set_kind("binary")
    set_symbols("debug")
    add_packages("openssl")
    
    add_includedirs("3des", "Utils")
    add_files("3des/*.cpp")
    after_build(function(target)
        import("target.action.install")(target)
    end)
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