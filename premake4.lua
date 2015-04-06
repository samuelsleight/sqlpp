solution "WorldGenLib"
	configurations { "debug", "release" }

project "sqlpp"
	kind "SharedLib"
	language "C++"
	files { "src/**" }
	links { "sqlite3" }
	objdir "build/obj"
	buildoptions { "-std=c++11" }

	configuration "debug"
		flags { "Symbols", "ExtraWarnings" }

	configuration "release"
		flags { "Optimize" }
