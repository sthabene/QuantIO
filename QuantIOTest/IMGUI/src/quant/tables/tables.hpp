#pragma once
#include "quant.hpp"

namespace QuantIO {

	class Tables {
	public:
		void Table(std::string& query, std::string& title, 
			std::vector<std::vector<std::string>>& data, int& refresh); //, void (f)(std::string& row)

	};

};