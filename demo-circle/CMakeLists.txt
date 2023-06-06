add_executable(demo-circle
        demo_circle.c
        opt/lwipopts.h
        handlers.c
        handlers.h
        )

target_link_libraries(demo-circle
	pico_stdio
        pico_stdlib
        picow_http
        pico_cyw43_arch_lwip_threadsafe_background
        pico_multicore
        )

# create map/bin/hex file etc.
pico_add_extra_outputs(demo-circle)

# add url via pico_set_program_url
example_auto_set_url(demo-circle)

pico_enable_stdio_uart(demo-circle 0)
pico_enable_stdio_usb(demo-circle 1)

target_include_directories(demo-circle
	PRIVATE	opt
	PRIVATE ${CMAKE_SOURCE_DIR}/lib/picow-http
)

add_subdirectory(../lib/picow-http picow)

picow_http_gen_handlers(demo-circle
	${CMAKE_CURRENT_SOURCE_DIR}/static/config.yaml
	${CMAKE_CURRENT_SOURCE_DIR}/static/templates
	${CMAKE_CURRENT_SOURCE_DIR}/static/templates/index.html
)
