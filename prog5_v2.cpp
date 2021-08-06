#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <time.h>
#include <semaphore.h>
using namespace std;

int battery;
int sensor;
int wifi;
int crawler;
int mutex;
int robot;
int dispatch;

void wait_battery(int s)
{

	while( s<= 0 || battery >= 1 || sensor == 0 || wifi == 0 || crawler == 0)
		if ( robot >=40 )
			break;
	;// busy waiting
	s--;
}

void wait_sensor(int s)
{
	while( s<= 0 || battery == 0 || sensor >= 1 || wifi == 0 || crawler == 0)
		if ( robot >=40 )
			break;
	;// busy waiting
	s--;
}

void wait_wifi(int s)
{
	while( s<= 0 || battery == 0 || sensor == 0 || wifi >= 1 || crawler == 0)
		if ( robot >=40 )
			break;
	;// busy waiting
	s--;
}

void wait_crawler(int s)
{
	while( s<= 0 || battery == 0 || sensor == 0 || wifi == 0 || crawler >= 1)
		if ( robot >=40 )
			break;
	;// busy waiting
	s--;
}

void wait(int s)
{
	while( s<= 0 || ((battery >= 1) + (sensor >= 1) + (wifi >= 1) + (crawler >= 1)) >= 3)
		if ( robot >=40 )
			break;
	;// busy waiting
	s--;
}

void signal(int s )
{
	s++;
}

void *dispatcher(void *data)
{
	do
	{
		wait(mutex);
		if ( robot >= 40 )
			break;
		dispatch = rand() % 4 + 1;
		if ( dispatch == 1 ) // battery
		{
			battery++;
			cout << "Dispatcher: battery" <<endl;
		}
		else if ( dispatch == 2 ) // sensor
		{
			sensor++;
			cout << "Dispatcher: sensor" << endl;
		}
		else if ( dispatch == 3 ) // wifi
		{
			wifi++;
			cout << "Dispatcher: wifi" << endl;
		}
		else if ( dispatch == 4 ) // crawler
		{
			crawler++;
			cout << "Dispatcher: crawler" << endl;
		}
		signal(mutex);
		
	}while(robot<40);

	return 0;
}

void *producer_battery(void *data)
{
	do
	{
		wait_battery(mutex);
		if ( robot >= 40 )
			break;
		// C.S.
		robot++;
		cout << "Producer(battery): OK, " << robot << " robot(s)" <<endl;
		
		sensor--;
		wifi--;
		crawler--;
		signal(mutex);
		
	}while(robot < 40);

	return 0;
}

void *producer_sensor(void *data)
{
	do
	{

		
		wait_sensor(mutex);
		if ( robot >= 40 )
			break;
		// C.S.
		robot++;
		cout << "Producer(sensor): OK, " << robot << " robot(s)" <<endl;
		battery--;
		
		wifi--;
		crawler--;
		signal(mutex);
		
	}while(robot < 40);
	
	return 0;
}

void *producer_wifi(void *data)
{
	do
	{
		
		
		wait_wifi(mutex);
		if ( robot >= 40 )
			break;
		// C.S.
		robot++;
		cout << "Producer(wifi): OK, " << robot << " robot(s)" <<endl;
		battery--;
		sensor--;
		
		crawler--;
		signal(mutex);
		
	}while(robot < 40);

	return 0;
}

void *producer_crawler(void *data)
{
	do
	{
		
		
		wait_crawler(mutex);
		if ( robot >= 40 )
			break;
		// C.S.
		robot++;
		cout << "Producer(crawler): OK, " << robot << " robot(s)" <<endl;
		battery--;
		sensor--;
		wifi--;
		
		signal(mutex);
		
	}while(robot < 40);

	return 0;
}


int main()
{
	battery=0;
	sensor=0;
	wifi=0;
	crawler=0;
	mutex = 1;
	robot=35;
	dispatch=0;
	//srand (time(NULL));
	pthread_t p1, p2, p3, p4, d;
	
	pthread_create( &d, NULL, dispatcher, NULL );	
	
	pthread_create( &p1, NULL, producer_battery, NULL );
	pthread_create( &p2, NULL, producer_sensor, NULL );	
	pthread_create( &p3, NULL, producer_wifi, NULL );	
	pthread_create( &p4, NULL, producer_crawler, NULL );
	pthread_join(d,NULL);	
	pthread_join(p1,NULL);	
	pthread_join(p2,NULL);	
	pthread_join(p3,NULL);	
	pthread_join(p4,NULL);	
	cout << endl;
	cout << "Remaining materials: " << endl
		<< "Batterys: " << battery << endl
		<< "Sensors: " << sensor << endl
		<< "Wifi: " << wifi << endl
		<< "Crawlers: " << crawler << endl;

}


