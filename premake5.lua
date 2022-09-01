workspace "CoronaEngine"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["imgui"] = "CoronaEngine/vendor/imgui"
IncludeDir["assimp"] = "CoronaEngine/vendor/assimp/include"

include "CoronaEngine/vendor/imgui"
include "CoronaEngine/vendor/assimp"

project "CoronaEngine"
	location "CoronaEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++14"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "crpch.h"
	pchsource "CoronaEngine/Core/crpch.cpp"

	files
	{
		"%{prj.name}/Core/**.h",
		"%{prj.name}/Core/**.cpp",
		"%{prj.name}/Core/**.inl"
	}

	vpaths
	{
		["Core/Graphics/Resource"] = 
		{
			"CoronaEngine/Core/BufferManager.cpp",
			"CoronaEngine/Core/BufferManager.h",
			"CoronaEngine/Core/ColorBuffer.cpp",
			"CoronaEngine/Core/Colorbuffer.h",
			"CoronaEngine/Core/DepthBuffer.cpp",
			"CoronaEngine/Core/DepthBuffer.h",
			"CoronaEngine/Core/EsramAllocator.h",
			"CoronaEngine/Core/GpuBuffer.cpp",
			"CoronaEngine/Core/GpuBuffer.h",
			"CoronaEngine/Core/GpuResource.h",
			"CoronaEngine/Core/LinearAllocator.cpp",
			"CoronaEngine/Core/LinearAllocator.h",
			"CoronaEngine/Core/PixelBuffer.cpp",
			"CoronaEngine/Core/PixelBuffer.h",
			"CoronaEngine/Core/UploadBuffer.cpp",
			"CoronaEngine/Core/UploadBuffer.h",
		},
		["Core/Graphics/Command"] = 
		{
			"CoronaEngine/Core/CommandAllocatorPool.cpp",
			"CoronaEngine/Core/CommandAllocatorPool.h",
			"CoronaEngine/Core/CommandContext.cpp",
			"CoronaEngine/Core/CommandContext.h",
			"CoronaEngine/Core/CommandListManager.cpp",
			"CoronaEngine/Core/CommandListManager.h",
		},
		["Core/Graphics/DescriptorHeap"] = 
		{
			"CoronaEngine/Core/DescriptorHeap.cpp",
			"CoronaEngine/Core/DescriptorHeap.h",
			"CoronaEngine/Core/DynamicDecriptorHeap.cpp",
			"CoronaEngine/Core/DynamicDescriptorHeap.h",
		},
		["Core/Graphics/Pipeline"] = 
		{
			"CoronaEngine/Core/PipelineState.cpp",
			"CoronaEngine/Core/PipelineState.h",
			"CoronaEngine/Core/RootSignature.cpp",
			"CoronaEngine/Core/RootSignature.h",
		},
		["Core/Graphics"] = 
		{
			"CoronaEngine/Core/Color.cpp",
			"CoronaEngine/Core/Color.h",
			"CoronaEngine/Core/d3dx12.h",
			"CoronaEngine/Core/GameCore.cpp",
			"CoronaEngine/Core/GameCore.h",
			"CoronaEngine/Core/SystemTime.cpp",
			"CoronaEngine/Core/SystemTime.h",
			"CoronaEngine/Core/Utility.cpp",
			"CoronaEngine/Core/Utility.h",
			"CoronaEngine/Core/VectorMath.h",
			"CoronaEngine/Core/Display.cpp",
			"CoronaEngine/Core/Display.h",
			"CoronaEngine/Core/crpch.cpp",
			"CoronaEngine/Core/crpch.h",
			"CoronaEngine/Core/Hash.h",
			"CoronaEngine/Core/GraphicsCore.cpp",
			"CoronaEngine/Core/GraphicsCore.h",
		}
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/Core",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.assimp}"
	}

	links
	{
		"imgui",
		"assimp"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"CR_PLATFORM_WINDOWS"
		}
		
	filter "configurations:Debug"
		defines "CR_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "CR_RELEASE"
		runtime "Release"
		optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++14"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/Core/**.h",
		"%{prj.name}/Core/**.cpp"
	}

	includedirs
	{
		"CoronaEngine/vendor/spdlog/include",
		"CoronaEngine/Core"
	}

	links
	{
		"CoronaEngine",
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"CR_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "CR_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "CR_RELEASE"
		runtime "Release"
		optimize "On"
