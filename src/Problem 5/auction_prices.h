#ifndef AUCTIONPRICES_H_
#define AUCTIONPRICES_H_

#include "flat_hash_map.hpp"
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <queue>
#include <algorithm>

namespace AP
{
    class orderbook
    {
        private:
            std::hash<std::string> hasher;
            std::unordered_map<size_t, std::string> hasher_lookup;
            //std::map<size_t, std::string> hasher_lookup;
            //std::string item_ID;
            std::map<size_t, int> bids_hashed;
            std::map<size_t, int> offers_hashed;
            //std::map<std::string,int> bids;
            //std::map<std::string,int> offers;
            ska::flat_hash_map <std::string, int> bids;
            ska::flat_hash_map <std::string, int> offers;
            
            

            // bool inline comparator_lesser(std::pair<int, size_t> p1, std::pair<int, size_t> p2);
            // bool inline comparator_greater(std::pair<int, size_t> p1, std::pair<int, size_t> p2);
            bool empty();
        
        public:
            // orderbook(const char* ID);
            orderbook();
            int addNewOrder_hashed(const char* auction_ID, int side, int price);
            int inline addNewOrder(const char* auction_ID, int side, int price);
            int deleteOrder_hashed(const char* auction_ID);
            int inline deleteOrder(const char* auction_ID);
            int print_hashed();

    };

    class AuctionPrices
    {
        private:
            std::hash<std::string> hasher;
            std::unordered_map<size_t, std::string> hasher_lookup;
            //std::map<size_t, std::string> hasher_lookup;
            std::map<size_t, AP::orderbook> Library;
        
        public:
            int addNewOrder_hashed(const char* item_ID, const char* auction_ID, int side, int price);
            int addNewOrder(const char* item_ID, const char* auction_ID, int side, int price);
            int deleteOrder_hashed(const char* item_ID, const char* auction_ID);
            int deleteOrder(const char* item_ID, const char* auction_ID);
            int print_hashed();
    }; 

    void init();
}

#endif