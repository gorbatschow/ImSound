cmake_minimum_required(VERSION 3.16.0)

project(imsound)

set(CXX_CMAKE_STANDARD 20)
set(CXX_CMAKE_STANDARD_REQUIRED ON)

set(IMSOUND_SRC
    src/ImSoundControlWidget.cpp
    src/ImSoundSetupDisplayWidget.cpp
    src/ImSoundClientInfoWidget.cpp
    src/ImSoundWaveformScopeWidget.cpp
    src/ImSoundWaveformPlotWidget.cpp
    src/ImSoundToneGeneratorWidget.cpp
)

add_library(${PROJECT_NAME} ${IMSOUND_SRC})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_include_directories(${PROJECT_NAME} PUBLIC .)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC rtsound)
target_link_libraries(${PROJECT_NAME} PUBLIC imapp)
target_link_libraries(${PROJECT_NAME} PUBLIC imwrap)
target_link_libraries(${PROJECT_NAME} PUBLIC downplot)
target_link_libraries(${PROJECT_NAME} PUBLIC mini)
