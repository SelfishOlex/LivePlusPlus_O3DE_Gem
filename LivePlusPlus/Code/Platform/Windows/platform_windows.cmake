
set(test_livepp ${CMAKE_CURRENT_LIST_DIR}/../../../3rdParty/LivePP)
message("Looking for Live++ deployment folder at ${test_livepp}")

if(EXISTS ${test_livepp})
    message("Live++ deployment found. The gem is ENABLED.")

    set(LY_COMPILE_DEFINITIONS
        PRIVATE
            # If you wish to disable Live++, comment out the next line.
            ENABLE_LIVEPP
    )


    # Place the following sections in each of your gems that you wish to fully work with Live++
    # Otherwise, Live++ will tell you in the log which binaries don't have the supported settings on.
    # See https://liveplusplus.tech/docs/documentation.html#compiler_settings for details.
    # See https://liveplusplus.tech/docs/documentation.html#linker_settings for details.
    set(LY_LINK_OPTIONS
        PRIVATE
            /FUNCTIONPADMIN
            /OPT:NOREF
            /OPT:NOICF
            /DEBUG:FULL
    )

    set(LY_COMPILE_OPTIONS
        PRIVATE
            /Z7
            /Zi
            /Gm-
            /Gy
            /Gw
    )

    # Here is a good way to do so for each gem:
    # 1. Create a PAL build file, such as: <Your Gem>\Code\Platform\Windows\platform_windows.cmake
    # 2. Copy the above two settings (LY_LINK_OPTIONS and LY_COMPILE_OPTIONS) into that file.
    # 3. For your .Static and gem target:
    #       ly_add_target(
    #       NAME MyGem.Static STATIC
    #       
    #       ly_add_target(
    #       NAME MyGem ${PAL_TRAIT_MONOLITHIC_DRIVEN_MODULE_TYPE}
    #    
    #    Add the following reference:
    #       PLATFORM_INCLUDE_FILES
    #            ${pal_dir}/platform_${PAL_PLATFORM_NAME_LOWERCASE}.cmake

else()
    message("Live++ deployment is not found. Live++ gem is DISABLED. Check that your have unpacked Live++ to ${test_livepp}")
endif()
