#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream> 
#include <iomanip> 
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

#define not_found (-1)

int compare(const char * a, const char * b) {
    char end = '\0';
    for (size_t i = 0; true; i++) {
        char n1 = a[i], n2 = b[i];
        int cmp;

        if (n1 == end && n2 == end) return 0;
        cmp = n1 - n2;
        if (cmp != 0) return cmp;
    }
}

void test_compare() {
    assert( !compare("a", "a") );
    assert( compare("a", "b") == -1 );
    assert( compare("aaaaab", "aaaaac") == -1 );
    assert( compare("", "1") < 0 );

    cout << "success" << endl;
}


class CPersonalAgenda
{
  public:
    void test_run() const;

    struct Person {
        Person(const string & name, const string & surname, const string & email, unsigned int salary)
        : name(name), surname(surname), email(email), salary(salary) {}

        // todo remove
        friend ostream & operator<<(ostream & os, const Person & person) {
            return os << "name: " << person.name
               << "   surname: " << person.surname
               << "  email: " << person.email
               << "   salary: " << person.salary;
        }
        
        string name, surname, email;
        unsigned int salary;
    };
    
                  CPersonalAgenda  () = default;
                  ~CPersonalAgenda ();
    bool          Add              ( const string    & name,
                                     const string    & surname,
                                     const string    & email,
                                     unsigned int      salary );
    bool          Del              ( const string    & name,
                                     const string    & surname );
    bool          Del              ( const string    & email );
    bool          ChangeName       ( const string    & email,
                                     const string    & newName,
                                     const string    & newSurname );
    bool          ChangeEmail      ( const string    & name,
                                     const string    & surname,
                                     const string    & newEmail );
    bool          SetSalary        ( const string    & name,
                                     const string    & surname,
                                     unsigned int      salary );
    bool          SetSalary        ( const string    & email,
                                     unsigned int      salary );
    unsigned int  GetSalary        ( const string    & name,
                                     const string    & surname ) const;
    unsigned int  GetSalary        ( const string    & email ) const;
    bool          GetRank          ( const string    & name,
                                     const string    & surname,
                                     int             & rankMin,
                                     int             & rankMax ) const;
    bool          GetRank          ( const string    & email,
                                     int             & rankMin,
                                     int             & rankMax ) const;
    bool          GetFirst         ( string          & outName,
                                     string          & outSurname ) const;
    bool          GetNext          ( const string    & name,
                                     const string    & surname,
                                     string          & outName,
                                     string          & outSurname ) const;
  // todo private:
    vector<Person*> orderedByName;
    vector<Person*> orderedByEmail;
    
    void sort();
    static int compareByName(const Person * a, const Person * b);
    static int compareByEmail(const Person * a, const Person * b);
    int getPosition(const string & email, size_t bottom, size_t top) const;
    int getPosition(const string & name, const string & surname, size_t bottom, size_t top) const;
    int search(const std::vector<CPersonalAgenda::Person*> & source, const CPersonalAgenda::Person & target, size_t bottomIndex, size_t topIndex, int (* compareFunction) (const Person*, const Person*)) const;

    // tests
    void test_compareByName() const;
    void test_compareByEmail() const;
    void test_getPosition_name() const;
    void test_getPosition_email() const;
    void test_search() const;
    void test_sort() const;
};

// private ------------------------------------------------------------------------------------

int CPersonalAgenda::compareByName(const Person * a, const Person * b) {
    int cmp = strcmp(a->name.c_str(), b->name.c_str());
    return cmp ? cmp : strcmp(a->surname.c_str(), b->surname.c_str());
}

int CPersonalAgenda::compareByEmail(const Person * a, const Person * b) {
    return strcmp(a->email.c_str(), b->email.c_str());
}

int CPersonalAgenda::search(
        const std::vector<CPersonalAgenda::Person*> & source,
        const CPersonalAgenda::Person & target,
        size_t bottomIndex,
        size_t topIndex,
        int (* compareFunction)(const CPersonalAgenda::Person*, const CPersonalAgenda::Person*)) const {
    Person * person;
    int cmp;
    size_t middle = (topIndex + bottomIndex) / 2;

    if (bottomIndex >= topIndex) return not_found;
    person = source[middle];

    cmp = compareFunction(person, &target);
    return !cmp ? int(middle) : (cmp > 0 ? search(source, target, middle + 1, topIndex, compareFunction) : search(source, target, bottomIndex, middle - 1, compareFunction));
}

void CPersonalAgenda::sort() {
    std::sort(orderedByName.begin(), orderedByName.end(), compareByName);
    std::sort(orderedByEmail.begin(), orderedByEmail.end(), compareByEmail);
}

int CPersonalAgenda::getPosition(const string & email, size_t bottom, size_t top) const {
    return search(orderedByEmail, Person("", "", email, 0), bottom, top, compareByEmail);
}

int CPersonalAgenda::getPosition(const string & name, const string & surname, size_t bottom, size_t top) const {
    return search(orderedByName, Person(name, surname, "", 0), bottom, top, compareByName);
}

// public -------------------------------------------------------------------------------------

CPersonalAgenda::~CPersonalAgenda() {
    for (auto person : orderedByName)
        delete person;
    this->orderedByName.clear();
    this->orderedByEmail.clear();
}

bool CPersonalAgenda::Add(const string & name,const string & surname, const string & email, unsigned int salary) {
    auto person = new Person(name, surname, email, salary);
    
    // verify that person is not already inserted
    if (binary_search(orderedByEmail.begin(), orderedByEmail.end(), person, compareByEmail))
        return false;
    
    // actually add new person
    orderedByEmail.push_back(person);
    orderedByName.push_back(person);
    sort();
    
    return true;
}

bool CPersonalAgenda::Del(const string & name, const string & surname) {
    Person * person;
    int index = getPosition(name, surname, 0, orderedByName.size() - 1);
    
    if (index == not_found) return false;
    
    // erase person in name list
    person = orderedByName[index];
    orderedByName.erase(orderedByName.begin() + index);
    
    // erase person in email list
    index = getPosition(person->email, 0, orderedByEmail.size() - 1);
    orderedByEmail.erase(orderedByEmail.begin() + index);
    
    // delete person object
    delete person;
    
    return true;
}

bool CPersonalAgenda::Del(const string & email) {
    Person * person;
    int index = getPosition(email, 0, orderedByName.size() - 1);
    
    if (index == not_found) return false;
    
    // erase person in email list
    person = orderedByEmail[index];
    orderedByEmail.erase(orderedByEmail.begin() + index);
    
    // erase person in name list
    index = getPosition(person->name, person->surname, 0, orderedByName.size() - 1);
    orderedByEmail.erase(orderedByEmail.begin() + index);
    
    // delete person object
    delete person;
    
    return true;
}

bool CPersonalAgenda::ChangeName(const string &email, const string &newName, const string &newSurname) {
    Person * person;
    int index = getPosition(email, 0, orderedByEmail.size() - 1);

    if (index == not_found) return false;
    person = orderedByEmail[index];
    person->name = newName;
    person->surname = newSurname;

    return true;
}

bool CPersonalAgenda::ChangeEmail(const string &name, const string &surname, const string &newEmail) {
    Person * person;
    int index = getPosition(name, surname, 0, orderedByName.size() - 1);

    if (index == not_found) return false;
    person = orderedByName[index];
    person->email = newEmail;

    return true;
}

bool CPersonalAgenda::SetSalary(const string &email, unsigned int salary) {
    Person * person;
    int index = getPosition(email, 0, orderedByEmail.size() - 1);

    if (index == not_found) return false;
    person = orderedByEmail[index];
    person->salary = salary;

    return true;
}

bool CPersonalAgenda::SetSalary(const string &name, const string & surname, unsigned int salary) {
    Person * person;
    int index = getPosition(name, surname, 0, orderedByName.size() - 1);

    if (index == not_found) return false;
    person = orderedByName[index];
    person->salary = salary;

    return true;
}

unsigned int CPersonalAgenda::GetSalary(const string &email) const {
    int index = getPosition(email, 0, orderedByEmail.size() - 1);
    return orderedByEmail[index]->salary;
}

unsigned int CPersonalAgenda::GetSalary(const string &name, const string &surname) const {
    int index = getPosition(name, surname, 0, orderedByName.size() - 1);
    return orderedByName[index]->salary;
}

bool CPersonalAgenda::GetRank(const string &email, int &rankMin, int &rankMax) const {
    // @todo

    return true;
}

bool CPersonalAgenda::GetRank(const string &name, const string & surname, int &rankMin, int &rankMax) const {
    // @todo

    return true;
}

bool CPersonalAgenda::GetFirst(string &outName, string &outSurname) const {
    // @todo

    return true;
}

bool CPersonalAgenda::GetNext(const string &name, const string &surname, string &outName, string &outSurname) const {
    // @todo

    return true;
}

// tests ------------------------------------------------------------------------------------------------------------

void run_tests() {
    CPersonalAgenda().test_run();
}

void CPersonalAgenda::test_run() const {
    CPersonalAgenda run;

    // run.test_compareByName();
    run.test_compareByEmail();
    run.test_sort();
    run.test_search();
    run.test_getPosition_name();
    run.test_getPosition_email();
}

void CPersonalAgenda::test_compareByName() const {
    CPersonalAgenda::Person p1("a", "z", "mail", 0), p2("b", "b", "email", 1), p3("b", "b", "email", 2);

    assert( !compareByName( &p2, &p3 ) );
    assert( compareByName( &p1, &p2 ) < 0 );
    assert( compareByName( &p2, &p1 ) > 0 );

    p1.name = "b";

    assert( compareByName( &p1, &p2 ) > 0 );
}

void CPersonalAgenda::test_compareByEmail() const {
    CPersonalAgenda::Person p1("a", "a", "mail", 0), p2("b", "b", "email", 1), p3("b", "b", "email", 2);

    assert( !compareByEmail( &p2, &p3 ) );
    assert( compareByEmail( &p1, &p2 ) > 0 );
    assert( compareByEmail( &p2, &p1 ) < 0 );

    p1.name = "b";

    assert( compareByEmail( &p1, &p2 ) > 0 );
}

void CPersonalAgenda::test_search() const {

}

void CPersonalAgenda::test_sort() const {
    CPersonalAgenda agenda;
    CPersonalAgenda::Person p1("a", "b", "gmail", 0), p2("a", "a", "fmail", 0), p3("c", "a", "email", 0);

    agenda.orderedByName.push_back(&p1);
    agenda.orderedByName.push_back(&p2);
    agenda.orderedByName.push_back(&p3);

    agenda.orderedByEmail.push_back(&p1);
    agenda.orderedByEmail.push_back(&p2);
    agenda.orderedByEmail.push_back(&p3);

    for (auto person : agenda.orderedByName)
        cout << *person << endl;
    cout << endl;
    for (auto person : agenda.orderedByEmail)
        cout << person << endl;

    agenda.sort();
    cout << endl << "Sorted:" << endl;

    for (size_t i = 0; i < agenda.orderedByName.size(); i++)
        cout << *agenda.orderedByName[i] << endl;
    cout << endl;
    for (size_t i = 0; i < agenda.orderedByEmail.size(); i++)
        cout << *agenda.orderedByEmail[i] << endl;


    assert( agenda.orderedByName[0] == &p2 );
    assert( agenda.orderedByName[1] == &p1 );
    assert( agenda.orderedByName[2] == &p3 );

    assert( agenda.orderedByEmail[0] == &p3 );
    assert( agenda.orderedByEmail[1] == &p2 );
    assert( agenda.orderedByEmail[2] == &p1 );
}

void CPersonalAgenda::test_getPosition_email() const {

}

void CPersonalAgenda::test_getPosition_name() const {

}

// run --------------------------------------------------------------------------------------------------------------
#ifndef __PROGTEST__
int main () {
  string outName, outSurname;
  int lo, hi;

  run_tests();
  test_compare();

  return 0; // temporary to run tests

  CPersonalAgenda b1;
  assert ( b1 . Add ( "John", "Smith", "john", 30000 ) );
  assert ( b1 . Add ( "John", "Miller", "johnm", 35000 ) );
  assert ( b1 . Add ( "Peter", "Smith", "peter", 23000 ) );
  assert ( b1 . GetFirst ( outName, outSurname )
           && outName == "John"
           && outSurname == "Miller" );
  assert ( b1 . GetNext ( "John", "Miller", outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( b1 . GetNext ( "John", "Smith", outName, outSurname )
           && outName == "Peter"
           && outSurname == "Smith" );
  assert ( ! b1 . GetNext ( "Peter", "Smith", outName, outSurname ) );
  assert ( b1 . SetSalary ( "john", 32000 ) );
  assert ( b1 . GetSalary ( "john" ) ==  32000 );
  assert ( b1 . GetSalary ( "John", "Smith" ) ==  32000 );
  assert ( b1 . GetRank ( "John", "Smith", lo, hi )
           && lo == 1
           && hi == 1 );
   /*assert ( b1 . GetRank ( "john", lo, hi )
           && lo == 1
           && hi == 1 );
  assert ( b1 . GetRank ( "peter", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . GetRank ( "johnm", lo, hi )
           && lo == 2
           && hi == 2 );
  assert ( b1 . SetSalary ( "John", "Smith", 35000 ) );
  assert ( b1 . GetSalary ( "John", "Smith" ) ==  35000 );
  assert ( b1 . GetSalary ( "john" ) ==  35000 );
  assert ( b1 . GetRank ( "John", "Smith", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . GetRank ( "john", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . GetRank ( "peter", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . GetRank ( "johnm", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . ChangeName ( "peter", "James", "Bond" ) );
  assert ( b1 . GetSalary ( "peter" ) ==  23000 );
  assert ( b1 . GetSalary ( "James", "Bond" ) ==  23000 );
  assert ( b1 . GetSalary ( "Peter", "Smith" ) ==  0 );
  assert ( b1 . GetFirst ( outName, outSurname )
           && outName == "James"
           && outSurname == "Bond" );
  assert ( b1 . GetNext ( "James", "Bond", outName, outSurname )
           && outName == "John"
           && outSurname == "Miller" );
  assert ( b1 . GetNext ( "John", "Miller", outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( ! b1 . GetNext ( "John", "Smith", outName, outSurname ) );
  assert ( b1 . ChangeEmail ( "James", "Bond", "james" ) );
  assert ( b1 . GetSalary ( "James", "Bond" ) ==  23000 );
  assert ( b1 . GetSalary ( "james" ) ==  23000 );
  assert ( b1 . GetSalary ( "peter" ) ==  0 );
  assert ( b1 . Del ( "james" ) );
  assert ( b1 . GetRank ( "john", lo, hi )
           && lo == 0
           && hi == 1 );
  assert ( b1 . Del ( "John", "Miller" ) );
  assert ( b1 . GetRank ( "john", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . GetFirst ( outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( ! b1 . GetNext ( "John", "Smith", outName, outSurname ) );
  assert ( b1 . Del ( "john" ) );
  assert ( ! b1 . GetFirst ( outName, outSurname ) );
  assert ( b1 . Add ( "John", "Smith", "john", 31000 ) );
  assert ( b1 . Add ( "john", "Smith", "joHn", 31000 ) );
  assert ( b1 . Add ( "John", "smith", "jOhn", 31000 ) );

  CPersonalAgenda b2;
  assert ( ! b2 . GetFirst ( outName, outSurname ) );
  assert ( b2 . Add ( "James", "Bond", "james", 70000 ) );
  assert ( b2 . Add ( "James", "Smith", "james2", 30000 ) );
  assert ( b2 . Add ( "Peter", "Smith", "peter", 40000 ) );
  assert ( ! b2 . Add ( "James", "Bond", "james3", 60000 ) );
  assert ( ! b2 . Add ( "Peter", "Bond", "peter", 50000 ) );
  assert ( ! b2 . ChangeName ( "joe", "Joe", "Black" ) );
  assert ( ! b2 . ChangeEmail ( "Joe", "Black", "joe" ) );
  assert ( ! b2 . SetSalary ( "Joe", "Black", 90000 ) );
  assert ( ! b2 . SetSalary ( "joe", 90000 ) );
  assert ( b2 . GetSalary ( "Joe", "Black" ) ==  0 );
  assert ( b2 . GetSalary ( "joe" ) ==  0 );
  assert ( ! b2 . GetRank ( "Joe", "Black", lo, hi ) );
  assert ( ! b2 . GetRank ( "joe", lo, hi ) );
  assert ( ! b2 . ChangeName ( "joe", "Joe", "Black" ) );
  assert ( ! b2 . ChangeEmail ( "Joe", "Black", "joe" ) );
  assert ( ! b2 . Del ( "Joe", "Black" ) );
  assert ( ! b2 . Del ( "joe" ) );
  assert ( ! b2 . ChangeName ( "james2", "James", "Bond" ) );
  assert ( ! b2 . ChangeEmail ( "Peter", "Smith", "james" ) );
  assert ( ! b2 . ChangeName ( "peter", "Peter", "Smith" ) );
  assert ( ! b2 . ChangeEmail ( "Peter", "Smith", "peter" ) );
  assert ( b2 . Del ( "Peter", "Smith" ) );
  assert ( ! b2 . ChangeEmail ( "Peter", "Smith", "peter2" ) );
  assert ( ! b2 . SetSalary ( "Peter", "Smith", 35000 ) );
  assert ( b2 . GetSalary ( "Peter", "Smith" ) ==  0 );
  assert ( ! b2 . GetRank ( "Peter", "Smith", lo, hi ) );
  assert ( ! b2 . ChangeName ( "peter", "Peter", "Falcon" ) );
  assert ( ! b2 . SetSalary ( "peter", 37000 ) );
  assert ( b2 . GetSalary ( "peter" ) ==  0 );
  assert ( ! b2 . GetRank ( "peter", lo, hi ) );
  assert ( ! b2 . Del ( "Peter", "Smith" ) );
  assert ( ! b2 . Del ( "peter" ) );
  assert ( b2 . Add ( "Peter", "Smith", "peter", 40000 ) );
  assert ( b2 . GetSalary ( "peter" ) ==  40000 );
*/
  return 0;
}
#endif /* __PROGTEST__ */
