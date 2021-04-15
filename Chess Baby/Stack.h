#include <iostream>
#include "Piece.h"

#ifndef STACK_H_
#define STACK_H_

class Stack {

private:
	int maxStackSize, topOfStack;
	std::unique_ptr<Piece *[]> capturedPieces;
	std::unique_ptr<int []> depthOfCapturedPiece;
	
	
public:

	Stack(const int maxStackSize) {
		this->maxStackSize = maxStackSize;
		topOfStack = -1;
		capturedPieces = std::make_unique<Piece* []>(maxStackSize);
		depthOfCapturedPiece = std::make_unique<int []>(maxStackSize);
	}

	void clearStack() { topOfStack = -1; }

	void push(Piece* capturedPiece, const int& depth) {
		if (topOfStack == maxStackSize - 1)
			throw std::overflow_error("Stack is full!\n");
		else {
			capturedPieces[++topOfStack] = capturedPiece;
			depthOfCapturedPiece[topOfStack] = depth;
		}
	}

	Piece* popPiece()  {
		if (topOfStack == -1)
			throw std::underflow_error("Stack is empty!\n");
		else
			return capturedPieces[topOfStack--];
	}

	int peekDepth() const { return depthOfCapturedPiece[topOfStack]; }

	int size() { return (topOfStack + 1); }
};

#endif
