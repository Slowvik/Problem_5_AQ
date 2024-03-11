#include "auction_prices.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <chrono>
#include <algorithm>

//https://codereview.stackexchange.com/questions/285623/fast-orderbook-implementation

int main()
{
    int test_size = 1000000;
    std::string base = "auction";
    std::vector<std::string> string_vec;

    for(int i=0;i<test_size;i++)
    {
        string_vec.push_back(base+std::to_string(i));
        //std::cout<<string_vec[i]<<std::endl;
    }

    std::map<std::string,int> orderbook_bids;
    std::map<int,std::string> orderbook_reverse;

    auto startTime = std::chrono::high_resolution_clock::now();
    for(int i=0;i<test_size;i++)
    {
        orderbook_bids.insert(make_pair(string_vec[i],i+100));
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout<<"Time taken to insert "<<test_size<<" string,int pairs in map: "<<std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - startTime).count()<<" milliseconds"<<std::endl;

    startTime = std::chrono::high_resolution_clock::now();
    for(int i=0;i<test_size;i++)
    {
        orderbook_reverse.insert(make_pair(i+100,string_vec[i])); // This takes about half the time
    }
    endTime = std::chrono::high_resolution_clock::now();
    std::cout<<"Time taken to insert "<<test_size<<" int,string pairs in map: "<<std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - startTime).count()<<" milliseconds"<<std::endl;

    //Now we test search times
    std::string search_for = "auction900000";
    startTime = std::chrono::high_resolution_clock::now();
    for(auto k_v:orderbook_reverse)
    {
        if(k_v.second==search_for)
        {
            std::cout<<k_v.first<<std::endl;
        }
    }
    endTime = std::chrono::high_resolution_clock::now();
    std::cout<<"Time taken to search for element \""<<search_for<<"\" in reverse map: "<<std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - startTime).count()<<" milliseconds"<<std::endl;

    int search_int = 900000;
    startTime = std::chrono::high_resolution_clock::now();
    for(auto k_v:orderbook_bids)
    {
        if(k_v.second==search_int)
        {
            std::cout<<k_v.first<<std::endl;//This also takes about half the time
        }
    }
    endTime = std::chrono::high_resolution_clock::now();
    std::cout<<"Time taken to search for element \""<<search_int<<"\" in map: "<<std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - startTime).count()<<" milliseconds"<<std::endl;


    //Now we hash the string before inserting
    std::map<int,size_t> orderbook_hashed;
    std::hash<std::string> hasher;
    startTime = std::chrono::high_resolution_clock::now();
    for(int i=0;i<test_size;i++)
    {
        orderbook_hashed.insert(std::make_pair(i+100,hasher(string_vec[i]))); //somehow faster than BOTH int,string and string,int (about 3% faster than int,string)
    }
    endTime = std::chrono::high_resolution_clock::now();
    std::cout<<"Time taken to insert "<<test_size<<" int,hashed string (size_t) pairs in map: "<<std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - startTime).count()<<" milliseconds"<<std::endl;

    //Search for hash code:
    size_t search_hashed = hasher(search_for);
    startTime = std::chrono::high_resolution_clock::now();
    for(auto k_v:orderbook_hashed)
    {
        if(k_v.second==search_hashed)
        {
            std::cout<<k_v.first<<std::endl;//25% faster than searching for an int in string,int map
        }
    }
    endTime = std::chrono::high_resolution_clock::now();
    std::cout<<"Time taken to search for element \""<<search_hashed<<"\" in hashed map(lol): "<<std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - startTime).count()<<" milliseconds"<<std::endl;

    /*
    CONCLUSION: DEFINITELY HASH YOUR STRINGS
    BUT: I don't think this is an option. If we just had to insert, delete and search, it would work, but we also have to print and print requiresthe originial strings for all million entires. You can't unhash a hashed string.
    Anyway, difference between int,string and int,hash isn't much compared to string,int and int,string
    */



    system("PAUSE");
    return 0;
}