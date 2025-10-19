#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <memory>

using namespace std;

class Person 
{

public:

	Person(const string& n, const string& phone, const string& email) // getter for protected strings 
		: name(n), phoneNumber(phone), emailAddress(email) // setter
	{
	}

	virtual string toString() const // concats Person's details and returns a sentence describing them.
	{
		return "A Person named " + name + " has phone number " + phoneNumber +
			" and can be reached at " + emailAddress + ". ";
	}

	virtual ~Person() = default; // virtual destructor

protected:
	string name;
	string phoneNumber;
	string emailAddress;
};

class Student : public Person // Student is an inherited class of Person 
{

public:
	Student(const string& n, const string& phone, const string& email, // Takes inherited Person info and adds getter for years in college
		const string& year)
		: Person(n, phone, email), yearInCollege(year) // setter
	{
	}

	string toString() const override 
	{
		return Person::toString() + "They are in their " + yearInCollege + " year."; // concats Person return with years in college and returns new sentence
	}

protected:

	string yearInCollege;
};

class Employee : public Person 
{

public:
	Employee(const string& n, const string& phone, const string& email, // Takes inherited Person info and adds getter for salary.
		double sal)
		: Person(n, phone, email), salary(sal) // setter
	{
	}

	string toString() const override // Added sstream to manipulate double value to string. Performs same as other concats otherwise.
	{
		ostringstream oss;
		oss << fixed << setprecision(2);
		oss << Person::toString() + "They earn " << salary << " annually. ";
		return oss.str();
	}

protected:

	double salary;
};

class Faculty : public Employee // Inherited class of Employee rather than Person
{
private:
	string title;

public:
	Faculty(const string& n, const string& phone, const string& email, //Takes inherited Employee info and adds getter for title
		double sal, const string& t)
		: Employee(n, phone, email, sal), title(t) // setter
	{
	}

	string toString() const override 
	{
		return Employee::toString() + "They are a/an " + title +". ";
	}
};

class Staff : public Employee 
{
private:
	string role;

public:
	Staff(const string& n, const string& phone, const string& email, //Takes inherited Employee info and adds getter for role
	
		double sal, const string& r)
		: Employee(n, phone, email, sal), role(r) // sstter
	{
	}

	string toString() const override 
	{
		return Employee::toString() + "They work in " + role + ". ";
	}
};

int main() 
{
	vector<Person*> people;

	// Creates instances of each class
	people.push_back(new Person("Susan", "800-555-1212", "susan@quirkymail.com"));
	people.push_back(new Student("Jerry", "123-456-7890", "jerjer@nat.edu", "sophomore"));
	people.push_back(new Employee("Larry", "333-444-5555", "larry@threestooges.org", 123456.0));
	people.push_back(new Faculty("Linda", "987-654-3210", "lindalou@whoknew.edu", 150000.0, "adjunct professor"));
	people.push_back(new Staff("Jane", "567-123-6565", "jane@whizkid.com", 100000.0, "registration"));

	// Iterates and calls toString()
	for (const auto& person : people) 
	{
		cout << person->toString() << endl;
	}

	// Cleans up memory
	for (auto person : people) 
	{
		delete person;
	}

	return 0;
}