#pragma once
#include <vector>
#include <string>

class CsvReader
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="fileName">読み込みたいファイルのパス</param>
	CsvReader(std::string fileName);
	~CsvReader();

	/// <summary>
	/// 読み込んだデータの行数を取得
	/// </summary>
	/// <returns>行数</returns>
	int GetLines();

	/// <summary>
	/// カラム数を取得
	/// </summary>
	/// <param name="line">行数</param>
	/// <returns>カラム数</returns>
	int GetColumns(int line);

	/// <summary>
	/// 指定した行・列のデータを文字列で取得
	/// </summary>
	/// <param name="line">行数</param>
	/// <param name="column">カラム数</param>
	/// <returns>文字列</returns>
	std::string GetString(int line, int column);

	/// <summary>
	/// 指定した行・列のデータをint型で取得
	/// </summary>
	/// <param name="line">行数</param>
	/// <param name="column">カラム数</param>
	/// <returns>int型</returns>
	int GetInt(int line, int column);

	/// <summary>
	/// 指定した行・列のデータをfloat型で取得
	/// </summary>
	/// <param name="line">行数</param>
	/// <param name="column">カラム数</param>
	/// <returns>float型</returns>
	float GetFloat(int line, int column);

private:
	struct LineRecord
	{
		std::vector<std::string> record; // １行分のデータを入れる配列
	};
	std::vector<LineRecord> all; // すべての行の配列
};
