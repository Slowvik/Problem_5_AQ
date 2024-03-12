/*
> The underlying data structure is abseil's flat_hash_map. After running tests with various open-source hash maps, default STL containers and my own hashing functions, it was concluded that abseil's flat_hash_map performs the best with respect to insertions, and marginally worse in case of deletions. Overall, abseil's flat_hash_map is extremely fast and is one of the best candidates for an orderbook data structure.
*/
#include "auction_prices.h"
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <queue>
#include <algorithm>

//flat_hash_map AuctionPrices functions:

int AP::AuctionPrices::addNewOrder(const char* item_ID, const char* auction_ID, int side, int price)
{
    return Library[(item_ID)].addNewOrder(auction_ID, side, price);
}

int AP::AuctionPrices::deleteOrder(const char* item_ID, const char* auction_ID)
{
    return Library[(item_ID)].deleteOrder(auction_ID);
}

int AP::AuctionPrices::print()
{
    int print_status = 1;
    for(auto& i: Library)
    {
        std::cout<<i.first<<":\n";
        print_status = i.second.print();
        if(print_status == 0)
        {
            return 0;
        }
        //std::cout<<"\n";
    }
    return 1;
}


AP::orderbook::orderbook()
{

}

// flat_hash_map orderbook functions:

int inline AP::orderbook::addNewOrder(const char* auction_ID, int side, int price)
{
    if(side == 1)
    {
        bids.insert(std::make_pair((auction_ID), price));
        return 1;
    }
    else if(side == 2)
    {
        offers.insert(std::make_pair((auction_ID), price));
        return 1;
    }

    return 0;
}

int inline AP::orderbook::deleteOrder(const char* auction_ID)
{
    if(bids.empty() && offers.empty())
    {
        //some message
        return 0;
    }
    
    if(bids.find(auction_ID)!=bids.end())
    {
        bids.erase(auction_ID);
        return 1;
    }
    else if(offers.find(auction_ID)!=offers.end())
    {
        offers.erase(auction_ID);
        return 1;
    }
    return 0;
}

int AP::orderbook::print()
{
    if(bids.empty() && offers.empty())
    {
        std::cout<<"Orderbook for this item is empty\n";
        return 1;
    }

    std::vector<std::pair<int,std::string>> price_ordered_bids;
    std::vector<std::pair<int,std::string>> price_ordered_offers;

    for(auto& p: bids)
    {
        price_ordered_bids.push_back(std::make_pair(p.second, p.first));
    }
    for(auto& p: offers)
    {
        price_ordered_offers.push_back(std::make_pair(p.second, p.first));
    }
    std::sort(price_ordered_bids.begin(),price_ordered_bids.end(), 
                [](std::pair<int,std::string> p1, std::pair<int,std::string> p2)
                {
                    return (p1.second>p2.second);
                });
    std::sort(price_ordered_offers.begin(),price_ordered_offers.end(), 
                [](std::pair<int,std::string> p1, std::pair<int,std::string> p2)
                {
                    return (p1.second<p2.second);
                });
    
    
    std::cout<<"Buy:\n";
    for(auto&i :price_ordered_bids)
    {
        std::cout<<i.second<<" "<<i.first<<"\n";
    }
    std::cout<<"Sell:\n";
    for(auto& i: price_ordered_offers)
    {
        std::cout<<i.second<<" "<<i.first<<"\n";
    }

    return 1;
}
