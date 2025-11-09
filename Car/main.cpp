#include <iostream>
#include <conio.h>
#include <windows.h> //дл€ sleep

using std::cin;
using std::cout;
using std::endl;

#define Enter  13
#define Escape 27

#define MIN_TANK_VOLUME 25
#define MAX_TANK_VOLUME 125

class Tank
{
	const int VOLUME;
	double fuel_level;
public:
	int get_VOLUME()const
	{
		return VOLUME;
	}
	double get_fuel_level()const
	{
		return fuel_level;
	}
	void fill(double fuel)
	{
		if (fuel < 0) return;
		fuel_level += fuel;
		if (fuel_level > VOLUME)fuel_level = VOLUME;
	}
	Tank(int volume) :
		VOLUME
		(
			volume < MIN_TANK_VOLUME ? MIN_TANK_VOLUME :
			volume > MAX_TANK_VOLUME ? MAX_TANK_VOLUME :
			volume
		)
	{
		this -> fuel_level = 0;
		cout << "Tank is ready:\t" << this << endl;
	}
	~Tank()
	{
		cout << "Tank is over:\t" << this << endl;
	}
	void info()const
	{
		cout << "Volume:\t" << VOLUME << " liters\n";
		cout << "Fuel:\t" << get_fuel_level() << " liters\n";
	}
};

#define MIN_ENGINE_CONSUMPTION 5
#define MAX_ENGINE_CONSUMPTION 25
class Engine
{
	double consumption_per_second;
	bool is_started;
public:
	const double CONSUMPTION;
	double get_consumption_per_second()const
	{
		return consumption_per_second;
	}
	Engine(double consumption) :
		CONSUMPTION
		(
			consumption < MIN_ENGINE_CONSUMPTION ? MIN_ENGINE_CONSUMPTION :
			consumption > MAX_ENGINE_CONSUMPTION ? MAX_ENGINE_CONSUMPTION :
			consumption
		)
	{
		consumption_per_second = CONSUMPTION * 3e-5;
		is_started = false;
		cout << "Engine is ready:\t" << this << endl;
	}
	~Engine()
	{
		cout << "Engine is over:\t" << this << endl;
	}
	void start()
	{
		is_started = true;
	}
	void stop()
	{
		is_started = false;
	}
	bool started()const
	{
		return is_started;
	}
	void info()const
	{
		cout << "Consumption per 100 km:\t" << CONSUMPTION << " liters.\n";
		cout << "Consumption per 1 sec:\t" << get_consumption_per_second() << " liters.\n";
	}
};

#define MAX_SPEED_LOWER_LIMIT  40
#define MAX_SPEED_UPPER_LIMIT 400

class Car
{
	Engine engine;
	Tank tank;
	const int MAX_SPEED;
	int speed;
	bool driver_inside;

public:
	Car(double consumption, int volume, int max_speed) :
		engine(consumption),
		tank(volume),
		speed(0),
		MAX_SPEED
		(
			max_speed < MAX_SPEED_LOWER_LIMIT ? MAX_SPEED_LOWER_LIMIT :
			max_speed < MAX_SPEED_UPPER_LIMIT ? MAX_SPEED_UPPER_LIMIT :
			max_speed
		)
	{
		driver_inside = false;
		cout << "Your car is ready:-)\t" << this << endl;
	}
	~Car()
	{
		cout << "Your car is over:-(\t" << this << endl;
	}

	void get_in()
	{
		driver_inside = true;
		panel();
	}

	void get_out()
	{
		driver_inside = false;
		system("CLS");
		cout << "You are out of the car" << endl;
	}

	void start_engine()
	{
		if (tank.get_fuel_level() > 0)
		{
			engine.start();
			cout << "Engine started!" << endl;
		}
		else
		{
			cout << "Can't start engine: no fuel!" << endl;
		}
	}

	void stop_engine()
	{
		engine.stop();
		cout << "Engine stopped!" << endl;
	}

	void drive()
	{
		if (!engine.started())
		{
			cout << "Start the engine first!" << endl;
			return;
		}

		if (tank.get_fuel_level() <= 0)
		{
			cout << "No fuel! Can't drive!" << endl;
			engine.stop();
			return;
		}

		cout << "Enter speed (0-" << MAX_SPEED << " km/h): ";
		int new_speed;
		cin >> new_speed;

		if (new_speed < 0) new_speed = 0;
		if (new_speed > MAX_SPEED) new_speed = MAX_SPEED;

		speed = new_speed;

		if (speed > 0)
		{
			cout << "Driving at " << speed << " km/h" << endl;

			double consumption_multiplier = 1.0 + (speed / 100.0);
			double fuel_consumed = engine.get_consumption_per_second() * consumption_multiplier;

			if (fuel_consumed > tank.get_fuel_level())
			{
				fuel_consumed = tank.get_fuel_level();
				cout << "Low fuel! Slowing down..." << endl;
				speed = 0;
			}

			cout << "Fuel consumed: " << fuel_consumed << " liters" << endl;
		}
		else
		{
			cout << "Car stopped" << endl;
		}
	}

	void control()
	{
		cout << "Press 'Enter' to get in/out" << endl;
		char key;
		do
		{
			key = _getch();
			switch (key)
			{
			case Enter:
				if (driver_inside)
				{
					get_out();
					
					cout << "\nControls outside car:" << endl;
					cout << "I - Info" << endl;
					cout << "F - Fill tank" << endl;
					cout << "E - Enter car" << endl;
					cout << "ESC - Exit program" << endl;
				}
				else
				{
					get_in();
				}
				break;
			case 'F':
			case 'f':
				if (!driver_inside)
				{
					double amount;
					cout << "How much fuel do you want? ";
					cin >> amount;
					tank.fill(amount);
					cout << "Tank filled. Current fuel level: " << tank.get_fuel_level() << " liters" << endl;
				}
				break;
			case 'I':
			case 'i':
				if (!driver_inside)
				{
					info();
				}
				break;
			case 'S':
			case 's':
				if (driver_inside)
				{
					if (engine.started())
					{
						stop_engine();
					}
					else
					{
						start_engine();
					}
				}
				break;
			case 'D':
			case 'd':
				if (driver_inside)
				{
					drive();
				}
				break;
			}

			if (driver_inside && engine.started() && tank.get_fuel_level() > 0)
			{
				
				double idle_consumption = engine.get_consumption_per_second() * 0.3;
				if (idle_consumption < tank.get_fuel_level())
				{
					//здесь должен быть учет времени
					cout << "Engine running... consuming fuel" << endl;
				}
				else
				{
					cout << "Out of fuel! Engine stopped!" << endl;
					engine.stop();
				}
			}

		} while (key != Escape);
	}

	void panel()const
	{
		while (driver_inside)
		{
			system("CLS");
			cout << "=== CAR PANEL ===" << endl;
			cout << "Fuel level:\t" << tank.get_fuel_level() << " liters\n";
			cout << "Engine is " << (engine.started() ? "STARTED" : "STOPPED") << endl;
			cout << "Speed:\t" << speed << " km/h\n";
			cout << "Max speed:\t" << MAX_SPEED << " km/h\n";
			cout << "\nControls:" << endl;
			cout << "S - Start/Stop engine" << endl;
			cout << "D - Drive" << endl;
			cout << "F - Fill tank" << endl;
			cout << "Enter - Get out" << endl;
			cout << "ESC - Exit program" << endl;

			
			if (_kbhit()) break;
			Sleep(100);  
		}
	}

	void info()const
	{
		engine.info();
		tank.info();
		cout << "Max speed:\t" << MAX_SPEED << " km/h\n";
		cout << "Current speed:\t" << speed << " km/h\n";
	}
};

//#define TANK_CHECK
//#define ENGINE_CHECK

void main()
{
	setlocale(LC_ALL, "");

#ifdef TANK_CHECK
	Tank tank(-30);
	tank.info();
	double fuel;
	do
	{
		cout << "¬ведите объем топлива: ";
		cin >> fuel;
		tank.fill(fuel);
		tank.info();
	} while (fuel > 0);
#endif // TANK_CHECK

#ifdef ENGINE_CHECK
	Engine engine(10);
	engine.info();
#endif // ENGINE_CHECK

	Car bmw(10, 80, 270);
	bmw.info();
	bmw.control();
}
