#include <vector>
#include <memory>
#ifndef ALLLEGALMOVES_H
#define ALLLEGALMOVES_H

struct AllLegalMoves {
	std::unique_ptr<std::vector<std::unique_ptr<std::vector<uint16_t>>>> allMoves;


	AllLegalMoves() {
		allMoves = std::make_unique<std::vector<std::unique_ptr<std::vector<uint16_t>>>>();
		allMoves->resize(8);
	}
};

#endif // !ALLLEGALMOVES_H