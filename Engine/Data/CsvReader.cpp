#include "CsvReader.h"
#include <Windows.h>
#include <fstream>

CsvReader::CsvReader(std::string fileName)
{
	all.clear();
	std::ifstream ifs(fileName);
	if (!ifs)
	{
		MessageBox(NULL, L"CsvReader CsvReader()：ファイルが開けませんでした", L"エラー", MB_OK);
		return;
	}

	// BOM[,][\n][\r]をはじく
	unsigned char BOMS[] = { 0xEF, 0xBB, 0xBF };
	bool found = true;
	for (int i = 0; i < 3; i++)
	{
		if (ifs.get() != BOMS[i])
		{
			found = false;
			break;
		}
	}
	//BOMがない場合、ファイルの先頭の位置に巻き戻しして戻す
	if (found == false)
	{
		ifs.seekg(std::ios_base::beg);
	}

	// データを読む
	std::string lineString;
	while (getline(ifs, lineString))
	{
		while (true)
		{
			int dq = 0;
			for (int i = 0; i < lineString.size(); i++)
			{
				if (lineString[i] == '"')
				{
					dq += 1;
				}
			}
			if (dq % 2 == 0)
			{
				break;
			}
			std::string s;
			getline(ifs, s);
			lineString += "\n" + s;
		}
		for (auto it = lineString.begin(); it != lineString.end();)
		{
			if (*it == '"')
			{
				it = lineString.erase(it);
			}
			if (it != lineString.end())
			{
				it += 1;
			}
		}

		// 行内を[,]で区切る
		LineRecord lineRecord;
		int top = 0;
		bool indq = false;
		for (int n = 0; n < lineString.size(); n++)
		{
			if (lineString[n] == ',')
			{
				if (!indq)
				{
					lineRecord.record.emplace_back(lineString.substr(top, (size_t)(n - top)));
					top = n + 1;
				}
			}
			else if (lineString[n] == '"')
			{
				indq = !indq;
			}
		}
		lineRecord.record.emplace_back(lineString.substr(top, lineString.size() - top));
		all.emplace_back(lineRecord);
	}
	ifs.close();
}

CsvReader::~CsvReader()
{
	for (auto record : all)
	{
		record.record.clear();
	}
	all.clear();
}

int CsvReader::GetLines()
{
	return (int)all.size();
}

int CsvReader::GetColumns(int line)
{
	return (int)all[line].record.size();
}

std::string CsvReader::GetString(int line, int column)
{
	if (column >= GetColumns(line))
	{
		return "";
	}
	return all[line].record[column];
}

int CsvReader::GetInt(int line, int column)
{
	std::string str = GetString(line, column);
	if (str == "")
	{
		return 0;
	}
	return std::stoi(str);
}

float CsvReader::GetFloat(int line, int column)
{
	std::string str = GetString(line, column);
	if (str == "")
	{
		return 0.0f;
	}
	return std::stof(str);
}
