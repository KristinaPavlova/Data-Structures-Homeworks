#ifndef IMPLEMENTATION_CPP
#define IMPLEMENTATION_CPP
#include "interface.h"
#include"LinkedList.hpp"
#include"heap.hpp"
#include"Queue.h"

struct Supplier
{
    int deliveryTime{};
    ResourceType deliveryProduct;
    Supplier(int time, ResourceType resourse):deliveryTime(time),deliveryProduct(resourse){}
};

struct Inventory
{
    int bananas{};
    int schweppes{};
};

struct MyClient : Client
{
    int clientNumber{}; // client number

	MyClient(int arriveMinutes_ , int banana_ , int schweppes_ , int maxWaitingTime_ , int clientNumber_ = 0)
	{
		arriveMinute = arriveMinutes_;
		banana = banana_;
		schweppes = schweppes_;
		maxWaitTime = maxWaitingTime_;
		clientNumber = clientNumber_;
	}

};

struct MyStore : Store 
{
    ActionHandler *actionHandler = nullptr;

    size_t freeSuppliers{0}; 

    LinkedList<MyClient> clientsList; //clients in the store
                                    // use list because wanna check all clients not only the first one

    Queue<Supplier> Sentsuppliers; // holds all sent suppliers added by deliveryTime -> the first supplier in the queue will deliver the soonest
                                        

    Inventory storeInventory; // bananas and schweppes in the store

    MinHeap<int> minutes; // holds all minutes we have to check for event 

    size_t suppliersSentForSchweppes{0}; // how many suppliers are sent for schweppes

    size_t suppliersSentForBanana{0}; // how many suppliers are sent for banana
     
    int Time{0};

    void updateInventory(ResourceType); // update store inventory when there is a delivery

    void order(int& , ResourceType); // order certain product

    void orderByPriority(int& , int&); // calls 'order' mehtod in right sequence

    bool haveInStock(LinkedList<MyClient>::Iterator&)const; // check if store has enough products for current client

    void checkForDelivery(); //processes all deliveries in current min

    void checkForClientArrival(); //processes all clients arrivals in current min

    void checkForClientLeave(); //processes all clients leavings in current min

    void setActionHandler(ActionHandler *handler) override 
    {
		actionHandler = handler;
	}

	void init(int workerCount, int startBanana, int startSchweppes) override ;

	void addClients(const Client *, int ) override ;

	void advanceTo(int minute) override ;

	virtual int getBanana() const 
    {
		return storeInventory.bananas;
	}

	virtual int getSchweppes() const 
    {
		return storeInventory.schweppes;
	}
};

void MyStore::init(int workerCount, int startBanana, int startSchweppes)
{
    freeSuppliers = workerCount;
    storeInventory.bananas = startBanana;
    storeInventory.schweppes = startSchweppes;
}

void MyStore::addClients(const Client *clients, int count)
{
    size_t clientID = 0; 
    while (count)
    {
        MyClient currentClient(clients->arriveMinute , clients->banana , clients->schweppes , clients->maxWaitTime , clientID);
        this->clientsList.push_back(currentClient);
        minutes.push_heap(currentClient.arriveMinute);
        minutes.push_heap(currentClient.arriveMinute + currentClient.maxWaitTime);
        clients++;
        count--;
        clientID++;
    }
}

void MyStore::updateInventory( ResourceType resoure) // update store inventory when there is delivery
{
    if(resoure == ResourceType::banana)
    {
        storeInventory.bananas += 100;
        suppliersSentForBanana --;
        return;
    }
    storeInventory.schweppes +=100;
    suppliersSentForSchweppes--;
}

void MyStore::order(int& amountNeeded , ResourceType resoure)
{
    if(resoure == ResourceType::banana) // order for bananas
    {
        if(amountNeeded <= (suppliersSentForBanana* 100)+ storeInventory.bananas) //check if there is need for sending new supplier to serve this client
        {
            return;
        }
        else
        {
            amountNeeded -= storeInventory.bananas + (suppliersSentForBanana* 100);
            while(amountNeeded > 0 && freeSuppliers > 0)
            {
                Sentsuppliers.push(Supplier{Time+60 , ResourceType::banana});
                suppliersSentForBanana++;
                freeSuppliers--;
                actionHandler->onWorkerSend(Time , ResourceType::banana);
                minutes.push_heap(Time+60); // include the minute in wich the suplier wiil deliver stock
                amountNeeded -= 100;
            }
            
        }
        return;
    }
    //order for schweppes
    if (amountNeeded <=(suppliersSentForSchweppes * 100)+storeInventory.schweppes) //check if there no need for sending new supplier to serve this client
    {
        return;
    }
    else
    {
        amountNeeded -= storeInventory.schweppes + (suppliersSentForSchweppes * 100);
        while (amountNeeded > 0 && freeSuppliers > 0)
        {
            Sentsuppliers.push(Supplier{Time + 60, ResourceType::schweppes});
            suppliersSentForSchweppes++;
            freeSuppliers--;
            actionHandler->onWorkerSend(Time, ResourceType::schweppes);
            minutes.push_heap(Time + 60); // include the minute in wich the suplier wiil deliver stock
            amountNeeded -= 100;
        }
      
    }
}

void MyStore::orderByPriority(int& needBanana , int& needSchweppes) 
{

    if (needBanana > 0 && needSchweppes > 0 && needBanana == needSchweppes)
    {
        order(needBanana, ResourceType::banana);
        order(needSchweppes, ResourceType::schweppes);
    }
    else if (needBanana > 0 && needBanana > needSchweppes)
    {
        order(needBanana, ResourceType::banana);
        if (needSchweppes > 0)
            order(needSchweppes, ResourceType::schweppes);
    }
    else if (needSchweppes > 0 && needSchweppes > needBanana)
    {
        order(needSchweppes, ResourceType::schweppes);
        if (needBanana > 0)
            order(needBanana, ResourceType::banana);
    }
}

bool MyStore::haveInStock(LinkedList<MyClient>::Iterator& client)const // check if store have what the client wants
{
    return (storeInventory.bananas >=  client->data.banana && storeInventory.schweppes >= client->data.schweppes);
}

void MyStore::checkForDelivery()
{
    if(Sentsuppliers.isEmpty()) // no suppliers -> no deliveries
    {
        return;
    }
    
    // check if thеre is a delevery and process it
    // no need to check all sent suppliers because they are added by delivery minute in the queue
    while (!Sentsuppliers.isEmpty() && Sentsuppliers.top().deliveryTime == Time) 
    {
        actionHandler->onWorkerBack(Time, Sentsuppliers.top().deliveryProduct); // delivery event
        updateInventory(Sentsuppliers.top().deliveryProduct);
        Sentsuppliers.pop(); // pop_front because sentSuppliers are ordered by deliveryTime
        freeSuppliers++;
    }

    // check waiting after delivery
    for(LinkedList<MyClient>::Iterator it = clientsList.begin() ; it != clientsList.end() ; ) 
    {
        // if there is enough in the inventory -> client leave
        if(it->data.arriveMinute < Time && haveInStock(it) ) 
        {
            actionHandler->onClientDepart(it->data.clientNumber, Time , it->data.banana , it->data.schweppes);
            storeInventory.bananas -= it->data.banana;
            storeInventory.schweppes -=it->data.schweppes;
            clientsList.deleteElemByPointer(it);
             // no need to increment "it" because when we delete by pointer "it" starts to point the next element
            
        }
        else // go to next client
        {
            ++it;
        }
    }
}

void MyStore::checkForClientArrival()
{
    if(clientsList.empty()) // no clients in the store
    {
        return;
    }
    for(LinkedList<MyClient>::Iterator it = clientsList.begin() ; it != clientsList.end() ;  ) 
    {
        if(it->data.arriveMinute == Time) 
        {
           if(haveInStock(it)) // if there is what the client wants  -> leaves the store
           {
               actionHandler->onClientDepart(it->data.clientNumber, Time, it->data.banana, it->data.schweppes);
               storeInventory.bananas -= it->data.banana;
               storeInventory.schweppes -= it->data.schweppes;
               clientsList.deleteElemByPointer(it); // delete client by pointer because he may not be the first one in the list
                // no need to increment "it" because when we delete by pointer "it" starts to point the next element
           }
           else
           {
               int needBananas = it->data.banana ;        // amount of bananas needed for this client's order
               int needSchweppes = it->data.schweppes ; // amount of schweppes needed for this client's order
               orderByPriority(needBananas , needSchweppes);
               ++it;
           }

        }
        else
        {
            ++it; 
        }    
    }
}

void MyStore::checkForClientLeave()
{
    if(clientsList.empty()) // no clients in the store
    {
        return;
    }

    for(LinkedList<MyClient>::Iterator it = clientsList.begin() ; it != clientsList.end() ;  ) 
    {
        if(it->data.arriveMinute + it->data.maxWaitTime == Time)
        {
            if(storeInventory.bananas <= it->data.banana) // takes as much bananas as he can
            {
                it->data.banana = storeInventory.bananas;
                storeInventory.bananas = 0;

            }
            else// takes as much bananas as he wants
            {
                storeInventory.bananas = storeInventory.bananas - it->data.banana; 

            }
            if(storeInventory.schweppes <= it->data.schweppes)// takes as much schweppes as he can
            {
                it->data.schweppes = storeInventory.schweppes;
                storeInventory.schweppes = 0;
            }
            else// takes as much schweppes as he wants
            {
                storeInventory.schweppes = storeInventory.schweppes  - it->data.schweppes;
            }
            
            actionHandler->onClientDepart(it->data.clientNumber , Time , it->data.banana , it->data.schweppes);
            clientsList.deleteElemByPointer(it); // remove client from the list by pointer because it may not be the fist in the list
           // no need to increment "it" because when we delete by pointer "it" starts to point the next element
        }
        else
        {
            ++it;
        }
    
    }
}

void MyStore::advanceTo(int minute )
{
    if(minute < 0)
    {
        throw std::invalid_argument("negative time");
    }

    if( minute == 0) 
    {
        checkForClientArrival();

        checkForDelivery();

        checkForClientLeave();

        while(!minutes.empty() && minutes.min() == 0)
        {
            minutes.pop_heap();
        }
    }

    while ( !minutes.empty() && minutes.min() <= minute )
    {

        if (Time == minutes.min()) // because we may have equal minutes in heap and in the biginning Time starts from 0 and heap.min() = 0
        {
            minutes.pop_heap();
        }
        else
        {
            Time = minutes.min(); // take closest time from the start
            minutes.pop_heap();
        }

        // three events in this moment for check

        checkForClientArrival();

        checkForDelivery();

        checkForClientLeave();
    }
}

Store *createStore() {
	return new MyStore();
}

#endif