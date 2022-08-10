#include<iostream>
#include<vector>
#include"interface.h"

typedef std::vector<Client> ClientList;

struct StoreEvent {
	enum Type {
		WorkerSend, WorkerBack, ClientDepart
	};

	Type type;
	int minute;

    struct Worker {
			ResourceType resource;
		};

		struct Client {
			int index;
			int banana;
			int schweppes;
		}; 

	union {
		
        Worker worker;
        Client client;
	};
};


struct TestStore : ActionHandler {
	Store *impl = nullptr;
	std::vector<StoreEvent> log;

	TestStore()
		: impl(createStore()) {
		impl->setActionHandler(this);
	}

	~TestStore() {
		delete impl;
	}

	TestStore(const TestStore &) = delete;
	TestStore &operator=(const TestStore &) = delete;

	void init(int workerCount, int startBanana, int startSchweppes) {
		impl->init(workerCount, startBanana, startSchweppes);
	}

	void advanceTo(int minute) {
		impl->advanceTo(minute);
	}

	int getBanana() const {
		return impl->getBanana();
	}

	int getSchweppes() const {
		return impl->getSchweppes();
	}

	void addClients(const ClientList &clients) {
		impl->addClients(clients.data(), clients.size());
	}

	void addClients(const Client &single) {
		impl->addClients(&single, 1);
	}

	void onWorkerSend(int minute, ResourceType resource) override {
		StoreEvent ev;
		ev.type = StoreEvent::WorkerSend;
		ev.minute = minute;
		ev.worker.resource = resource;
		log.push_back(ev);
	}

	void onWorkerBack(int minute, ResourceType resource) override {
		StoreEvent ev;
		ev.type = StoreEvent::WorkerBack;
		ev.minute = minute;
		ev.worker.resource = resource;
		log.push_back(ev);
	}

	void onClientDepart(int index, int minute, int banana, int schweppes) override {
		StoreEvent ev;
		ev.type = StoreEvent::ClientDepart;
		ev.minute = minute;
		ev.client.banana = banana;
		ev.client.schweppes = schweppes;
		ev.client.index = index;
		log.push_back(ev);
	}
};

void input(TestStore& st)
{
    ClientList clients;

    int numberOfClients , numberOfSuppliers;

    std::cout<<"enter number of suppliers: ";

    std::cin>>numberOfSuppliers;

    st.init(numberOfSuppliers , 0 , 0);

    std::cout<<"enter number of clients: ";

    std::cin>>numberOfClients;
	
	if(numberOfClients <= 0)
	{
		return;
	}
    
    for(int i = 0 ; i < numberOfClients ; i++)
    {
        Client client;
        std::cin>>client.arriveMinute>>client.banana>>client.schweppes>>client.maxWaitTime;
        clients.push_back(client);
    }
    st.addClients(clients);

    int lastMin = 0;
	int size = clients.size();
	for(int i =0 ; i < size ; ++i)
	{
		if(clients[i].arriveMinute + clients[i].maxWaitTime > lastMin)
		{
			lastMin = clients[i].arriveMinute + clients[i].maxWaitTime;
		}
	}

    st.advanceTo(lastMin + 60 );

}

void output(TestStore& st)
{
    int size= st.log.size();

    for(int i =0 ; i < size; ++i)
    {
        if(st.log[i].type == 0 )
        {
            std::cout<<'W'<<" "<<st.log[i].minute<<" ";

            if(st.log[i].worker.resource == ResourceType::banana)
            {
                std::cout<<"banana"<<std::endl;
            }
            else
            {
                std::cout<<"schweppes"<<std::endl;
            }
        }
        else if(st.log[i].type == 1)
        {
            std::cout << 'D' << " " << st.log[i].minute << " ";

            if (st.log[i].worker.resource == ResourceType::banana)
            {
                std::cout << "banana" << std::endl;
            }
            else
            {
                std::cout << "schweppes" << std::endl;
            }
        }
        else
        {
            std::cout<<st.log[i].client.index<<" "<<st.log[i].minute<<" "<<st.log[i].client.banana<<" "<<st.log[i].client.schweppes<<std::endl;
        }
    }

}

int main()
{
	TestStore st;

	input(st);

	output(st);

	return 0;
}