#pragma once
#include <chrono>
#include <vector>
#include <iostream>
#include <thread>

#include "BlockingFIFO.h"
#include "NewFIFO.h"


#define PRODUCERS 100
#define CONSUMERS 100
#define ELEMENTS 1000


void print(const std::string& text);

void NewFIFOQueue_produce(int num);
void NewFIFOQueue_consume(int num);

void BlockingFIFO_produce(int num);
void BlockingFIFO_consume(int num);



void testFIFO(int producers, int consumers, void produce(int), void consume(int));
void mainTests();