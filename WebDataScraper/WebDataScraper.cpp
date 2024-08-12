#include <iostream>
#include <vector>
#include <fstream>
#include <curl/curl.h> // for retriving HTML documents
#include "libxml/HTMLparser.h" // for parsing HTML documents
#include "libxml/xpath.h" // for organizing parsed info

struct Product {
    std::string url;
    std::string image;
    std::string name;
    std::string price;
};

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
        // perform GET request
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

        // free local curl resources
        curl_easy_cleanup(curl);
    }
    // return un-processed scraped string
    return result;
}

void* write_to_csv(std::vector<Product> products) {
    // create the CSV file of output
    std::ofstream csv_file("products.csv");
    // populate it with the header
    csv_file << "url,image,name,price" << std::endl;
    // populate the CSV output file
    for (int i = 0; i < products.size(); ++i) {
        // transform a Product instance to a
        // CSV string record
        Product p = products.at(i);
        std::string csv_record = p.url + "," + p.image + "," + p.name + "," + p.price;
        csv_file << csv_record << std::endl;
    }
    // free up the resources for the CSV file
    csv_file.close();

    return 0;
}

int main() {
    // initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);

    // download the target HTML document 
    // and print it
    std::string html_document = get_request("https://www.scrapingcourse.com/ecommerce/");
    std::cout << html_document;

    // scraping logic... Now we need to post-process

    // forward HTML document to libxml2
    htmlDocPtr doc = htmlReadMemory(html_document.c_str(), html_document.length(), nullptr, nullptr, HTML_PARSE_NOERROR);

    // retrieve all HTML li.product elements with the XPath selector
    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    xmlXPathObjectPtr product_html_elements = xmlXPathEvalExpression((xmlChar*)"//li[contains(@class, 'product')]", context);

    // initialize an array of "Product"
    std::vector<Product> products;

    // Iterate over the list of the product nodes and extract the desired data:
    for (int i = 0; i < product_html_elements->nodesetval->nodeNr; ++i) {
        // get the current element of the loop
        xmlNodePtr product_html_element = product_html_elements->nodesetval->nodeTab[i];

        // set the context to restrict XPath selectors
        // to the children of the current element
        xmlXPathSetContextNode(product_html_element, context);

        xmlNodePtr url_html_element = xmlXPathEvalExpression((xmlChar*)".//a", context)->nodesetval->nodeTab[0];
        std::string url = std::string(reinterpret_cast<char*>(xmlGetProp(url_html_element, (xmlChar*)"href")));
        xmlNodePtr image_html_element = xmlXPathEvalExpression((xmlChar*)".//a/img", context)->nodesetval->nodeTab[0];
        std::string image = std::string(reinterpret_cast<char*>(xmlGetProp(image_html_element, (xmlChar*)"src")));
        xmlNodePtr name_html_element = xmlXPathEvalExpression((xmlChar*)".//a/h2", context)->nodesetval->nodeTab[0];
        std::string name = std::string(reinterpret_cast<char*>(xmlNodeGetContent(name_html_element)));
        xmlNodePtr price_html_element = xmlXPathEvalExpression((xmlChar*)".//a/span", context)->nodesetval->nodeTab[0];
        std::string price = std::string(reinterpret_cast<char*>(xmlNodeGetContent(price_html_element)));

        Product product = { url, image, name, price };
        products.push_back(product);
    }

    // free libxml2 resources
    xmlXPathFreeContext(context);
    xmlFreeDoc(doc);

    write_to_csv(products);

    // free global curl resources
    curl_global_cleanup();

    return 0;
}
