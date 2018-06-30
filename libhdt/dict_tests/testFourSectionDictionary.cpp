#include <iostream>
#include <exception>
#include "TriplesFourSectionDictionary.hpp"
using namespace std;
using namespace hdt;
int main()
{
try
{
	Dictionary* fsdict = new TriplesFourSectionDictionary(); 
	cout << "fsdict->idToString(0, SUBJECT)=" << fsdict->idToString(0, SUBJECT) << endl;
	cout << "fsdict->idToString(0, PREDICATE)=" << fsdict->idToString(0, PREDICATE) << endl;
	cout << "fsdict->idToString(0, OBJECT)=" << fsdict->idToString(0, OBJECT) << endl;
	cout << "fsdict->idToString(0, GRAPH)=" << fsdict->idToString(0, GRAPH) << endl;
	cout << "fsdict->getNSubjects()=" << fsdict->getNsubjects()<<endl;
	cout << "fsdict->getNObjects()=" << fsdict->getNobjects()<<endl;
delete fsdict;
}
catch (exception& e)
{
	cout << e.what() << '\n';
}

}


