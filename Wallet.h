#include <string>
#include <map>
#include "OrderBookEntry.h"

class Wallet {
    public:
        Wallet();
        /** insert currency into the wallet */
        void insertCurrency(std::string type, double amount);

        /** remove currency into the wallet */
        bool removeCurrency(std::string type, double amount);
        
        /** check if the wallet contains this much currency or more */
        bool containsCurrency(std::string type, double amount);

        /** checks isf the wallet can cope with this ask or bid*/
        bool canFulfilOrder(const OrderBookEntry& order);

        /** update the content of the wallet 
         * assumes the order was made by the owner of the waller
        */
        void processSale(OrderBookEntry& sale);
        
        /** generate string representation of the wallet */
        std::string toString();
    
    private:
        std::map<std::string,double> currencies;
};