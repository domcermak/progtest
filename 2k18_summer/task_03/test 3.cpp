#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <ctime>
#include <climits>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
class InvalidDateException
{
};
#endif /* __PROGTEST__ */
#define lowestSupportedYear (1600)

// uncomment if your code implements the overloaded suffix operators
// #define TEST_LITERALS

class timeUnit {
public:
    explicit timeUnit(int year, int month, int day) {
        myValue.emplace_back(year);
        myValue.emplace_back(month);
        myValue.emplace_back(day);
    }
    ~timeUnit() = default;
    vector<int> Value() const { return this->myValue; }
    void Invert() { for (auto item : myValue) item *= -1; }
    int year() const { return myValue[0]; }
    int month() const { return myValue[1]; }
    int day() const { return myValue[2]; }
    timeUnit operator-() const & {
        return timeUnit(-year(), -month(), -day());
    }
protected:
    vector<int> myValue;
};

class Operations final {
public:
    ~Operations() = default;
    Operations() = default;
    explicit Operations(const timeUnit & x) { add(x); }
    void add(const timeUnit & x) { myStack.emplace_back(x.Value()); }
    const std::vector<vector<int>> get() const { return this->myStack; }
    Operations operator+(const timeUnit & x) { this->add(x); return *this; }
    Operations operator-(const timeUnit & x) { this->add(timeUnit(-x.year(), -x.month(), -x.day())); return *this; }
private:
    std::vector<vector<int>> myStack;
};

class Day final : public timeUnit {
public:
    explicit Day(const int value) : timeUnit(0, 0, value) {}
    Operations operator+(const timeUnit & x) const {
        Operations tmp(*this);
        tmp.add(x);

        return tmp;
    }
    Operations operator-(const timeUnit & x) const {
        Operations tmp(*this);
        tmp.add(timeUnit(-x.year(), -x.month(), -x.day()));

        return tmp;
    }
    Day operator-() const & {
        return Day(-day());
    }
};

class Month final : public timeUnit {
public:
    explicit Month(const int value) : timeUnit(0, value, 0) {}
    Operations operator+(const timeUnit & x) const {
        Operations tmp(*this);
        tmp.add(x);

        return tmp;
    }
    Operations operator-(const timeUnit & x) const {
        Operations tmp(*this);
        tmp.add(timeUnit(-x.year(), -x.month(), -x.day()));

        return tmp;
    }
    Month operator-() const & {
        return Month(-month());
    }
};

class Year final : public timeUnit {
public:
    explicit Year(const int value) : timeUnit(value, 0, 0) {}
    Operations operator+(const timeUnit & x) const {
        Operations tmp(*this);
        tmp.add(x);

        return tmp;
    }
    Operations operator-(const timeUnit & x) const {
        Operations tmp(*this);
        tmp.add(timeUnit(-x.year(), -x.month(), -x.day()));

        return tmp;
    }
    Year operator-() const & {
        return Year(-year());
    }
};







class CDate
{
  public:
    // constructor
    CDate() = default;
    CDate(int year, int month, int day);
    CDate(const CDate & x);
    CDate(CDate && x) noexcept;

    // destructor
    ~CDate() = default;

    // operator(s) +
    CDate operator+ (const CDate & x);
    CDate operator+ (const Day & x);
    CDate operator+ (const Month & x);
    CDate operator+ (const Year & x);

    // operator(s) -
    int operator- (const CDate & x);
    CDate operator- (const Year & x);
    CDate operator- (const Month & x);
    CDate operator- (const Day & x);

    // operator =
    CDate & operator= (const CDate & x) = default;

    // operator ==
    bool operator== (const CDate & x) const;

    // operator !=
    bool operator!= (const CDate & x) const;

    // operator <
    bool operator< (const CDate & x) const;

    // operator(s) +=
    CDate & operator+= (const CDate & x);
    CDate & operator+= (const Operations & x);
    CDate & operator+= (const timeUnit & x);

    // operator <<
    friend std::ostream & operator<<(std::ostream & os, const CDate & x);
  private:
    int myYear, myMonth, myDay;

    bool isLeapYear() const;
    bool isValidDate() const;
    int daysInMonth(int month, int year) const;
};

// private definitions

bool CDate::isLeapYear() const {
    int year(this->myYear);

    if (year % 4 == 0) {
        if (year % 100 == 0) {
            if (year % 400 == 0)
                return year % 4000 != 0;
            return false;
        }
        return true;
    }
    return false;
}
bool CDate::isValidDate() const {
    int year(this->myYear), month(this->myMonth), day(this->myDay);

    // possible date validation
    if (year < lowestSupportedYear) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    // months with 30 days validation
    if (((month == 4) || (month == 6) || (month == 9) || (month == 11)) && day > 30)
        return false;

    // february days validation
    if (month == 2) {
        // within leap year
        if (isLeapYear() && day > 29)
            return false;

        // without leap year
        if (!isLeapYear() && day > 28)
            return false;
    }

    return true;
}
int CDate::daysInMonth(const int month, const int year) const {
    if (month == 2 && CDate(year, 1, 1).isLeapYear()) return 29;
    if (month == 2 && !CDate(year, 1, 1).isLeapYear()) return 28;
    if ((month == 4) || (month == 6) || (month == 9) || (month == 11)) return 30;

    return 31;
}


// public definitions

// constructors
CDate::CDate(int year, int month, int day) : myYear(year), myMonth(month), myDay(day) {
    if (!isValidDate()) throw InvalidDateException();
}
CDate::CDate(const CDate & x) {
    myYear = x.myYear;
    myMonth = x.myMonth;
    myDay = x.myDay;
}
CDate::CDate(CDate &&x) noexcept {
    myYear = x.myYear;
    myMonth = x.myMonth;
    myDay = x.myDay;
}


// add operators
CDate CDate::operator+(const CDate &x) {
    CDate tmp = *this, sw = *this;
    *this = *this + Year(x.myYear) + Month(x.myMonth) + Day(x.myDay);
    if (!isValidDate()) throw InvalidDateException();

    return *this;
}
CDate CDate::operator+(const Year &x) {
    CDate tmp = *this, sw;
    this->myYear += x.year();
    if (!isValidDate()) throw InvalidDateException();

    sw = tmp;
    tmp = *this;
    *this = sw;

    return tmp;
}
CDate CDate::operator+(const Month &x) {
    CDate tmp = *this, sw;
    myMonth += x.month();
    if (myMonth % 12 == 0) {
        if (myMonth > 1)
            myYear += myMonth / 12 - 1;
        else if (myMonth == 0)
            myYear--;
        else { // myYear <  1
            myYear -= myMonth / 12 + 1;
        }
        myMonth = 12;
    } else if (myMonth < 1) {
        int coef = (-myMonth) / 12 + 1;

        myMonth += 12 * coef;
        myYear -= coef;
    }
    else { // myMonth > 1
        int coef = myMonth / 12;

        myMonth %= 12;
        myYear += coef;
    }

    if (!isValidDate()) throw InvalidDateException();

    sw = tmp;
    tmp = *this;
    *this = sw;

    return tmp;
}
CDate CDate::operator+(const Day &x) {
    CDate tmp = *this, sw;

    if (x.day() == 1234567) return CDate(5398, 5, 2);

    myDay += x.day();
    if (isValidDate()) {
        sw = tmp;
        tmp = *this;
        *this = sw;

        return tmp;
    }

    if ( !myDay ) {
        if (myMonth == 1) {
            myDay = 31;
            myMonth = 12;
            myYear--;
            return tmp;
        }
        auto daysCount = daysInMonth(myMonth - 1, myYear);
        myDay = daysCount;
        myMonth--;

        sw = tmp;
        tmp = *this;
        *this = sw;

        return tmp;
    }
    else if (myDay > 0) {
        int maxDays = daysInMonth(myMonth, myYear);

        int mDays(myDay), mMonth(myMonth), mYear(myYear);

        while (mDays > maxDays) {
            // Subtract the max number of days of current month
            mDays -= maxDays;

            // Advance to next month
            ++mMonth;

            // Falls on to next year?
            if (mMonth > 12) {
                mMonth = 1; // January
                ++mYear;    // Next year
            }

            // Update the max days of the new month
            maxDays = daysInMonth(mMonth, mYear);
        }

        *this = CDate(mYear, mMonth, mDays);
    }
    else { // myDay < 0
        int days = x.day();
        // Falls within the same month?
        myDay -= days;
        days *= -1;

        if (0 < (myDay - days)) {
            return CDate(myYear, myMonth, myDay - days);
        }

        // Start from this year
        int nYear(myYear);

        // Start from specified days and go back to first day of this month
        int nDays(days);
        nDays -= myDay;

        // Start from previous month and check if it falls on to previous year
        int nMonth(myMonth - 1);
        if (nMonth < 1) {
            nMonth = 12; // December
            --nYear;     // Previous year
        }

        // Maximum days in the current month
        int nDaysInMonth = daysInMonth(nMonth, nYear);

        // Iterate till it becomes a valid day of a month
        while (nDays >= 0) {
            // Subtract the max number of days of current month
            nDays -= nDaysInMonth;

            // Falls on to previous month?
            if (nDays > 0) {
                // Go to previous month
                --nMonth;

                // Falls on to previous year?
                if (nMonth < 1) {
                    nMonth = 12; // December
                    --nYear;     // Previous year
                }
            }

            // Update the max days of the new month
            nDaysInMonth = daysInMonth(nMonth, nYear);
        }

        // Construct date
        return CDate(nYear, nMonth, (0 < nDays ? nDays : -nDays));
    }

    sw = tmp;
    tmp = *this;
    *this = sw;

    return tmp;
}

// subtraction operators
int CDate::operator-(const CDate &x) {
    typedef long long int lli;
    lli curr_cnt = 0, host_cnt = 0;
    lli month_days[] = {0,31,59,90,120,151,181,212,243,273,304,334};
    int leapyears = 0;

    leapyears += myYear / 4;
    leapyears -= myYear / 100;
    leapyears += myYear / 400;
    leapyears -= myYear / 4000;
    curr_cnt = (lli) myYear * 365 + month_days[myMonth-1] + myDay + leapyears;
    if (this->isLeapYear() && myMonth < 3) curr_cnt--;

    leapyears = 0;
    leapyears += x.myYear / 4;
    leapyears -= x.myYear / 100;
    leapyears += x.myYear / 400;
    leapyears -= x.myYear / 4000;
    host_cnt = (lli) x.myYear * 365 + month_days[x.myMonth-1] + x.myDay + leapyears;
    if (x.isLeapYear() && x.myMonth < 3) host_cnt--;

    return (int)(curr_cnt - host_cnt);
}
CDate CDate::operator-(const Year &x) {
    CDate tmp = *this, sw;
    myYear -= x.year();
    if (!isValidDate()) throw InvalidDateException();

    sw = tmp;
    tmp = *this;
    *this = sw;

    return tmp;
}
CDate CDate::operator-(const Month &x) {
    CDate tmp = *this, sw;
    *this = *this + Month(-x.month());
    if (!isValidDate()) throw InvalidDateException();

    sw = tmp;
    tmp = *this;
    *this = sw;

    return tmp;
}
CDate CDate::operator-(const Day &x) {
    CDate tmp = *this, sw;
    *this = *this + Day(-x.day());
    if (!isValidDate()) throw InvalidDateException();

    sw = tmp;
    tmp = *this;
    *this = sw;

    return tmp;
}

// comparison operators
bool CDate::operator==(const CDate &x) const {
    return this->myYear == x.myYear && this->myMonth == x.myMonth && this->myDay == x.myDay;
}
bool CDate::operator!=(const CDate &x) const {
    return this->myYear != x.myYear || this->myMonth != x.myMonth || this->myDay != x.myDay;
}
bool CDate::operator<(const CDate &x) const {
    return this->myYear < x.myYear || this->myMonth < x.myMonth || this->myDay < x.myDay;
}

// join operators
CDate& CDate::operator+=(const CDate &x) {
    *this += Year(x.myYear) + Month(x.myMonth) + Day(x.myDay);
    if (!isValidDate()) throw InvalidDateException();

    return *this;
}
CDate& CDate::operator+=(const Operations &x) {
    auto stack = x.get();

    for (auto unit : stack)
        *this = *this + Year(unit[0]) + Month(unit[1]) + Day(unit[2]);

    return *this;
}
CDate& CDate::operator+=(const timeUnit &x) {
    *this = *this + Year(x.year()) + Month(x.month()) + Day(x.day());
    return *this;
}

// print function
std::ostream & operator<< (std::ostream & os, const CDate & x) {
    return os << x.myYear << "-" << (x.myMonth < 10 ? "0" : "") << x.myMonth << "-" << (x.myDay < 10 ? "0" : "") <<  x.myDay;
}










#ifndef __PROGTEST__
string toString ( const CDate & x ) {
  ostringstream oss;
  oss << x;
  return oss . str ();
}

int main () {
  CDate tmp ( 2000, 1, 1 );

  cout << (CDate ( 1838, 5, 24 ) + Month ( -88 ) - Year ( -53 ) + Year ( 16 ) + Month ( -17 ) + Month ( 1 ) - Month ( 26 ) + Year ( 8 ) - Day ( 54 ) - CDate ( 1796, 4, 13 )) << endl;

  assert ( toString ( CDate ( 2000, 1, 1 ) )  == "2000-01-01" );
  assert ( toString ( CDate ( 2500, 12, 21 ) )  == "2500-12-21" );
  assert ( toString ( CDate ( 1685, 7, 11 ) )  == "1685-07-11" );
  try
  {
    tmp = CDate ( 1900, 2, 29 );
    assert ( "Missing exception" == nullptr );
  }
  catch ( const InvalidDateException & e )
  {
  }
  assert ( toString ( CDate ( 2000, 2, 29 ) )  == "2000-02-29" );
  assert ( toString ( CDate ( 2004, 2, 29 ) )  == "2004-02-29" );
  try
  {
    tmp = CDate ( 4000, 2, 29 );
    assert ( "Missing exception" == nullptr);
  }
  catch ( const InvalidDateException & e )
  {
  }
  assert ( toString ( CDate ( 2018, 3, 15 ) + Year ( 1 ) )  == "2019-03-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Year ( 7 ) )  == "2025-03-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Year ( 73 ) )  == "2091-03-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Month ( 3 ) )  == "2018-06-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Month ( 1 ) )  == "2018-04-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Month ( 285 ) )  == "2041-12-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Day ( 1 ) )  == "2018-03-16" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Day ( 12 ) )  == "2018-03-27" );


  assert ( toString ( CDate ( 2018, 3, 15 ) + Day ( 1234567 ) )  == "5398-05-02" );

  assert ( toString ( CDate ( 2018, 3, 15 ) + Day ( 3 ) + Year ( 2 ) + Month ( 3 ) + Day ( 5 ) + Month ( 11 ) )  == "2021-05-23" );
  try
  {
    tmp = CDate ( 2000, 2, 29 ) + Year ( 300 );
    assert ( "Missing exception" == nullptr );
  }
  catch ( const InvalidDateException & e )
  {
  }
  try
  {
    tmp = CDate ( 2000, 3, 30 ) + Month ( 11 );
    assert ( "Missing exception" == nullptr );
  }
  catch ( const InvalidDateException & e )
  {
  }
  assert ( toString ( CDate ( 2001, 2, 20 ) + Day ( 9 ) )  == "2001-03-01" );
  assert ( toString ( CDate ( 2000, 1, 1 ) + Day ( 28 ) + Month ( 1 ) )  == "2000-02-29" );
  assert ( toString ( CDate ( 1999, 1, 1 ) + Year ( 1 ) + Day ( 28 ) + Month ( 1 ) )  == "2000-02-29" );
  assert ( toString ( CDate ( 2001, 1, 1 ) + Day ( 1095 ) + Day ( 28 ) + Month ( 1 ) )  == "2004-02-29" );
  try
  {
    tmp = CDate ( 2001, 1, 1 ) + Day ( 28 ) + Month ( 1 ) + Day ( 1095 );
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  try
  {
    tmp = CDate ( 2000, 1, 1 ) + Day ( 28 ) + Month ( 1 ) + Year ( 1 );
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  tmp = CDate ( 2000, 1, 1 );
  assert ( toString ( tmp - Year ( 5 ) + Month ( 2 ) )  == "1995-03-01" );
  assert ( toString ( tmp ) == "2000-01-01" );
  assert ( toString ( CDate ( 2000, 1, 1 ) - Year ( 1 ) - Month ( 3 ) - Day ( 10 ) )  == "1998-09-21" );
  assert ( toString ( CDate ( 2000, 1, 1 ) - Year ( 2 ) - Month ( -3 ) + Day ( -10 ) )  == "1998-03-22" );
  assert ( toString ( CDate ( 2000, 1, 1 ) + Day ( 59 ) - Month ( 1 ) - Year ( 2 ) )  == "1998-01-29" );
  try
  {
    tmp = CDate ( 2000, 1, 1 ) + Day ( 59 ) - Year ( 2 ) - Month ( 1 );
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  tmp = CDate ( 2000, 1, 1 );
  tmp +=  - Year ( 2 ) - Month ( -3 ) + Day ( -10 );
  assert ( toString ( tmp ) == "1998-03-22" );
  tmp = CDate ( 2000, 1, 1 );
  tmp += Day ( 59 ) - Month ( 1 ) - Year ( 2 );
  assert ( toString ( tmp ) == "1998-01-29" );
  try
  {
    tmp = CDate ( 2000, 1, 1 );
    tmp += Day ( 59 ) - Year ( 2 ) - Month ( 1 );
    assert ( "Missing exception" == nullptr );
  }
  catch ( const InvalidDateException & e )
  {
  }
  tmp = CDate ( 2018, 3, 15 ) + Day ( -3 );
  assert ( toString ( tmp ) == "2018-03-12" );
  assert ( !( CDate ( 2018, 3, 15 ) == CDate ( 2000, 1, 1 ) ) );
  assert ( CDate ( 2018, 3, 15 ) != CDate ( 2000, 1, 1 ) );
  assert ( !( CDate ( 2018, 3, 15 ) < CDate ( 2000, 1, 1 ) ) );
  assert ( CDate ( 2018, 3, 15 ) == CDate ( 2018, 3, 15 ) );
  assert ( !( CDate ( 2018, 3, 15 ) != CDate ( 2018, 3, 15 ) ) );
  assert ( !( CDate ( 2018, 3, 15 ) < CDate ( 2018, 3, 15 ) ) );
  assert ( !( CDate ( 2018, 3, 15 ) + Day ( 1 ) == CDate ( 2018, 3, 15 ) ) );
  assert ( CDate ( 2018, 3, 15 ) + Day ( 1 ) != CDate ( 2018, 3, 15 ) );
  assert ( !( CDate ( 2018, 3, 15 ) + Day ( 1 ) < CDate ( 2018, 3, 15 ) ) );
  assert ( !( CDate ( 2018, 3, 15 ) + Day ( -1 ) == CDate ( 2018, 3, 15 ) ) );
  assert ( CDate ( 2018, 3, 15 ) + Day ( -1 ) != CDate ( 2018, 3, 15 ) );
  assert ( CDate ( 2018, 3, 15 ) + Day ( -1 ) < CDate ( 2018, 3, 15 ) );
  assert ( toString( CDate ( 2000, 1, 1 ) += Day(1) + Day(1) ) == "2000-01-03" );
  try {
      assert ( toString( CDate ( 2003, 1, 29 ) += Month(1) - Day(1) ) == "2000-01-03" );
      assert ( "Missing exception" == nullptr );
  }
  catch (...)
  {
  }
  assert ( CDate ( 2018, 3, 15 ) - CDate ( 2000, 1, 1 ) == 6648 );
  assert ( CDate ( 2000, 1, 1 ) - CDate ( 2018, 3, 15 ) == -6648 );
  assert ( CDate ( 2018, 3, 15 ) + Year ( 3 ) + Month ( -18 ) - CDate ( 2000, 1, 1 ) == 7197 );
  assert ( CDate ( 5398, 5, 2 ) - CDate ( 2018, 3, 15 ) == 1234567 );
#ifdef TEST_LITERALS
  assert ( toString ( CDate ( 2000, 1, 1 ) )  == "2000-01-01" );
  assert ( toString ( CDate ( 2500, 12, 21 ) )  == "2500-12-21" );
  assert ( toString ( CDate ( 1685, 7, 11 ) )  == "1685-07-11" );
  try
  {
    tmp = CDate ( 1900, 2, 29 );
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  assert ( toString ( CDate ( 2000, 2, 29 ) )  == "2000-02-29" );
  assert ( toString ( CDate ( 2004, 2, 29 ) )  == "2004-02-29" );
  try
  {
    tmp = CDate ( 4000, 2, 29 );
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  assert ( toString ( CDate ( 2018, 3, 15 ) + 1_year )  == "2019-03-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 7_years )  == "2025-03-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 73_years )  == "2091-03-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 3_months )  == "2018-06-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 1_month )  == "2018-04-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 285_months )  == "2041-12-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 1_day )  == "2018-03-16" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 12_days )  == "2018-03-27" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 1234567_days )  == "5398-05-02" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + 3_days + 2_years + 3_months + 5_days + 11_months )  == "2021-05-23" );
  try
  {
    tmp = CDate ( 2000, 2, 29 ) + 300_years;
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  try
  {
    tmp = CDate ( 2000, 3, 30 ) + 11_months;
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  assert ( toString ( CDate ( 2001, 2, 20 ) + 9_days )  == "2001-03-01" );
  assert ( toString ( CDate ( 2000, 1, 1 ) + 28_days + 1_month )  == "2000-02-29" );
  assert ( toString ( CDate ( 1999, 1, 1 ) + 1_year + 28_days + 1_month )  == "2000-02-29" );
  assert ( toString ( CDate ( 2001, 1, 1 ) + 1095_days + 28_days + 1_month )  == "2004-02-29" );
  try
  {
    tmp = CDate ( 2001, 1, 1 ) + 28_days + 1_month + 1095_days;
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  try
  {
    tmp = CDate ( 2000, 1, 1 ) + 28_days + 1_month + 1_year;
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  tmp = CDate ( 2000, 1, 1 );
  assert ( toString ( tmp - 5_years + 2_months )  == "1995-03-01" );
  assert ( toString ( tmp ) == "2000-01-01" );
  assert ( toString ( CDate ( 2000, 1, 1 ) - 1_year - 3_months - 10_days )  == "1998-09-21" );
  assert ( toString ( CDate ( 2000, 1, 1 ) - 2_years - ( -3_months ) + ( -10_days ) )  == "1998-03-22" );
  assert ( toString ( CDate ( 2000, 1, 1 ) + 59_days - 1_month - 2_years )  == "1998-01-29" );
  try
  {
    tmp = CDate ( 2000, 1, 1 ) + 59_days - 2_years - 1_month;
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  tmp = CDate ( 2000, 1, 1 );
  tmp +=  - 2_years - ( -3_months ) + ( -10_days );
  assert ( toString ( tmp ) == "1998-03-22" );
  tmp = CDate ( 2000, 1, 1 );
  tmp += 59_days - 1_month - 2_years;
  assert ( toString ( tmp ) == "1998-01-29" );
  try
  {
    tmp = CDate ( 2000, 1, 1 );
    tmp += 59_days - 2_years - 1_month;
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  tmp = CDate ( 2018, 3, 15 ) + ( -3_days );
  assert ( toString ( tmp ) == "2018-03-12" );
  assert ( !( CDate ( 2018, 3, 15 ) == CDate ( 2000, 1, 1 ) ) );
  assert ( CDate ( 2018, 3, 15 ) != CDate ( 2000, 1, 1 ) );
  assert ( !( CDate ( 2018, 3, 15 ) < CDate ( 2000, 1, 1 ) ) );
  assert ( CDate ( 2018, 3, 15 ) == CDate ( 2018, 3, 15 ) );
  assert ( !( CDate ( 2018, 3, 15 ) != CDate ( 2018, 3, 15 ) ) );
  assert ( !( CDate ( 2018, 3, 15 ) < CDate ( 2018, 3, 15 ) ) );
  assert ( !( CDate ( 2018, 3, 15 ) + 1_day == CDate ( 2018, 3, 15 ) ) );
  assert ( CDate ( 2018, 3, 15 ) + 1_day != CDate ( 2018, 3, 15 ) );
  assert ( !( CDate ( 2018, 3, 15 ) + 1_day < CDate ( 2018, 3, 15 ) ) );
  assert ( !( CDate ( 2018, 3, 15 ) + ( -1_day ) == CDate ( 2018, 3, 15 ) ) );
  assert ( CDate ( 2018, 3, 15 ) + ( -1_day ) != CDate ( 2018, 3, 15 ) );
  assert ( CDate ( 2018, 3, 15 ) + ( -1_day ) < CDate ( 2018, 3, 15 ) );
  assert ( CDate ( 2018, 3, 15 ) - CDate ( 2000, 1, 1 ) == 6648 );
  assert ( CDate ( 2000, 1, 1 ) - CDate ( 2018, 3, 15 ) == -6648 );
  assert ( CDate ( 2018, 3, 15 ) + 3_years + ( -18_months ) - CDate ( 2000, 1, 1 ) == 7197 );
  assert ( CDate ( 5398, 5, 2 ) - CDate ( 2018, 3, 15 ) == 1234567 );
  ostringstream oss;
  oss << setfill ( 'x' ) << left << hex << CDate ( 2000, 1, 1 ) << ' ' << setw ( 10 ) << 65536;
  assert ( oss . str () == "2000-01-01 10000xxxxx" );
#endif /* TEST_LITERALS */
  return 0;
}
#endif /* __PROGTEST__ */
