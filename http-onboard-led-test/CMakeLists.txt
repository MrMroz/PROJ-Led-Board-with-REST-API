add_executable(http_onboard_led_test
        http_onboard_led_test.cpp
        )

target_link_libraries(http_onboard_led_test
        pico_stdlib              # for core functionality
        pico_cyw43_arch_none     # we need Wifi to access the GPIO, but we don't need anything else
        httpserver
        )

# create map/bin/hex file etc.
pico_add_extra_outputs(http_onboard_led_test)

# add url via pico_set_program_url
example_auto_set_url(http_onboard_led_test)

add_subdirectory(libhttpserver)
