A simple orderbook with 3 basic functions is implemented. The functions are as follows:

1. addNewOrder(...): Adds an order with an item_ID, auction_ID, price and side to the orderbook of the correct item.
2. deleteOrder(...): Deletes an order using unique item_ID and auction_ID to identify the order.
3. print(): prints the orderbook (bids and offers) in the proper sorted order.


Features of the orderbook with reasoning:
1. The base data structure used is Abseil's flat_hash_map (https://github.com/skarupke/flat_hash_map/blob/master/flat_hash_map.hpp). After testing insertion and deletion times with several different STL and open-source containers as well as self-defined data structures, it was concluded that Abeil's flat_hash_map<string,int> outperforms everything in case of insertions, and is marginally worse for deletions against a <hashed string,int> map. It even outperforms a simple std::map<int,int> if compile-time optimisations using the -O3 flag is absent.
2. The decision to use a hash map is based on the cost of the 3 given functions. First, addNewOrder() and deleteOrder() are expected to be called much more frequently than print(), and deleteOrder() requires a lookup based on a string. Hence, it makes sense to sort orders during addNewOrder() by their auction_ID for faster searches and deletions. If the orders are sorted by price, lookups for auction_ID will take O(N) on average, and calling a deleteOrder() will be expensive. However, this might pose a problem if we need the top of the bids or offers. Currently, we don't.
3. The decision to use Abseil's flat_hash_map is based on the fact that we get ordered entries stored in a contiguous memory location, making insertion, search and traversal less expensive than an std::unordered_map as well as an std::map.
4. The decision to use a data structure tht has efficient hashing is motivated by the fact that comparing strings takes longer than comparing integers. However, std::hasher combined with an std::map is extremely inefficient, and performance of std::unordered_map decreases rapidly as the number of entries grows. It might be noted here that searches in an std::unordered_map that stores <hashed string,int> take significantly less time than every other data structure tested so far. However, this is not enough to make up for the insertion costs.
5. Since print() is expected to be called infrequently, bids and offers are currently sorted by price ONLY when print() is called. As mentioned earlier, this might make it expensive to return the best bid/best offer, but this is out of the scope of the question.


Possible improvements:
1. It can be noted that the strings item_ID and auction_ID are not random. If the possible values of these strings are known, the hashes could be computed at compile-time, thus reducing the runtime.
2. In an auction house, prices are going to be in multiples of some base price. For example, in the recent IPL (Indian Premiere League) auctions, the prices were in multiples of INR 500,000, the highest being INR 247,500,000. We can note that in this auction, there were a maximum of 495 different price levels. This opens up the possibility of using a simple data structure with contiguous memory (like an array), where array indexes are equal to (order price)/(base price). This can be done if this data is available before the auction, even if different items have different base prices.
3. Currently, the orderbook is optimised to handle large volumes of orders (in the range of 1 million orders). If the approximate number of orders can be calculated before the start of the auction, it could be further optimised to handle smaller number of orders. For example, in the IPL auction, an small orderbook with 500 to 1000 entires would have been sufficient.


In conclusion, any computation that can be performed at compile time, from allocating fixed memory based on expected number of orders to calculating hashes for strings based on expected strings is going to improve the performance of the code. It must also be mentioned here that compile-time optimisation by g++ using the -O3 flag reduces runtime in the sample testcases by 200-300%.

Compiled on Windows 10 on a Ryzen5 2600, 3.40 GHz processor with g++ 12.2.0 as follows:
g++ testcases.cpp auction_prices.cpp -O3 -o tests
