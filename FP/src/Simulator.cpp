

#include <string>

#include "main.hpp"
#include "Simulator.hpp"
#include "Always.hpp"
#include "OneBit.hpp"
#include "TwoBit.hpp"
#include "TwoLevelGlobal.hpp"
#include "TwoLevelGShare.hpp"
#include "TwoLevelGSelect.hpp"
#include "TwoLevelLocal.hpp"
#include "BTFN.hpp"
#include "AgreeBTFN.hpp"
#include "AgreeFirst.hpp"
#include "AgreeISA.hpp"

Simulator::Simulator(trace_t&& trace, ParseData dat)
	: mTrace{ std::move(trace) }
	, mBP{ nullptr }
	, mCorrect{ 0 }
	, mTotal{ 0 }
{
	switch (dat.predictor) {
		using namespace BranchPredictorTypes;
		case Predictor::ALWAYSN: mBP = new Always{ false }; break;
		case Predictor::ALWAYST: mBP = new Always{ true }; break;
		case Predictor::ONE_BIT: mBP = new OneBit{ dat.shift_reg_bits }; break;
		case Predictor::TWO_BIT: mBP = new TwoBit{ dat.shift_reg_bits, dat.saturation_bits }; break;
		case Predictor::GLOBAL:  mBP = new TwoLevelGlobal{ dat.shift_reg_bits, dat.saturation_bits }; break;
		case Predictor::GSHARE:  mBP = new TwoLevelGShare{ dat.shift_reg_bits, dat.saturation_bits }; break;
		case Predictor::GSELECT: mBP = new TwoLevelGSelect{ dat.shift_reg_bits, dat.saturation_bits, dat.addr_bits }; break;
		case Predictor::LOCAL:   mBP = new TwoLevelLocal{ dat.shift_reg_bits, dat.saturation_bits, dat.lhr_bits }; break;
		case Predictor::BTFN:    mBP = new BTFN{  }; break;

		case Predictor::AGREEBTFN: { // Gshare Global Local TwoBit
			switch(dat.agree_base) {
				case Predictor::TWO_BIT: mBP = new AgreeBTFN<TwoBit>{ dat.shift_reg_bits, dat.saturation_bits }; break;
				case Predictor::GLOBAL:  mBP = new AgreeBTFN<TwoLevelGlobal>{ dat.shift_reg_bits, dat.saturation_bits }; break;
				case Predictor::GSHARE:  mBP = new AgreeBTFN<TwoLevelGShare>{ dat.shift_reg_bits, dat.saturation_bits }; break;
				case Predictor::LOCAL:   mBP = new AgreeBTFN<TwoLevelLocal>{ dat.shift_reg_bits, dat.saturation_bits, dat.lhr_bits }; break;
				default: throw std::invalid_argument{ "[E] " + std::to_string((int)dat.agree_base) + " is not a valid agree predictor base" };
			}
		} break;

		case Predictor::AGREEFIRST: {
			switch(dat.agree_base) {
				case Predictor::TWO_BIT: mBP = new AgreeFirst<TwoBit>{ dat.shift_reg_bits, dat.saturation_bits }; break;
				case Predictor::GLOBAL:  mBP = new AgreeFirst<TwoLevelGlobal>{ dat.shift_reg_bits, dat.saturation_bits }; break;
				case Predictor::GSHARE:  mBP = new AgreeFirst<TwoLevelGShare>{ dat.shift_reg_bits, dat.saturation_bits }; break;
				case Predictor::LOCAL:   mBP = new AgreeFirst<TwoLevelLocal>{ dat.shift_reg_bits, dat.saturation_bits, dat.lhr_bits }; break;
				default: throw std::invalid_argument{ "[E] " + std::to_string((int)dat.agree_base) + " is not a valid agree predictor base" };
			}
		} break;

		case Predictor::AGREEISA: {
			switch(dat.agree_base) {
				case Predictor::TWO_BIT: mBP = new AgreeISA<TwoBit>{ dat.shift_reg_bits, dat.saturation_bits }; break;
				case Predictor::GLOBAL:  mBP = new AgreeISA<TwoLevelGlobal>{ dat.shift_reg_bits, dat.saturation_bits }; break;
				case Predictor::GSHARE:  mBP = new AgreeISA<TwoLevelGShare>{ dat.shift_reg_bits, dat.saturation_bits }; break;
				case Predictor::LOCAL:   mBP = new AgreeISA<TwoLevelLocal>{ dat.shift_reg_bits, dat.saturation_bits, dat.lhr_bits }; break;
				default: throw std::invalid_argument{ "[E] " + std::to_string((int)dat.agree_base) + " is not a valid agree predictor base" };
			}
		} break;

		default: throw std::invalid_argument{ "[E] " + std::to_string((int)dat.predictor) + " is not a valid predictor" };
	}
}

Simulator::~Simulator() {
	delete mBP;
}

void Simulator::operator()() {
	BranchPredictorTypes::BranchPredictor& bp = *mBP;

	for (auto& p : mTrace) {
		++mTotal;

		// if (mTotal % 10000 == 0) {
		// 	std::cout << mCorrect << "\t" << mTotal << "\t" << (double(mCorrect) / mTotal) << std::endl;
		// }

		bool guess = bp(p.address, p.target, p.ext);
		bp(p.address, p.result, guess);

		if (guess == p.result) {
			++mCorrect;
		}
	}
}

ratio_t Simulator::getResults() {
	return { mCorrect, mTotal };
}