cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(TroyBinary)
add_executable(TroyBinary
    types.hpp
    main.cpp
    inibin.h
    inibin.cpp
    file.hpp
    ritoskn.hpp
    ritoskl.hpp
    ritoanm.hpp
    ritonvr.hpp
    ritomath.hpp
    ritoresource.hpp
    particle/complex.h
    particle/complex.cpp
    particle/fields.h
    particle/fields.cpp
    particle/system.h
    particle/system.cpp
    particle/ptypes.h
    particle/ptypes.cpp
    particle/simple.h
    particle/simple.cpp
    particle/instance/complex.h
    particle/instance/complex.cpp
    particle/instance/field.h
    particle/instance/field.cpp
    particle/instance/simple.h
    particle/instance/simple.cpp
    particle/instance/system.h
    particle/instance/system.cpp
)

