#pragma once
#include "vector.h"

Vector<int> computePowerIndexes(Vector<int>& blocks);
Vector<int> convertToPercentages(const Vector<int>& vector);
int calculateCriticalVotes(Vector<int>& rest, Vector<int>& subset, int target, int boundary, int index, int currentSum);
