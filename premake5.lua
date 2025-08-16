require("ecc/ecc")
-- ===============================
-- WORKSPACE(DropPlatformer)
-- ===============================
workspace "DropPlatformer"
architecture "x64"
configurations {"Debug", "Release", "Dist"}

defines {"UNICODE", "_UNICODE"}

filter {"configurations:Debug"}
defines {"DEBUG", "_DEBUG"}
symbols "On"

filter {"configurations:Release"}
defines {"NDEBUG"}
optimize "On"

filter {"configurations:Dist"}
defines {"NDEBUG", "DO_NLOG"}
optimize "On"

filter {"action:vs*", "configurations:Release"}
staticruntime "On"

filter {"action:vs*", "configurations:Dist"}
staticruntime "On"

filter {"action:gmake", "configurations:Release"}
buildoptions {"-static"}

filter {"action:gmake", "configurations:Dist"}
buildoptions {"-static"}

filter {}

outdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- ===============================
-- Project(EngineDLL)
-- ===============================
project "EngineDLL"
location "EngineDLL"
kind "SharedLib"
language "C"
cdialect "C11"

targetdir("bin/" .. outdir .. "/data")
objdir("bin-int/" .. outdir .. "/data")

pchheader "pch.h"
pchsource "%{prj.location}/src/pch.c"

files {"%{prj.location}/include/**.h", "%{prj.location}/src/**.c"}
includedirs {"%{prj.location}", "%{prj.location}/include"}

links {"user32", "d3d11", "dxgi", "dxguid", "d3dcompiler"}

defines {"DO_DLL_EXPORTS"}

filter {"action:gmake"}
postbuildcommands {'[ -f %{cfg.targetdir}/EngineDLL.dll ] && cp -f %{cfg.targetdir}/EngineDLL.dll %{wks.location}/bin/' ..
    outdir .. '/'}

filter {"action:vs*"}
postbuildcommands {'{COPY} %{cfg.targetdir}/EngineDLL.dll %{wks.location}/bin/' .. outdir .. '/'}

filter {}

-- ===============================
-- Project(DropPlatformer)
-- ===============================
project "DropPlatformer"
location "DropPlatformer"
kind "ConsoleApp"
language "C"
cdialect "C11"

targetdir("bin/" .. outdir)
objdir("bin-int/" .. outdir)

files {"%{prj.location}/*.c"}
includedirs {"EngineDLL"}

links {"EngineDLL"}

