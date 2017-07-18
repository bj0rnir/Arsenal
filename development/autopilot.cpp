#include <iostream>
#include <string>
#include <regex>
#include <curl/curl.h>
#define POPULAR_ITEMS "https://www.g2a.com/lucene/search/filter?jsoncallback=jQuery111007667557919303079_1500323788288&skip=&minPrice=0.00&maxPrice=900.00&cc=US&stock=all&event=bestseller&platform=0&search=&genre=0&cat=0&sortOrder=popularity+desc&steam_app_id=&steam_category=&steam_prod_type=&includeOutOfStock=&includeFreeGames=false&isWholesale=false&_=1500323788290&start="
#define G2A_BESTSELLERS 0x01
#define DYN "&start=0&rows=12"
#define CURL_PREP(curl){    \
  #ifdef SKIP_PEER_VERIFICATION     \
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);   \
  #endif    \
  #ifdef SKIP_HOSTNAME_VERIFICATION \
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);   \
  #endif    \
}   \

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


void updateITEMS(int num){
  
  int remainder = 0;
  if (!(num <= 24)){
    remainder = num % 24;
  }
  CURL* curl;
  CURLcode res;
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  std::string buf;
  if (remainder == 0) {
    std::string url = POPULAR_ITEMS;
    url += "&start=0&rows=";
    url += std::to_string(num);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);
    res = curl_easy_perform(curl);
    if(res != CURLE_OK){
        std::cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    }
    curl_easy_cleanup(curl);
    } else {
        int i = num;
        while (num){
            std::string url;
            if (i < 24){
                url = POPULAR_ITEMS;
                url += std::to_string(num - i);
                url += "&rows="
                url += std::to_string(i);
                curl_easy_setopt(curl, CURLOPT_URL, url);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);
                i = 0;
            } else {
                url = POPULAR_ITEMS;
                url += std::to_string(num - i);
                url += "&rows=24";
                i += 24;
        }
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);
        CURL_PREP(curl);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK){
            std::cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
    }
  curl_global_cleanup();
  std::regex r("(?:\"id\":)(\\d+)");
  std::smatch m;
  for(auto it = std::sregex_iterator(buf.begin(), buf.end(), r);
    it != std::sregex_iterator();
     ++it)
    {
    std::cout << "New Product ID:\t" << (*it)[1] << std::endl;
  }
}
int main(55){
  updateITEMS();
  return 1;
}
