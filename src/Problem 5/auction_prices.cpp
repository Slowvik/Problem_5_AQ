/*
> Basic Idea:
    > Orders are uniquely identified by strings. 
    > assuming there's only one order at every price level
    > It's an auction house, so prices are going to be in multiples of something. In the given example, they look like integers around ~100.
    > This might be useful if we want to use contiguous memory to store bids and asks. We need to check how they perform.

> Data structures:
    > keep track of what functionality is needed in what container. 
    > auction prices class as a whole for example needs a print() method, a lookup based on itemID(string), but no sorting of any kind
    > an individual orderbook::ask or bid object needs to be sorted based on price(int) and needs a lookup based on auctionID (string)
    > lookups are going to be expensive based on:
        > what the memory allocation of the container is (contiguous memory lookup is faster than random memory lookup)
        > what type we are looking up (integers for example take less time than strings because a comparison is involved)
        > whether the container is sorted and what kind if search we can employ (binary, std::find, etc)
        > so ideally for fastest lookups, we want contiguous memory, filled with integers that need to be compared and that are sorted.
        > maps for example are not contiguous but are sorted
        > priority_queue implemented with a vector will be contiguous and sorted
        > a simple vector + an std::sort will be contiguous sorted as well (have to check if priority_queue jumps around within the vector to find the next element or if it is implemented with a sorted vector as well)
        > Finally, wherever possible, it might be beneficial to hash strings and possibly store the hashed string vs the original string in another priority_queue implemented with a vector. Have to check if the double lookup is still faster than string lookup.
    > printing is always going to take linear time. The differences between two containers when it comes to printing is in whether:
        > the memory locations are contiguous or random
        > Hence, for containers that need print() functionality, it might be slightly beneficial to use contiguous memory
        > Keep in mind that even auctionPrices will require a lookup based on itemID whenever an order comes in. Might make sense to use a sorted contiguous container (like vector+sort/priority_queue)

> Compile-time optimisations:
    > Look into a way to template the addNewOrder function so that bids and asks can be resolved using templates rather than an if-else block


> Idea: store a hash vs string in a map. No need to delete anything ever, just use it to look up the string corresponding to the hash.
*/
#include "auction_prices.h"
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <queue>
#include <algorithm>

int AP::AuctionPrices::addNewOrder_hashed(const char* item_ID, const char* auction_ID, int side, int price)
{
    return Library[item_ID].addNewOrder_hashed(auction_ID, side, price);
}

// int AP::AuctionPrices::addNewOrder(const char *item_ID, const char *auction_ID, int side, int price)
// {
//     return Library[item_ID].addNewOrder(auction_ID, side, price);
// }

int AP::AuctionPrices::deleteOrder_hashed(const char* item_ID, const char* auction_ID)
{
    return Library[item_ID].deleteOrder_hashed(auction_ID);
}

// int AP::AuctionPrices::deleteOrder(const char* item_ID, const char* auction_ID)
// {
//     return Library[item_ID].deleteOrder(auction_ID);
// }

int AP::AuctionPrices::print_hashed()
{
    int print_status = 1;
    for(auto& i: Library)
    {
        std::cout<<i.first<<":\n";
        print_status = i.second.print_hashed();
        if(print_status == 0)
        {
            return 0;
        }
        std::cout<<"\n";
    }
    return 1;
}
    
bool AP::orderbook::empty()
{
    if(bids_hashed.empty() && offers_hashed.empty())
    {
        return true;
    }
    return false;
}

AP::orderbook::orderbook()
{

}

// AP::orderbook::orderbook(const char* ID)
// {
//     this->item_ID = ID;
// }

int inline AP::orderbook::addNewOrder_hashed(const char* auction_ID, int side, int price)
{
    hasher_lookup[hasher(auction_ID)] = auction_ID;
    if(side == 1)
    {
        bids_hashed.insert(std::make_pair(hasher(auction_ID), price));
        return 1;
    }
    else if(side == 2)
    {
        offers_hashed.insert(std::make_pair(hasher(auction_ID), price));
        return 1;
    }

    return 0;
}

// inline int AP::orderbook::addNewOrder(const char *auction_ID, int side, int price)
// {
//     if(side == 1)
//     {
//         bids.insert(std::make_pair((auction_ID), price));
//         return 1;
//     }
//     else if(side == 2)
//     {
//         offers.insert(std::make_pair((auction_ID), price));
//         return 1;
//     }
//     return 0;
// }

// inline int AP::orderbook::deleteOrder(const char *auction_ID)
// {
//     if(bids.empty() && offers.empty())
//     {
//         //some message
//         return 0;
//     }
//     if(bids.find(auction_ID)!=bids.end())
//     {
//         bids.erase(auction_ID);
//         return 1;
//     }
//     else if(offers.find(auction_ID)!=offers.end())
//     {
//         offers.erase(auction_ID);
//         return 1;
//     }
//     return 0;
// }

int inline AP::orderbook::deleteOrder_hashed(const char* auction_ID)
{
    if(bids_hashed.empty() && offers_hashed.empty())
    {
        //some message
        return 0;
    }
    int hashed_ID = hasher(auction_ID);
    if(bids_hashed.find(hashed_ID)!=bids_hashed.end())
    {
        bids_hashed.erase(hashed_ID);
        return 1;
    }
    else if(offers_hashed.find(hashed_ID)!=offers_hashed.end())
    {
        offers_hashed.erase(hashed_ID);
        return 1;
    }
    return 0;
}

int AP::orderbook::print_hashed()
{
    if(bids_hashed.empty() && offers_hashed.empty())
    {
        std::cout<<"Orderbook for this item is empty\n";
        return 1;
    }
    std::vector<std::pair<int,size_t>> price_ordered_bids;
    std::vector<std::pair<int,size_t>> price_ordered_offers;

    for(auto& p: bids_hashed)
    {
        price_ordered_bids.push_back(std::make_pair(p.second, p.first));
    }
    for(auto& p: offers_hashed)
    {
        price_ordered_offers.push_back(std::make_pair(p.second, p.first));
    }
    std::sort(price_ordered_bids.begin(),price_ordered_bids.end(), 
                [](std::pair<int,size_t> p1, std::pair<int,size_t> p2)
                {
                    return (p1.second>p2.second);
                });
    std::sort(price_ordered_offers.begin(),price_ordered_offers.end(), 
                [](std::pair<int,size_t> p1, std::pair<int,size_t> p2)
                {
                    return (p1.second<p2.second);
                });
    
    
    std::cout<<"Buy:\n";
    for(auto& i: price_ordered_bids)
    {
        std::cout<<hasher_lookup[i.second]<<" "<<i.first<<"\n";
    }
    std::cout<<"Sell:\n";
    for(auto& i: price_ordered_offers)
    {
        std::cout<<hasher_lookup[i.second]<<" "<<i.first<<"\n";
    }

    return 1;
}
