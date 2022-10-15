# LivePlusPlus_O3DE_Gem

An integration of [Live++](https://liveplusplus.tech/) for [Open 3D Engine](https://o3de.org/) ([GitHub](https://github.com/o3de/o3de)) (Windows only).

*Important*: Live++ is NOT open source. See its website for details: [Pricing](https://liveplusplus.tech/pricing.html) and [Legal](https://liveplusplus.tech/legal_notice.html).

## Example Project

Refer to the included example project with Live++ enabled and configured:

- https://github.com/AMZN-Olex/LivePlusPlus_O3DE_Gem/tree/main/ExampleProject

> I left out the actual Live++ package due to copyright, of course!

## Compatiblity with Live++ versions

- Use 'main' branch for Live++ **2.0.x**
- Use 'v1' branch for Live++ **1.x**: https://github.com/AMZN-Olex/LivePlusPlus_O3DE_Gem/tree/v1

## Installation

1. Let's assume O3DE engine is at `C:\git\o3de` and your project is at `C:\git\YourProject`.
1. Pull down this repository, for example to `C:\git\Gems\LivePlusPlus_O3DE_Gem\LivePlusPlus`.
1. Register it as a gem for your O3DE project (see [Register Gems](https://o3de.org/docs/user-guide/project-config/register-gems/) for details). For example:
    - `C:\git\o3de> .\scripts\o3de.bat register --gem-path C:\git\Gems\LivePlusPlus_O3DE_Gem\LivePlusPlus --external-subdirectory-project-path C:\git\YourProject`.
1. Enable the `LivePlusPlus` gem in your project
    - This can be achieved by adding `LivePlusPlus` to `enabled_gems.json` in the `C:\git\YourProject` directory.
    - Or by using O3DE Project Manager.
1. Get your copy of [Live++](https://liveplusplus.tech/).
1. Unzip it into `C:\git\Gems\LivePlusPlus_O3DE_Gem\LivePlusPlus\3rdParty\LivePP`.
1. Compile your project and run.

## Modifying Compiler/Linker Settings for Hotpatching C++ code

### Enable Hotpatching for your project and your gems

For each project/gem you wish to hotpatch, do the following:

- create a PAL file for windows, such as: `Gems\MyGem\Code\Platform\Windows\enable_hotpatch_link_options_windows.cmake` with the following content:
    https://github.com/AMZN-Olex/LivePlusPlus_O3DE_Gem/blob/f0eea72f356d3e9e84a663e915316607b8d42b93/ExampleProject/Gem/Platform/Windows/platform_windows.cmake#L2-L17
- other platforms should have this file as empty, for example: `Gems\MyGem\Code\Platform\Linux\enable_hotpatch_linux.cmake`
    ```
    ```
- for a Gem/Project target in CMake include the above PAL file as:
    ```
    ly_add_target(
        NAME MyGem.Static STATIC
        ...
        PLATFORM_INCLUDE_FILES
            ${pal_dir}/enable_hotpatch_${PAL_PLATFORM_NAME_LOWERCASE}.cmake
        ...
    )
    > For example: https://github.com/AMZN-Olex/LivePlusPlus_O3DE_Gem/blob/f0eea72f356d3e9e84a663e915316607b8d42b93/ExampleProject/Gem/CMakeLists.txt#L28-L35

    ly_add_target(
        NAME MyGem ${PAL_TRAIT_MONOLITHIC_DRIVEN_MODULE_TYPE}
        ...
        PLATFORM_INCLUDE_FILES
            ${pal_dir}/enable_hotpatch_${PAL_PLATFORM_NAME_LOWERCASE}.cmake
        ...
    )
    ```
    > For example: https://github.com/AMZN-Olex/LivePlusPlus_O3DE_Gem/blob/f0eea72f356d3e9e84a663e915316607b8d42b93/ExampleProject/Gem/CMakeLists.txt#L45-L52

This will enable hotpatching just for gems and projects that you wish to hotpatch. You do need to make this change for each ${PAL_TRAIT_MONOLITHIC_DRIVEN_MODULE_TYPE} target, though. An additional improvement would be to create a common place for `enable_hotpatch_windows.cmake` PAL files and re-use them across Gems.


## Configuration

1. Refer to [Live++ documentation](https://liveplusplus.tech/docs/documentation.html).
1. By default, all gems registered for your O3DE project will be hot patched.
1. Optionally, you can set a regex filter to choose which O3DE gems by using O3DE registry.

    > Note: This can significantly improve performance of Live++ and is highly encouraged to improve iteration times.
    
    https://github.com/AMZN-Olex/LivePlusPlus_O3DE_Gem/blob/f0eea72f356d3e9e84a663e915316607b8d42b93/ExampleProject/Registry/liveplusplus.setreg#L1-L9

    Here are some other examples:
    - ".*" - Matches any path
    - ".*ImGui.*" - Matches ImGui gems
    - ".*ImGui.*|.*ScriptCanvas.*" - Matches ImGui and ScriptCanvas gem


That is it! You are done!



## Optional Advanced Configurations

### (Optional) Enable Hotpatching for the the entire engine

1. Go to `o3de\cmake\Platform\Common\MSVC\Configurations_msvc.cmake`
1. Make the following changes:
a. Profile Builds
    ```bash
    PS C:\git\o3de> git diff C:\git\o3de\cmake\Platform\Common\MSVC\Configurations_msvc.cmake
    diff --git a/cmake/Platform/Common/MSVC/Configurations_msvc.cmake b/cmake/Platform/Common/MSVC/Configurations_msvc.cmake
    index 9353e4eb1c..3ae8796b3c 100644
    --- a/cmake/Platform/Common/MSVC/Configurations_msvc.cmake
    +++ b/cmake/Platform/Common/MSVC/Configurations_msvc.cmake
    @@ -68,6 +68,8 @@ ly_append_configurations_options(
            /Zc:inline      # Removes unreferenced functions or data that are COMDATs or only have internal linkage
            /Zc:wchar_t     # Use compiler native wchar_t
            /Zi             # Generate debugging information (no Edit/Continue)
    +        /Z7             # for hot patching C++ code
    +        /Gw             # for hot patching C++ code
        COMPILATION_RELEASE
            /Ox             # Full optimization
            /Ob2            # Inline any suitable function
    @@ -78,8 +80,9 @@ ly_append_configurations_options(
            /NOLOGO             # Suppress Copyright and version number message
            /IGNORE:4099        # 3rdParty linking produces noise with LNK4099
        LINK_NON_STATIC_PROFILE
    -        /OPT:REF            # Eliminates functions and data that are never referenced
    -        /OPT:ICF            # Perform identical COMDAT folding. Redundant COMDATs can be removed from the linker output
    +        /OPT:NOREF            # for hot patching C++ code
    +        /OPT:NOICF            # for hot patching C++ code
    +        /FUNCTIONPADMIN       # for hot patching C++ code
            /INCREMENTAL:NO
            /DEBUG              # Generate pdbs
        LINK_NON_STATIC_RELEASE
    ```
    b. Debug Builds
    ```bash
    PS E:\o3de> git diff cmake\Platform\Common\MSVC\Configurations_msvc.cmake
    diff --git a/cmake/Platform/Common/MSVC/Configurations_msvc.cmake b/cmake/Platform/Common/MSVC/Configurations_msvc.cmake
    index 66a5b7b01f..221d813e08 100644
    --- a/cmake/Platform/Common/MSVC/Configurations_msvc.cmake
    +++ b/cmake/Platform/Common/MSVC/Configurations_msvc.cmake
    @@ -70,6 +70,8 @@ ly_append_configurations_options(
                            # It also causes the compiler to place the library name MSVCRTD.lib into the .obj file.
            /Ob0            # Disables inline expansions
            /Od             # Disables optimization
    +       /Z7             # for hot patching C++ code
    +       /Gw             # for hot patching C++ code
        COMPILATION_PROFILE
            /GF             # Enable string pooling
            /Gy             # Function level linking
    @@ -88,9 +90,11 @@ ly_append_configurations_options(
        LINK
            /NOLOGO             # Suppress Copyright and version number message
            /IGNORE:4099        # 3rdParty linking produces noise with LNK4099
    +        /OPT:NOREF         # for hot patching C++ code
    +        /OPT:NOICF         # for hot patching C++ code
    +        /FUNCTIONPADMIN    # for hot patching C++ code
    +
        LINK_NON_STATIC_PROFILE
    -        /OPT:REF            # Eliminates functions and data that are never referenced
    -        /OPT:ICF            # Perform identical COMDAT folding. Redundant COMDATs can be removed from the linker output
         /INCREMENTAL:NO
    ```

### Enabling Live++ for other dynamic libraries
1. By default only AZ Modules are loaded for use with Live++. There are some dynamic libraries that are themselves not AZ Modules. It is possible to load these explicitly in `LivePlusPlusSystemComponent_Windows.cpp` with:

    ```c++
    const char* library = <full-path-to-dll>; // e.g. <path/to>/EditorLib.dll
    m_lppAgent.EnableModuleANSI(library, lpp::LPP_MODULES_OPTION_NONE);
    ```
    
    In the following section:
    https://github.com/AMZN-Olex/LivePlusPlus_O3DE_Gem/blob/f0eea72f356d3e9e84a663e915316607b8d42b93/LivePlusPlus/Code/Platform/Windows/LivePlusPlusSystemComponent_Windows.cpp#L169-L176

Enjoy!
