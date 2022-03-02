if is_os("windows") then
    if(is_mode("release")) then
        add_cxflags("-MD", {force = true})
    else
        add_cxflags("-MDd", {force = true})
    end
else
    add_cxflags("-MD", {force = true})
end
set_config("shared", "true")

option("openssl.static")
    add_includedirs("$(env iudge_devpkg_root)\\third\\openssl.pkg\\v1.0.2p-1-static\\inc")
    add_linkdirs("$(env iudge_devpkg_root)\\third\\openssl.pkg\\v1.0.2p-1-static\\lib\\windows\\x86\\release")
    add_links("libeay32", "ssleay32")
    add_syslinks("User32", "Advapi32", "GDI32")

target("3DesTest")
    set_kind("binary")
    set_symbols("debug")
    add_options("openssl.static", {force = true})
    
    add_includedirs("3des", ".")
    add_files("3des/*.cpp")
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