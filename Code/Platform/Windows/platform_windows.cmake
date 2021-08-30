
set(livepp_path ${CMAKE_CURRENT_LIST_DIR}/../../../3rdParty/LivePP/API/LPP_API.h)
#message(livepp_path="${livepp_path}")

if(EXISTS ${livepp_path})

    set(LY_COMPILE_DEFINITIONS
        PRIVATE
            ENABLE_LIVEPP
    )

endif()
