#ifndef AUCTIONPRICES_H_
#define AUCTIONPRICES_H_

#include "flat_hash_map.hpp"
#include "unordered_map.hpp"
#include "bytell_hash_map.hpp"
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
            ska::flat_hash_map <std::string, int> bids;
            ska::flat_hash_map <std::string, int> offers;         
            
            bool empty();
        
        public:
            orderbook();

            int addNewOrder(const char* auction_ID, int side, int price);
            int deleteOrder(const char* auction_ID); 

            int print();
            

    };

    class AuctionPrices
    {
        private:
            ska::flat_hash_map <std::string, AP::orderbook> Library;
        
        public:
            int addNewOrder(const char* item_ID, const char* auction_ID, int side, int price);            
            int deleteOrder(const char* item_ID, const char* auction_ID);

            int print();
    }; 
}

#endif