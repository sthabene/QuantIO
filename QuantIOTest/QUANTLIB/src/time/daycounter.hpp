#include <ql/time/daycounter.hpp>

using namespace QuantLib;

class CustomDayCounter : public DayCounter {
public:
	class CustomDayCounterImpl : public DayCounter::Impl {
	public:
		std::string m_dayCounterName;
		std::string name() const override;
		Date::serial_type dayCount(const Date& d1, const Date& d2) const override;
		Time yearFraction(const Date& d1, const Date& d2, const Date&, const Date&) const override;
		CustomDayCounterImpl(std::string& name) : m_dayCounterName(name) {};
	};
public:
	CustomDayCounter() = default;
	CustomDayCounter(std::string& name);
	CustomDayCounter(const CustomDayCounter& customDayCounter);
};