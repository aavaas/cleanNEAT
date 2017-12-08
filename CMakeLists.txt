cmake_minimum_required(VERSION 3.8)
project(cleanNEAT)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        include/experiments.h
        include/gene.h
        include/genome.h
        include/innovation.h
        include/link.h
        include/neat.h
        include/neatmain.h
        include/network.h
        include/nnode.h
        include/organism.h
        include/population.h
        include/species.h
        include/trait.h
        include/neatIO.h
        include/avsExperiment.h
        src/experiments.cpp
        src/gene.cpp
        src/genome.cpp
        src/innovation.cpp
        src/link.cpp
        src/neat.cpp
        src/neatmain.cpp
        src/network.cpp
        src/nnode.cpp
        src/organism.cpp
        src/population.cpp
        src/species.cpp
        src/trait.cpp
        src/neatIO.cpp src/avsExperiment.cpp )

include_directories(include)

add_executable(cleanNEAT ${SOURCE_FILES})