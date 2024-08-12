#include <iostream>
#include <algorithm>
#include <cctype>  // for std::isspace
#include <string>
#include <curl/curl.h>
#include "libxml/HTMLparser.h"
#include "libxml/xpath.h"

// Callback function used for CURL to process data buffer
size_t WriteCB(void* contents, size_t size, size_t nmemb, std::string* response) {
    size_t total_size = size * nmemb;
    // Append the response to contents of buffer with total_size being buffer length
    response->append((char*)contents, total_size);

    // Debugging output for the callback
    std::cout << "Received " << total_size << " bytes of data." << std::endl;

    // return buffer size for CURL operation
    return total_size;
}


std::string get_request(const std::string& url) {
    // initialize curl locally
    CURL* curl = curl_easy_init();
    std::string result;

    if (curl) {
        // perform the GET request
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L); // Toggle Verbose Debug Level: 0L (none) -> 1L (standard)

        // Convert URL to proper CURL string
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        
        // pass "chunk" to callback function for write operations
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCB);

        // Stored returned data to local string variable
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        
        // ensure CURL perform occurs properly
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        // free up the local curl resources
        curl_easy_cleanup(curl);
    }
    // return un-processed scraped string
    return result;
}

int main() {
    // initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);

    // download the target HTML document 
    // and print it
    std::string html_document = get_request("https://www.scrapingcourse.com/ecommerce/");
    std::cout << html_document;

    // scraping logic... Now we need to post-process

    // free up the global curl resources
    curl_global_cleanup();

    return 0;
}
