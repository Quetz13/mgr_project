#pragma once
#include <chrono>
#include <vector>
#include <iostream>
#include <thread>

#include "BlockingFIFO.h"
#include "NewFIFO.h"


#define PRODUCERS 1
#define CONSUMERS 10
#define ELEMENTS 10000




void NewFIFOQueue_produce(int num);
void NewFIFOQueue_consume(int num);

void BlockingFIFO_produce(int num);
void BlockingFIFO_consume(int num);



void testFIFO(int producers, int consumers, void produce(int), void consume(int));
void mainTests();