solution "WorldGenLib"
	configurations { "debug", "release" }

project "sqlpp"
	kind "SharedLib"
	language "C++"
	files { "new/**" }
	links { "sqlite3" }
	objdir "build/obj"
	buildoptions { "-std=c++1y" }

	configuration "debug"
		flags { "Symbols", "ExtraWarnings" }

	configuration "release"
		flags { "Optimize" }
