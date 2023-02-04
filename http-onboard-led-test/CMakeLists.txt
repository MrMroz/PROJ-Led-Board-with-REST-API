add_executable(http_blink
        http_blink.c
        opt/lwipopts.h
        handlers.c
        handlers.h
        )

target_link_libraries(http_blink
		pico_stdio
        pico_stdlib
        picow_http
        pico_cyw43_arch_lwip_threadsafe_background
        pico_multicore
        )

# create map/bin/hex file etc.
pico_add_extra_outputs(http_blink)

# add url via pico_set_program_url
example_auto_set_url(http_blink)

pico_enable_stdio_uart(http_blink 0)
pico_enable_stdio_usb(http_blink 1)

target_include_directories(http_blink
	PRIVATE	opt
	PRIVATE ${CMAKE_SOURCE_DIR}/lib/picow_http
)

add_subdirectory(../lib/picow_http picow)

picow_http_gen_handlers(http_blink
	${CMAKE_CURRENT_SOURCE_DIR}/static/config.yaml
	${CMAKE_CURRENT_SOURCE_DIR}/static/templates
	${CMAKE_CURRENT_SOURCE_DIR}/static/templates/index.html
)
