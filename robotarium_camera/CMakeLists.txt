project(robotarium_camera C CXX)
cmake_minimum_required(VERSION 2.8.3)
set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)
find_package(Boost 1.54.0
             COMPONENTS thread
                        system
                        log
                        log_setup
                        program_options
             REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/Vimba_2_1)
include_directories(${PROJECT_SOURCE_DIR}/Vimba_2_1/VimbaImageTransform/Include)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${Boost_INCLUDE_DIRS})

set(SOURCES include/CustomObserver.h
            include/CustomObserver.cpp
            include/RobotariumCamera.h
            include/RobotariumCamera.cpp)
set(HEADERS include/ProgramConfig.h
            include/BaseException.h
            include/TransformImage.h
            Vimba_2_1/VimbaImageTransform/Include/VmbCommonTypes.h
            Vimba_2_1/VimbaImageTransform/Include/VmbTransform.h
            Vimba_2_1/VimbaImageTransform/Include/VmbTransformTypes.h
            Vimba_2_1/VimbaCPP/Include/AncillaryData.h
            Vimba_2_1/VimbaCPP/Include/BasicLockable.h
            Vimba_2_1/VimbaCPP/Include/Camera.h
            Vimba_2_1/VimbaCPP/Include/Camera.hpp
            Vimba_2_1/VimbaCPP/Include/EnumEntry.h
            Vimba_2_1/VimbaCPP/Include/EnumEntry.hpp
            Vimba_2_1/VimbaCPP/Include/FeatureContainer.h
            Vimba_2_1/VimbaCPP/Include/FeatureContainer.hpp
            Vimba_2_1/VimbaCPP/Include/Feature.h
            Vimba_2_1/VimbaCPP/Include/Feature.hpp
            Vimba_2_1/VimbaCPP/Include/FileLogger.h
            Vimba_2_1/VimbaCPP/Include/Frame.h
            Vimba_2_1/VimbaCPP/Include/ICameraFactory.h
            Vimba_2_1/VimbaCPP/Include/ICameraListObserver.h
            Vimba_2_1/VimbaCPP/Include/IFeatureObserver.h
            Vimba_2_1/VimbaCPP/Include/IFrameObserver.h
            Vimba_2_1/VimbaCPP/Include/IInterfaceListObserver.h
            Vimba_2_1/VimbaCPP/Include/Interface.h
            Vimba_2_1/VimbaCPP/Include/Interface.hpp
            Vimba_2_1/VimbaCPP/Include/IRegisterDevice.h
            Vimba_2_1/VimbaCPP/Include/LoggerDefines.h
            Vimba_2_1/VimbaCPP/Include/Mutex.h
            Vimba_2_1/VimbaCPP/Include/SharedPointerDefines.h
            Vimba_2_1/VimbaCPP/Include/SharedPointer.h
            Vimba_2_1/VimbaCPP/Include/SharedPointer_impl.h
            Vimba_2_1/VimbaCPP/Include/UserLoggerDefines.h
            Vimba_2_1/VimbaCPP/Include/UserSharedPointerDefines.h
            Vimba_2_1/VimbaCPP/Include/VimbaCPPCommon.h
            Vimba_2_1/VimbaCPP/Include/VimbaCPP.h
            Vimba_2_1/VimbaCPP/Include/VimbaSystem.h
            Vimba_2_1/VimbaCPP/Include/VimbaSystem.hpp)
set(VIMBA_LIBS ${PROJECT_SOURCE_DIR}/Vimba_2_1/VimbaImageTransform/DynamicLib/x86_64bit/libVimbaImageTransform.so
               ${PROJECT_SOURCE_DIR}/Vimba_2_1/VimbaCPP/DynamicLib/x86_64bit/libVimbaC.so
		   ${PROJECT_SOURCE_DIR}/Vimba_2_1/VimbaCPP/DynamicLib/x86_64bit/libVimbaCPP.so)

add_executable(record_video src/record_video.cpp
                            ${SOURCES}
                            ${HEADERS})

add_executable(test_record_video src/test_record_video.cpp)

target_link_libraries(record_video ${VIMBA_LIBS} ${OpenCV_LIBS})
target_link_libraries(test_record_video ${OpenCV_LIBS})
