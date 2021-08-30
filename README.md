# LivePlusPlus_O3DE_Gem
An integration of https://liveplusplus.tech/ for Open 3D Engine (Windows only)

## Installation
1. Pull down this repository, for example to `C:\git\LivePlusPlus_O3DE_Gem\`.
1. Register it as a gem for your O3DE project. 
      See https://o3de.org/docs/user-guide/project-config/register-gems/ for details. 
      For example:
      
      `C:\git\o3de> .\scripts\o3de.bat register --gem-path C:\git\LivePlusPlus_O3DE_Gem\ --external-subdirectory-project-path C:\git\YourProject\`
1. Get a copy of Live++ from https://liveplusplus.tech/.
1. Unzip it into `C:\git\LivePlusPlus_O3DE_Gem\3rdParty\LivePP`.
1. Compile your project and run.

## Configuration
1. Refer to https://liveplusplus.tech/docs/documentation.html for Live++ docs.
1. By default, all gems registered for your O3DE project will be hot patched.
1. Optionally, you can set a regex filter to choose which O3DE gems to hot patch in the following file.

    `C:\git\LivePlusPlus_O3DE_Gem\Code\Platform\Windows\LivePlusPlusSystemComponent_Windows.h`

    `std::regex m_approvedRegex{ ".*" }; // Matches any path`

## Troubleshooting
1. You can use `Code\Platform\Windows\platform_windows.cmake` to enable a trace line to see what path this gem is expected to find Live++ library at.

    `set(livepp_path ${CMAKE_CURRENT_LIST_DIR}/../../../3rdParty/LivePP/API/LPP_API.h)`

    `#message(livepp_path="${livepp_path}")`


Enjoy!
