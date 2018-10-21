
#pragma once

#include <functional>

#include "main.hpp"
#include "Cache.hpp"


class Simulator {
	Cache* mDataCache;
	Cache* mInsCache;
	access_type mIns;

public:
	Simulator() = default;
	Simulator(access_type&& access, const ParseData& pd);
	~Simulator();

	void doSim();

	ratio_t<int> getRatio() const;

};