//SSM_2nd_Cpp Assignment

#include <iostream>
#include <list>

#define MAXCOUNT 100

using namespace std;


class Unit
{
public:
	Unit(){Health = 50; Direction = true;}
	int Health;
	int Position;
	int Damage;
	char name;
	bool Direction;

	virtual void PrintName(){};
	virtual void Move(void){};
	virtual void Attack(Unit*){};
};



class Marine : public Unit
{
public:
	Marine()
	{
		this->Position = rand()%100; 
		name = 'A';
		Damage = -10;
	}
	void Move()
	{
		if(this->Position<0 || this->Position>100) this->Direction=!this->Direction;

		if(this->Direction==true)
			this->Position+=2;
		else
			this->Position-=2;
	}
	void Attack(Unit* unit)
	{
		if(unit->name == 'S') unit->Health += Damage;	
	}
};

/*Medic moves -=1 , health += 3*/
class Medic : public Unit
{
public:
	Medic()
	{
		this->Position = rand()%100;
		name = 'M';
		Damage = 3;
	}
	void Move()
	{ 
		if(this->Position<0 || this->Position>100) this->Direction=!this->Direction;

		if(this->Direction==true)
			this->Position+=1;
		else 
			this->Position-=1;
	}
	void Attack(Unit *unit)
	{
		if(unit->name=='A') {unit->Health += Damage; }
		
	}
};

/**/
//Medic or Marine -> health-=5
class Soldier : public Unit
{
public:
	Soldier()
	{
		this->Position = rand()%100;
		name = 'S';
		Damage = -5;
	}
	void Move()
	{
		if(this->Position<0 || this->Position>100) this->Direction=!this->Direction;

		if(this->Direction==true)
			this->Position+=1;
		else 
			this->Position-=1;
	}
	void Attack(Unit* unit)
	{
		//cout<<unit->name<<endl;
		if(unit->name=='M' || unit->name=='A') unit->Health += Damage;
	}
};


int main()
{
	Unit *OBJECTS[MAXCOUNT];

	for(int i=0; i<MAXCOUNT; i++)
	{
		switch(rand()%3)
		{
		case 0: OBJECTS[i] = new Soldier(); break;
		case 1: OBJECTS[i] = new Medic(); break;
		case 2: OBJECTS[i] = new Marine(); break;
		default: cout<< "Exception Error" << endl; break;
		}
	}

	while(1)
	{
		for(int k=0; k<100; k++)
		{
			for(int i=0; i<MAXCOUNT; i++)
			{
				//OBJECTS[i]->PrintName();
				if(OBJECTS[i]->Health > 0)
				{
					OBJECTS[i]->Move();
					for(int j = 0; j<MAXCOUNT; j++)
					{
						if(i==j) continue;
						if(OBJECTS[i]->Position == OBJECTS[j]->Position)
						{
							OBJECTS[i]->Attack(OBJECTS[j]);
						}
					}
				}
			}
		}

		for(int m=0; m<MAXCOUNT; m++)
		{
				cout<<m<<"번째"<<"]]";
				if(OBJECTS[m]->Health>0)
				{
					cout<<"체력:"<<OBJECTS[m]->Health<<" ";
					cout<<"위치:"<<OBJECTS[m]->Position<<endl;
				}
				else
					cout<<"사망"<<endl;
				cout<<"======================="<<endl;
		
		}
		getchar();
	}
	return 0;
}