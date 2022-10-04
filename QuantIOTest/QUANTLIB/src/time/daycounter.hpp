#include <ql/time/daycounter.hpp>

using namespace QuantLib;

class CustomDayCounter : public DayCounter {
public:
	class CustomDayCounterImpl : public DayCounter::Impl {
	public:
		std::string m_dayCounterName;
		float m_yearDenominator;
		bool m_includeLast;
		std::string name() const override;
		//number of days between two dates
		Date::serial_type dayCount(const Date& d1, const Date& d2) const override; 
		//the period between two dates as a fraction of year
		Time yearFraction(const Date& d1, const Date& d2, const Date&, const Date&) const override;
		CustomDayCounterImpl(std::string& name, float& yearDenominator, bool& includeLast) : m_dayCounterName(name),
			m_yearDenominator(yearDenominator), m_includeLast(includeLast) {};
	};
public:
	CustomDayCounter() = default;
	CustomDayCounter(std::string& name, float& yearDenominator, bool& includeLast);
	CustomDayCounter(const CustomDayCounter& customDayCounter);

	void setAnotherDayCounter(std::string& name, float& yearDenominator, bool& includeLast);
};