add_executable(shapes-api
        shapes_api.c
        opt/lwipopts.h
        handlers.c
        handlers.h
        lib/led_matrix.h
        lib/picture.h
        )

target_link_libraries(shapes-api
	pico_stdio
        pico_stdlib
        picow_http
        pico_cyw43_arch_lwip_threadsafe_background
        pico_multicore
        )

# create map/bin/hex file etc.
pico_add_extra_outputs(shapes-api)

# add url via pico_set_program_url
example_auto_set_url(shapes-api)

pico_enable_stdio_uart(shapes-api 0)
pico_enable_stdio_usb(shapes-api 1)

target_include_directories(shapes-api
	PRIVATE	opt
	PRIVATE ${CMAKE_SOURCE_DIR}/lib/picow-http
)

add_subdirectory(../lib/picow-http picow)

picow_http_gen_handlers(shapes-api
	${CMAKE_CURRENT_SOURCE_DIR}/static/config.yaml
	${CMAKE_CURRENT_SOURCE_DIR}/static/templates
	${CMAKE_CURRENT_SOURCE_DIR}/static/templates/index.html
)
