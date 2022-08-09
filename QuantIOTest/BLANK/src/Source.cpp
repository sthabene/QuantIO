#include "Source.hpp"

int ReturnRandom(int a) {
	return a * 5;
}

int UsingSource::returnd() {
	return m_d;
}

void UsingSource::returnf() {
	std::cout << this->m_f;
}