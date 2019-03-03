# Locate OpenCV
# Rafael Radkowski


set(VERSION "345")

set(_OPENCV_DEFAULT_INSTALL_DIR 
	"C:/SDK/opencv/build" 
	"C:/SDK/opencv" 
	"D:/SDK/opencv"
	"D:/SDK/opencv/build"
	"C:/SDK/opencv-3.4.5"
)
    

find_path(OPENCV_ROOT 
	NAMES /sources/include/opencv2/opencv.hpp
	PATHS ${_OPENCV_DEFAULT_INSTALL_DIR}
)

find_path(OPENCV_ROOT 
	NAMES /builds/include/opencv2/opencv.hpp
	PATHS ${_OPENCV_DEFAULT_INSTALL_DIR}
)


find_path(OPENCV_DIR
	NAMES /sources/include/opencv2/opencv.hpp
	PATHS ${_OPENCV_DEFAULT_INSTALL_DIR}
)

find_path(OPENCV_DIR
	NAMES /builds/include/opencv2/opencv.hpp
	PATHS ${_OPENCV_DEFAULT_INSTALL_DIR}
)




find_path(_WITH_BUILD_
	NAMES /include/opencv2/opencv.hpp
	PATHS ${OPENCV_ROOT}/builds
)

find_path(_WITH_SOURCE_
	NAMES /include/opencv2/opencv.hpp
	PATHS ${OPENCV_ROOT}/sources
)
#mark_as_advanced(${_WITH_SOURCE_} ${_WITH_BUILD_})


if(_WITH_BUILD_)

find_path(OPENCV_INCLUDE_DIR 
	NAMES /opencv2/opencv.hpp 
	PATHS ${_WITH_BUILD_}/include)


string(CONCAT _core_lib_  "opencv_core" ${VERSION} ".lib" )
string(CONCAT _core_libd_  "opencv_core" ${VERSION} "d.lib" )

find_path(OPENCV_LIBRARY_PATH
	NAMES  ${_core_lib_}
	PATHS ${_WITH_BUILD_}/lib/Release)

find_path(OPENCV_LIBRARY_DIR
	NAMES  ${_core_lib_}
	PATHS ${_WITH_BUILD_}/lib/Release)

find_path(OPENCV_LIBRARY_PATH_D
	NAMES  ${_core_libd_}
	PATHS ${_WITH_BUILD_}/lib/Debug)

find_path(OPENCV_LIBRARY_DIR_D
	NAMES  ${_core_libd_}
	PATHS ${_WITH_BUILD_}/lib/Debug)

elseif(_WITH_SOURCE_)

find_path(OPENCV_INCLUDE_DIR 
	NAMES /opencv2/opencv.hpp 
	PATHS ${_WITH_SOURCE_}/include)

string(CONCAT _core_lib_  "opencv_core" ${VERSION} ".lib" )

find_path(OPENCV_LIBRARY_DIR
	NAMES  ${_core_lib_}
	PATHS ${_WITH_SOURCE_}/x64/vc14/lib)


find_path(OPENCV_LIBRARY_PATH
	NAMES  ${_core_lib_}
	PATHS ${_WITH_SOURCE_}/x64/vc14/lib)


endif(_WITH_BUILD_)
#mark_as_advanced(${OPENCV_LIBRARY_PATH})

string(CONCAT _calib_lib_  "opencv_calib3d" ${VERSION}  )
find_library(OPENCV_CALIB ${_calib_lib_}  PATHS ${OPENCV_LIBRARY_PATH})
string(CONCAT _calib_lib_d  "opencv_calib3d" ${VERSION} "d" )
find_library(OPENCV_CALIB_DEBUG ${_calib_lib_d}  PATHS ${OPENCV_LIBRARY_PATH_D})

string(CONCAT _contrib_lib_  "opencv_contrib" ${VERSION}  )
find_library(OPENCV_CONTRIB ${_contrib_lib_}  PATHS ${OPENCV_LIBRARY_PATH})
string(CONCAT _contrib_lib_d  "opencv_contrib" ${VERSION} "d" )
find_library(OPENCV_CONTRIB_DEBUG ${_contrib_lib_d}  PATHS ${OPENCV_LIBRARY_PATH_D})

string(CONCAT _core_lib_  "opencv_core" ${VERSION}  )
find_library(OPENCV_CORE ${_core_lib_}  PATHS ${OPENCV_LIBRARY_PATH})
string(CONCAT _core_lib_d  "opencv_core" ${VERSION} "d" )
find_library(OPENCV_CORE_DEBUG ${_core_lib_d}  PATHS ${OPENCV_LIBRARY_PATH_D})

string(CONCAT _feature_lib_  "opencv_features2d" ${VERSION}  )
find_library(OPENCV_FEATURE ${_feature_lib_}  PATHS ${OPENCV_LIBRARY_PATH})
string(CONCAT _feature_lib_d  "opencv_features2d" ${VERSION} "d" )
find_library(OPENCV_FEATURE_DEBUG ${_feature_lib_d}  PATHS ${OPENCV_LIBRARY_PATH_D})

string(CONCAT _flann_lib_  "opencv_flann" ${VERSION}  )
find_library(OPENCV_FLANN ${_flann_lib_}  PATHS ${OPENCV_LIBRARY_PATH})
string(CONCAT _flann_lib_d  "opencv_flann" ${VERSION} "d" )
find_library(OPENCV_FLANN_DEBUG ${_flann_lib_d}  PATHS ${OPENCV_LIBRARY_PATH_D})

string(CONCAT _highgui_lib_  "opencv_highgui" ${VERSION}  )
find_library(OPENCV_HIGHGUI ${_highgui_lib_}  PATHS ${OPENCV_LIBRARY_PATH})
string(CONCAT _highgui_lib_d  "opencv_highgui" ${VERSION} "d" )
find_library(OPENCV_HIGHGUI_DEBUG ${_highgui_lib_d}  PATHS ${OPENCV_LIBRARY_PATH_D})

string(CONCAT _imgproc_lib_  "opencv_imgproc" ${VERSION}  )
find_library(OPENCV_IMGPROC ${_imgproc_lib_}  PATHS ${OPENCV_LIBRARY_PATH})
string(CONCAT _imgproc_lib_d  "opencv_imgproc" ${VERSION} "d" )
find_library(OPENCV_IMGPROC_DEBUG ${_imgproc_lib_d}  PATHS ${OPENCV_LIBRARY_PATH_D})

string(CONCAT _legacy_lib_  "opencv_legacy" ${VERSION}  )
find_library(OPENCV_LEGACY ${_legacy_lib_}  PATHS ${OPENCV_LIBRARY_PATH})
string(CONCAT _legacy_lib_d  "opencv_legacy" ${VERSION} "d" )
find_library(OPENCV_LEGACY_DEBUG ${_legacy_lib_d}  PATHS ${OPENCV_LIBRARY_PATH_D})

string(CONCAT _ml_lib_  "opencv_ml" ${VERSION}  )
find_library(OPENCV_ML ${_ml_lib_}  PATHS ${OPENCV_LIBRARY_PATH})
string(CONCAT _ml_lib_d  "opencv_ml" ${VERSION} "d" )
find_library(OPENCV_ML_DEBUG ${_ml_lib_d}  PATHS ${OPENCV_LIBRARY_PATH_D})

string(CONCAT _nonfree_lib_  "opencv_nonfree" ${VERSION}  )
find_library(OPENCV_NONFREE ${_nonfree_lib_}  PATHS ${OPENCV_LIBRARY_PATH})
string(CONCAT _nonfree_lib_d  "opencv_nonfree" ${VERSION} "d" )
find_library(OPENCV_NONFREE_DEBUG ${_nonfree_lib_d}  PATHS ${OPENCV_LIBRARY_PATH_D})

string(CONCAT _objdetect_lib_  "opencv_objdetect" ${VERSION}  )
find_library(OPENCV_OBJDETECT ${_objdetect_lib_}  PATHS ${OPENCV_LIBRARY_PATH})
string(CONCAT _objdetect_lib_d  "opencv_objdetect" ${VERSION} "d")
find_library(OPENCV_OBJDETECT_DEBUG ${_objdetect_lib_d}  PATHS ${OPENCV_LIBRARY_PATH_D})

string(CONCAT _ts_lib_  "opencv_ts" ${VERSION}  )
find_library(OPENCV_TS ${_ts_lib_}  PATHS ${OPENCV_LIBRARY_PATH})
string(CONCAT _ts_lib_d  "opencv_ts" ${VERSION} "d")
find_library(OPENCV_TS_DEBUG ${_ts_lib_d}  PATHS ${OPENCV_LIBRARY_PATH_D})

string(CONCAT _video_lib_  "opencv_video" ${VERSION}  )
find_library(OPENCV_VIDEO ${_video_lib_}  PATHS ${OPENCV_LIBRARY_PATH})
string(CONCAT _video_lib_d  "opencv_video" ${VERSION} "d")
find_library(OPENCV_VIDEO_DEBUG ${_video_lib_d}  PATHS ${OPENCV_LIBRARY_PATH_D})


string(CONCAT _imgcodecs_lib_  "opencv_imgcodecs" ${VERSION}  )
find_library(OPENCV_IMGCODECS ${_imgcodecs_lib_}  PATHS ${OPENCV_LIBRARY_PATH})
string(CONCAT _imgcodecs_lib_d  "opencv_imgcodecs" ${VERSION} "d")
find_library(OPENCV_IMGCODECS_DEBUG ${_imgcodecs_lib_d}  PATHS ${OPENCV_LIBRARY_PATH_D})


set(OPENCV_LIBRARIES
	optimized ${OPENCV_CALIB}
	#optimized ${OPENCV_CONTRIB}
	optimized ${OPENCV_CORE}
	optimized ${OPENCV_FEATURE}
	optimized ${OPENCV_FEATURE}
	optimized ${OPENCV_FLANN}
	optimized ${OPENCV_HIGHGUI}
	optimized ${OPENCV_IMGPROC}
	#optimized ${OPENCV_LEGACY}
	optimized ${OPENCV_ML}
	#optimized ${OPENCV_NONFREE}
	optimized ${OPENCV_OBJDETECT}
	optimized ${OPENCV_TS}
	optimized ${OPENCV_VIDEO}
	optimized ${OPENCV_IMGCODECS}
	
	debug ${OPENCV_CALIB_DEBUG}
	#debug ${OPENCV_CONTRIB_DEBUG}
	debug ${OPENCV_CORE_DEBUG}
	debug ${OPENCV_FEATURE_DEBUG}
	debug ${OPENCV_FEATURE_DEBUG}
	debug ${OPENCV_FLANN_DEBUG}
	debug ${OPENCV_HIGHGUI_DEBUG}
	debug ${OPENCV_IMGPROC_DEBUG}
	#debug ${OPENCV_LEGACY_DEBUG}
	debug ${OPENCV_ML_DEBUG}
	#debug ${OPENCV_NONFREE_DEBUG}
	debug ${OPENCV_OBJDETECT_DEBUG}
	debug ${OPENCV_TS_DEBUG}
	debug ${OPENCV_VIDEO_DEBUG}
	debug ${OPENCV_IMGCODECS_DEBUG}
 )
 
