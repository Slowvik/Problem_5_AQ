#include "auction_prices.h"
#include "flat_hash_map.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <chrono>
#include <algorithm>

int main()
{
    AP::AuctionPrices AuctionHouse;std::cout<<"Example print: "<<std::endl;

    AuctionHouse.addNewOrder("item1", "auction1", 1, 100);
    AuctionHouse.addNewOrder("item1", "auction2", 1, 101);
    AuctionHouse.addNewOrder("item2", "order3", 1, 99);
    AuctionHouse.addNewOrder("item2", "order4", 2, 100);

    AuctionHouse.print();

    //Time calcs:

    std::cout<<std::endl<<std::endl;

    int test_size = 1000000;
    std::string base = "auction";
    std::vector<std::string> string_vec;

    for(int i=0;i<test_size;i++)
    {
        string_vec.push_back(base+std::to_string(i));
        //std::cout<<string_vec[i]<<std::endl;
    }    
    

    //Insertion performance for simple <int,int> map
    std::map<int,int> orderbook_bids;

    auto startTime = std::chrono::high_resolution_clock::now();
    for(int i=0;i<test_size;i++)
    {
        orderbook_bids.insert(std::make_pair(i,i+100));
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout<<std::setw(20) << std::left <<test_size<<std::setw(40) << std::left<< "insertion - <int,int>:"<<std::setw(20) << std::right <<std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - startTime).count()<<" ms"<<std::endl; 

    //Insertion performance of simple <string,int> map:
    std::map<std::string,int> orderbook_bids_string;
    startTime = std::chrono::high_resolution_clock::now();
    for(int i=0;i<test_size;i++)
    {
        orderbook_bids_string.insert(std::make_pair(string_vec[i],i+100));
    }
    endTime = std::chrono::high_resolution_clock::now();
    std::cout<<std::setw(20) << std::left <<test_size<<std::setw(40) << std::left<< "insertion - <string,int>:"<<std::setw(20) << std::right <<std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - startTime).count()<<" ms"<<std::endl; 

    //Insertion performance for flat_hash_map
    AP::AuctionPrices House2;
    std::string base_item_ID = "item3";

    startTime = std::chrono::high_resolution_clock::now();
    for(int i=0; i<test_size; i++)
    {
        std::string i_ID = base_item_ID;
        int insert_status = House2.addNewOrder(i_ID.c_str(), string_vec[i].c_str(), 1, i+100);
    }
    endTime = std::chrono::high_resolution_clock::now();
    std::cout<<std::setw(20) << std::left <<test_size<<std::setw(40) << std::left<< "insertion - AuctionPrices with flat_hash_map:"<<std::setw(20) << std::right <<std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - startTime).count()<<" ms"<<std::endl;

    std::cout<<std::endl;




    //Deletion performance for simple map <int,int>:

    startTime = std::chrono::high_resolution_clock::now();
    for(int i=test_size-1; i>=0; i--)
    {
        //std::string i_ID = base_item_ID;
        orderbook_bids.erase(i);
    }
    endTime = std::chrono::high_resolution_clock::now();
    std::cout<<std::setw(20) << std::left <<test_size<<std::setw(40) << std::left<< "deletion - <int,int>:"<<std::setw(20) << std::right <<std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - startTime).count()<<" ms"<<std::endl;

    //Deletion performance for simple map <string,int>:

    startTime = std::chrono::high_resolution_clock::now();
    for(int i=test_size-1; i>=0; i--)
    {
        //std::string i_ID = base_item_ID;
        orderbook_bids_string.erase(string_vec[i]);
    }
    endTime = std::chrono::high_resolution_clock::now();
    std::cout<<std::setw(20) << std::left <<test_size<<std::setw(40) << std::left<< "deletion - <string,int>:"<<std::setw(20) << std::right <<std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - startTime).count()<<" ms"<<std::endl;

    //Deletion performance for flat_hash_map:

    startTime = std::chrono::high_resolution_clock::now();
    for(int i=test_size-1; i>=0; i--)
    {
        //std::string i_ID = base_item_ID;
        int delete_status = House2.deleteOrder(base_item_ID.c_str(), string_vec[i].c_str());
    }
    endTime = std::chrono::high_resolution_clock::now();
    std::cout<<std::setw(20) << std::left <<test_size<<std::setw(40) << std::left<< "deletion - AuctionPrices with flat_hash_map:"<<std::setw(20) << std::right <<std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - startTime).count()<<" ms"<<std::endl;


    

    

    std::cout<<std::endl;

    //Combined insert-delete <int,int>:
    AP::AuctionPrices House3;
    startTime = std::chrono::high_resolution_clock::now();
    for(int i=0;i<test_size/2;i++)
    {
        orderbook_bids.insert(std::make_pair(i,i+100));
    }
    for(int i=(test_size/2)-1; i>=0; i--)
    {
        orderbook_bids.erase(i);
    }
    endTime = std::chrono::high_resolution_clock::now();
    std::cout<<std::setw(20) << std::left <<test_size<<std::setw(40) << std::left<< "50:50 - <int, int>:"<<std::setw(20) << std::right <<std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - startTime).count()<<" ms"<<std::endl;

    //Combined insert-delete <string,int>:

    startTime = std::chrono::high_resolution_clock::now();
    for(int i=0;i<test_size/2;i++)
    {
        orderbook_bids_string.insert(std::make_pair(string_vec[i],i+100));
    }
    for(int i=(test_size/2)-1; i>=0; i--)
    {
        orderbook_bids_string.erase(string_vec[i]);
    }
    endTime = std::chrono::high_resolution_clock::now();
    std::cout<<std::setw(20) << std::left <<test_size<<std::setw(40) << std::left<< "50:50 - <string, int>:"<<std::setw(20) << std::right <<std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - startTime).count()<<" ms"<<std::endl;

    //COMBINED INSERTION-DELETION performance for flat_hash_map:

    startTime = std::chrono::high_resolution_clock::now();
    for(int i=0; i<test_size/2; i++)
    {
        //std::string i_ID = base_item_ID;
        int insert_status = House3.addNewOrder(base_item_ID.c_str(), string_vec[i].c_str(), 1, i+100);
    }
    for(int i=(test_size/2)-1; i>=0; i--)
    {
        //std::string i_ID = base_item_ID;
        int delete_status = House3.deleteOrder(base_item_ID.c_str(), string_vec[i].c_str());
    }
    endTime = std::chrono::high_resolution_clock::now();
    std::cout<<std::setw(20) << std::left <<test_size<<std::setw(40) << std::left<< "50:50 - AuctionPrices with flat_hash_map:"<<std::setw(20) << std::right <<std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - startTime).count()<<" ms"<<std::endl;




    /*
    Conclusions:
    1. hashing before inserting string,int takes MUCH longer (30% longer with 100,000 insertions, 100% longer with 500,000 insertions, 160% longer with 1,000,000 insertions). This is because we are actually performing two inserts: one in hashed string vs string and the other in the regular orderbook. After checking the performance, unordered_map works better than map for the hasher_lookup data structure.

    2. the reverse is true for deletions: deleting from hashed map is MUCH faster (about 400% faster with 1,000,000 entries)
    */


    return 0;
}
