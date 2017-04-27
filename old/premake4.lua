solution "PhysLang"
  configurations { "Debug", "Release" }
  
  configuration { "Debug" }
  targetdir "bin/debug"
 
  configuration { "Release" }
  targetdir "bin/release"
project "phc"
  language "C++"
  kind     "ConsoleApp"
  files  { "src/**.c", "src/**.cpp"}
  includedirs {"./include/"}
  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }
    flags   { "Symbols" }

  configuration { "Release*" }
    defines { "NDEBUG" }
    flags   { "Optimize" }
