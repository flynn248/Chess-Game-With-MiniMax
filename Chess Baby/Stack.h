#include <iostream>
#include "Piece.h"

#ifndef STACK_H_
#define STACK_H_

class Stack {

private:
	int maxStackSize, topOfStack;
	std::unique_ptr<Piece *[]> capturedPieces;
	
	
public:

	Stack(const int maxStackSize) {
		this->maxStackSize = maxStackSize;
		topOfStack = -1;
		capturedPieces = std::make_unique<Piece* []>(maxStackSize);
	}

	void clearStack() { topOfStack = -1; }

	void push(Piece* capturedPiece) {
		if (topOfStack == maxStackSize - 1)
			throw std::overflow_error("Stack is full!\n");
		else
			capturedPieces[++topOfStack] = capturedPiece;
	}

	Piece* pop()  {
		if (topOfStack == -1)
			throw std::underflow_error("Stack is empty!\n");
		else
			return capturedPieces[topOfStack--];
	}

	int size() { return (topOfStack + 1); }
};

#endif
