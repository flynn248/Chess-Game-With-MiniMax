#include <iostream>
#include "Piece.h"

#ifndef STACK_H_
#define STACK_H_

class Stack {

private:
	int maxStackSize, topOfStack,
		topOfStackEnPassant;
	std::unique_ptr<Piece *[]> capturedPieces;
	std::unique_ptr<int []> depthOfCapturedPiece;
	std::unique_ptr<unsigned long long[]> enPassantBoard;
	
public:

	Stack(const int maxStackSize) {
		this->maxStackSize = maxStackSize;
		topOfStack = -1;
		topOfStackEnPassant = -1;
		capturedPieces = std::make_unique<Piece* []>(maxStackSize);
		depthOfCapturedPiece = std::make_unique<int []>(maxStackSize);
		enPassantBoard = std::make_unique<unsigned long long[]>(maxStackSize);
	}

	void clearStack() { 
		topOfStack = -1;
		topOfStackEnPassant = -1;
	}

	void push(Piece* capturedPiece, const int& depth) {
		if (topOfStack == maxStackSize - 1)
			throw std::overflow_error("Stack is full!\n");
		else {
			capturedPieces[++topOfStack] = capturedPiece;
			depthOfCapturedPiece[topOfStack] = depth;
		}
	}
	void pushEnPassantBoard(unsigned long long& enPassBoard) {
		if (topOfStackEnPassant == maxStackSize - 1)
			throw std::overflow_error("EnPassant stack is full!\n");
		else {
			enPassantBoard[++topOfStackEnPassant] = enPassBoard;
		}
	}

	Piece* popPiece()  {
		if (topOfStack == -1)
			throw std::underflow_error("Stack is empty!\n");
		else
			return capturedPieces[topOfStack--];
	}
	unsigned long long popEnPassantBoard() {
		if (topOfStackEnPassant == -1)
			throw std::underflow_error("EnPassant stack is empty!\n");
		else
			return enPassantBoard[topOfStackEnPassant--];
	}
	int peekDepth() const { return depthOfCapturedPiece[topOfStack]; }

	int size() { return (topOfStack + 1); }
	int sizeEnPassant() { return topOfStackEnPassant + 1; }
};

#endif
