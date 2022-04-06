#pragma once
#include <iostream>
#include <vector>


class instancia {
public:
	std::vector<std::vector<int>> instancias;
	instancia() {
		instancias = { {4,2}, {4,3}, {5,2}, {5,3},
			{6,2}, {6,3}, {7,2}, {7,3}, 
			{8,2}, {8,4}, {9,4}, {10,2}, 
			{10,3}, {10,4}};
	}
	virtual ~instancia() {};
};

