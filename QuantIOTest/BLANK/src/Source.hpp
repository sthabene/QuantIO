#pragma once
#include <iostream>
#include <vector>

int ReturnRandom(int a);

class Source {
private:
	int m_a, m_b, m_c;
public:
	Source(int a, int b, int c) : m_a(a), m_b(b), m_c(c) {}
	virtual void returnf() = 0;
};

class UsingSource : Source {
private:
	int m_d, m_e, m_f;
public:
	UsingSource() : Source(2,3, ReturnRandom(4)) , m_d(3), m_e(2), m_f(ReturnRandom(4)) {}
	int returnd();
	void returnf() override;
};

