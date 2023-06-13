add_executable(demo
        main.c
        lib/matrix.c
        lib/symbols.c
        lib/controller.c
        lib/handlers.c
        opt/lwipopts.h
        )

target_link_libraries(demo
	pico_stdio
        pico_stdlib
        pico_multicore
        pico_cyw43_arch_none
        picow_http
        pico_cyw43_arch_lwip_threadsafe_background
        )

# create map/bin/hex file etc.
pico_add_extra_outputs(demo)

# add url via pico_set_program_url
example_auto_set_url(demo)

pico_enable_stdio_uart(demo 0)
pico_enable_stdio_usb(demo 1)

target_include_directories(demo
	PRIVATE	opt
	PRIVATE ${CMAKE_SOURCE_DIR}/lib/picow-http
)

add_subdirectory(../lib/picow-http picow)

picow_http_gen_handlers(demo
	${CMAKE_CURRENT_SOURCE_DIR}/static/config.yaml
	${CMAKE_CURRENT_SOURCE_DIR}/static/templates
	${CMAKE_CURRENT_SOURCE_DIR}/static/templates/index.html
)