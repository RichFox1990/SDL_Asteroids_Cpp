#include "HighScore.h"
#include <string>


HighScore::HighScore()
{
	//int one, two, three;
	input.open(path);
	for (int i = 0; i < 3; i++)
	{
		input >> top3[i].first >> top3[i].second;
	}
	input.close();
}

void HighScore::PushScore(std::string name, const int score)
{
	name = name + ":";
	if (score > top3[0].second)
	{
		top3[1] = top3[2];
		top3[0] = top3[1];
		top3[0].first = name;
		top3[0].second = score;
	}
	else if (score > top3[1].second)
	{
		top3[1] = top3[2];
		top3[1].first = name;
		top3[1].second = score;
	}
	else
	{
		top3[2].first = name;
		top3[2].second = score;
	}
	remove(path.c_str());
	output.open(path);
	for (auto line : top3)
	{
		output << line.first << " " << line.second << "\n";
	}
	output.close();
}

void HighScore::Print()
{
	std::cout << "||||||||||||||||||";
	input.open(path);
	for (auto line : top3)
	{
		std::cout << "\n" << line.first << " " << line.second;
	}
	input.close();
	std::cout << "\n||||||||||||||||||";
}