

////////
// THis is the dirtiest impl of this idea I have, I dont know what it will become
// Basic Idea is that I want a collection where I can reference an object like a hashtable
// using 2 keys (multiple indexed) with 0(1) time

template <typename PKey, typename SKey, typename Value>
struct MIMItem {
	Value* mVal;
	PKey* mKey1;
	SKey* mKwy2;
};

template <typename PKey, typename SKey, typename Value>
struct MIMPKey {
	PKey mKey;
	MIMItem<PKey, SKey, Value>* mPtr;
};

template <typename PKey, typename SKey, typename Value>
struct MIMSKey {
	SKey mKey;
	MIMItem<PKey, SKey, Value>* mPtr;
};

// The two keys are primary Key and secondary Key

template <typename PKey, typename SKey, typename V>
class multi_index_map {
	std::vector<MIMPKey> mKeySet1;
	std::vector<MIMSKey> mKeySet2;

public:
	void add(PKey p, SKey s, V item) {

	}

	bool lookup(PKey p) {

	}

	bool lookup(SKey s) {

	}

	void remove(PKey p)

};