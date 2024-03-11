#include "auction_prices.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <chrono>
#include <algorithm>

int main()
{
    AP::AuctionPrices AuctionHouse;

    AuctionHouse.addNewOrder_hashed("item1", "auction1", 1, 100);
    AuctionHouse.addNewOrder_hashed("item1", "auction2", 1, 101);
    AuctionHouse.addNewOrder_hashed("item2", "order3", 1, 99);
    AuctionHouse.addNewOrder_hashed("item2", "order4", 2, 100);

    AuctionHouse.print_hashed();

    //Time calcs:

    int test_size = 1000000;
    std::string base = "auction";
    std::vector<std::string> string_vec;

    for(int i=0;i<test_size;i++)
    {
        string_vec.push_back(base+std::to_string(i));
        //std::cout<<string_vec[i]<<std::endl;
    }    
    std::map<std::string,int> orderbook_bids;

    //Insertion performance for simple <string,int> map

    auto startTime = std::chrono::high_resolution_clock::now();
    for(int i=0;i<test_size;i++)
    {
        orderbook_bids.insert(make_pair(string_vec[i],i+100));
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout<<"Time taken to insert "<<test_size<<" <string,int> pairs in map: "<<std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - startTime).count()<<" milliseconds"<<std::endl;

    AP::AuctionPrices House2;
    std::string base_item_ID = "item3";

    //Insertion performance for auctionprices with constant itemID and <hashed string,int> map

    startTime = std::chrono::high_resolution_clock::now();
    for(int i=0; i<test_size; i++)
    {
        std::string i_ID = base_item_ID;
        int insert_status = House2.addNewOrder_hashed(i_ID.c_str(), string_vec[i].c_str(), 1, i+100);
    }
    endTime = std::chrono::high_resolution_clock::now();
    std::cout<<"Time taken to insert "<<test_size<<" orders in auctionprices (hashed): "<<std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - startTime).count()<<" milliseconds"<<std::endl;

    //Insertion performance for auctionprices with constant itemID and <string,int> map

    // startTime = std::chrono::high_resolution_clock::now();
    // for(int i=0; i<test_size; i++)
    // {
    //     //std::string i_ID = base_item_ID;
    //     int insert_status = House2.addNewOrder(base_item_ID.c_str(), string_vec[i].c_str(), 1, i+100);
    // }
    // endTime = std::chrono::high_resolution_clock::now();
    // std::cout<<"Time taken to insert "<<test_size<<" orders in auctionprices (UNHASHED): "<<std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - startTime).count()<<" milliseconds"<<std::endl;

    std::cout<<"\n";

    //Deletion performance for simple map <string,int>

    startTime = std::chrono::high_resolution_clock::now();
    for(int i=test_size-1; i>=0; i--)
    {
        //std::string i_ID = base_item_ID;
        orderbook_bids.erase(string_vec[i]);
    }
    endTime = std::chrono::high_resolution_clock::now();
    std::cout<<"Time taken to delete "<<test_size<<" entries in simple <string,int> map (reverse order): "<<std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - startTime).count()<<" milliseconds"<<std::endl;

    //Deletion performance for <hashed string,int> map:

    startTime = std::chrono::high_resolution_clock::now();
    for(int i=test_size-1; i>=0; i--)
    {
        //std::string i_ID = base_item_ID;
        int delete_status = House2.deleteOrder_hashed(base_item_ID.c_str(), string_vec[i].c_str());
    }
    endTime = std::chrono::high_resolution_clock::now();
    std::cout<<"Time taken to delete "<<test_size<<" orders in auctionprices (hashed) (reverse order): "<<std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - startTime).count()<<" milliseconds"<<std::endl;

    //Deletion performance for <string,int> map:

    // startTime = std::chrono::high_resolution_clock::now();
    // for(int i=test_size-1; i>=0; i--)
    // {
    //     //std::string i_ID = base_item_ID;
    //     int delete_status = House2.deleteOrder(base_item_ID.c_str(), string_vec[i].c_str());
    // }
    // endTime = std::chrono::high_resolution_clock::now();
    // std::cout<<"Time taken to delete "<<test_size<<" orders in auctionprices (UNHASHED) (reverse order): "<<std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - startTime).count()<<" milliseconds"<<std::endl;

    //Deletion performance for <string,int> map top element first:

    // startTime = std::chrono::high_resolution_clock::now();
    // for(int i=0; i<test_size; i++)
    // {
    //     //std::string i_ID = base_item_ID;
    //     int delete_status = House2.deleteOrder(base_item_ID.c_str(), string_vec[i].c_str());
    // }
    // endTime = std::chrono::high_resolution_clock::now();
    // std::cout<<"Time taken to delete "<<test_size<<" orders in auctionprices (UNHASHED) (straight order): "<<std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - startTime).count()<<" milliseconds"<<std::endl;


    /*
    Conclusions:
    1. hashing before inserting string,int takes MUCH longer (30% longer with 100,000 insertions, 100% longer with 500,000 insertions, 160% longer with 1,000,000 insertions). This is because we are actually performing two inserts: one in hashed string vs string and the other in the regular orderbook. After checking the performance, unordered_map works better than map for the hasher_lookup data structure.

    2. the reverse is true for deletions: deleting from hashed map is MUCH faster (about 400% faster with 1,000,000 entries)
    */


    return 0;
}