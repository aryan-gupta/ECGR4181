/*
 * This class runs a simulation, very simple class, all it does is goes through all the instructions and
 * access the memory. Most of the grunt work is in the cache class.
 */


#pragma once

#include <functional>

#include "main.hpp"
#include "Cache.hpp"

class Simulator {
	Cache* mDataCache; ///< The data cache
	Cache* mInsCache;  ///< The instruction cache
	access_type mIns;  ///< The instructions to execute

public:
	Simulator() = default;

	/// Creates a simulator using the ParseData passed in and the instructions inputed
	/// from the file/stdin
	/// @param access The program instructions to execute
	/// @param pd The arguments passed in from cli
	Simulator(access_type&& access, const ParseData& pd);

	/// Destroys the Simulator
	~Simulator();

	/// Does the actual simulation
	void doSim();

	/// Gets the Hit ratio for the current simulation. Returns { 0, 0 } if the simulation
	/// hasnt been run.
	/// @return The ratio of hits to total accesses as a pair of ints
	ratio_t<int> getRatio() const;

};