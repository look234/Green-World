class lootList
{
	private:
	struct lootListStruct
	{
		char *name;
		lootListStruct *next;
	} *start;

	//lootListStruct* start;

public:

	int lootAmount;
	int maxLootAmount;

	lootList();
	~lootList();

	void add(char *name);
	//void next();
	//void remove(lootListStruct);
	//char returnList();
	void print();
};