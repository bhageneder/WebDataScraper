# WebDataScraper
 C++ based website data scraper
 Compiled via mingw32 gcc

# Pre-requisites:
 vcpkg (installation guide: [build w/ cmake](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-cmd))
 NOTE: Only need to complete up to Step 2.1 Configure the VCPKG_ROOT environment variable.
 -------------------------------
 w/ vcpkg installed and added to path: \n
 install [libcurl](https://curl.se/libcurl/): An open-source and easy-to-use HTTP client for C and C++ built on top of cURL.
 install [libxml2](https://gitlab.gnome.org/GNOME/libxml2/): A HTML and XML parser with a complete element selection API based on XPath.
 -------------------------------
 ex: vcpkg install curl
 ex: vcpkg install libxml2
 -------------------------------
 Run vcpkg integration script:
 vcpkg integrate install
