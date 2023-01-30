#pragma once
#include "track.h"

namespace tema2 {
	enum class treeType { FIR };

	class Forest : public Object {
	public:
		Forest(tema2::Track *currentTrack);
		Forest();

		std::vector<tema2::treeType> getTreesType();

	private:
		tema2::Track* currentTrack;
		std::vector<tema2::treeType> trees;
	};
}