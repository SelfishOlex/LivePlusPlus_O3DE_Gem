
# Currently we are in the Code folder: ${CMAKE_CURRENT_LIST_DIR}
# Get the platform specific folder ${pal_dir} for the current folder: ${CMAKE_CURRENT_LIST_DIR}/Platform/${PAL_PLATFORM_NAME}
# Note: ly_get_list_relative_pal_filename will take care of the details for us, as this may be a restricted platform
#       in which case it will see if that platform is present here or in the restricted folder.
#       i.e. It could here in our gem : Gems/LivePlusPlus/Code/Platform/<platorm_name>  or
#            <restricted_folder>/<platform_name>/Gems/LivePlusPlus/Code
o3de_pal_dir(pal_dir "${CMAKE_CURRENT_LIST_DIR}/Platform/${PAL_PLATFORM_NAME}" "${o3de_gem_restricted_path}" "${o3de_gem_path}" "${o3de_gem_name}")

# Now that we have the platform abstraction layer (PAL) folder for this folder, thats where we will find the
# traits for this platform. Traits for a platform are defines for things like whether or not something in this gem
# is supported by this platform.
include(${pal_dir}/PAL_${PAL_PLATFORM_NAME_LOWERCASE}.cmake)

# Add the LivePlusPlus.Static target
# Note: We include the common files and the platform specific files which are set in liveplusplus_common_files.cmake
# and in ${pal_dir}/liveplusplus_${PAL_PLATFORM_NAME_LOWERCASE}_files.cmake
ly_add_target(
    NAME LivePlusPlus.Static STATIC
    NAMESPACE Gem
    FILES_CMAKE
        ${pal_dir}/liveplusplus_${PAL_PLATFORM_NAME_LOWERCASE}_files.cmake
    PLATFORM_INCLUDE_FILES
        ${pal_dir}/platform_${PAL_PLATFORM_NAME_LOWERCASE}.cmake
    INCLUDE_DIRECTORIES
        PUBLIC
            Include
            ${pal_dir}
        PRIVATE
            Source
    BUILD_DEPENDENCIES
        PUBLIC
            AZ::AzCore
            AZ::AzFramework
            Legacy::CryCommon
)

# Here add LivePlusPlus target, it depends on the LivePlusPlus.Static
ly_add_target(
    NAME LivePlusPlus ${PAL_TRAIT_MONOLITHIC_DRIVEN_MODULE_TYPE}
    NAMESPACE Gem
    FILES_CMAKE
        ${pal_dir}/liveplusplus_shared_${PAL_PLATFORM_NAME_LOWERCASE}_files.cmake
    INCLUDE_DIRECTORIES
        PUBLIC
            Include
        PRIVATE
            Source
    BUILD_DEPENDENCIES
        PRIVATE
            Gem::LivePlusPlus.Static
)

# By default, we will specify that the above target LivePlusPlus would be used by
# Client and Server type targets when this gem is enabled.  If you don't want it
# active in Clients or Servers by default, delete one of both of the following lines:
ly_create_alias(NAME LivePlusPlus.Clients  NAMESPACE Gem TARGETS Gem::LivePlusPlus)
ly_create_alias(NAME LivePlusPlus.Servers  NAMESPACE Gem TARGETS Gem::LivePlusPlus)
ly_create_alias(NAME LivePlusPlus.Tools    NAMESPACE Gem TARGETS Gem::LivePlusPlus)
