# WebDataScraper
 C++ based website data scraper <br>
 Compiled via mingw32 gcc <br>

# Pre-requisites:
 vcpkg (installation guide: [build w/ cmake](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-cmd)) <br>
 NOTE: Only need to complete up to Step 2.1 Configure the VCPKG_ROOT environment variable. <br>
 -------------------------------
 w/ vcpkg installed and added to path: <br>
 install [libcurl](https://curl.se/libcurl/): An open-source and easy-to-use HTTP client for C and C++ built on top of cURL. <br>
 NOTE: I highly recommend checking out [libcurl documentation](https://curl.se/libcurl/c/) if you are new to web scraping / c++. <br> 
 install [libxml2](https://gitlab.gnome.org/GNOME/libxml2/): A HTML and XML parser with a complete element selection API based on XPath. <br>
 -------------------------------
 ex: vcpkg install curl <br>
 ex: vcpkg install libxml2 <br>
 -------------------------------
 Run vcpkg integration script: <br>
 vcpkg integrate install <br>
