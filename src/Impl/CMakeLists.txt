set(IMPL_HEADERS
	headers/enum_impl.hpp
	headers/utils_impl.hpp
	headers/Processing_impl.hpp
	headers/Display_impl.hpp
	)

set(IMPL_SOURCES
	src/utils_impl.cpp
	src/utils_impl.tpp
	src/Processing_impl.cpp
	src/Display_impl.cpp
	)

add_library(impl SHARED ${IMPL_HEADERS} ${IMPL_SOURCES})
target_link_libraries(impl ${OpenCV_LIBS})
install(TARGETS impl LIBRARY DESTINATION lib)
