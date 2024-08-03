#pragma once

#include "listnode.h"

void runSort(ListNode*& front);
void quickSort(ListNode*& front);
void partition(ListNode*& front, ListNode*& lesser, ListNode*& equal, ListNode*& greater);
ListNode* concatenate(ListNode*& lesser, ListNode*& equal, ListNode*& greater);
