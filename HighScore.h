#pragma once
#include <iostream>
#include <fstream>

class HighScore
{
public:
	std::ifstream input{};
	std::ofstream output{};

	std::string path = "High_Score/High_Score.txt";
	std::pair<std::string, int> top3[3];

	void PushScore(std::string name, int score);
	void Print();
	HighScore();

};