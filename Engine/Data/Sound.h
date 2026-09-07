#pragma once
#include <string>

/// <summary>
/// wavに対応しています　※mp3は不可
/// 下のページなどで.wavに変換してください
/// https://convertio.co/ja/mp3-wav/
/// </summary>
namespace Sound
{
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// ファイルを読み込む
	/// </summary>
	/// <param name="name">読み込むファイルの名前</param>
	/// <returns>成功したら0が返る</returns>
	int Load(const std::string& name);

	/// <summary>
	/// 音を再生する
	/// </summary>
	/// <param name="name">再生するファイルの名前</param>
	/// <param name="isLoop">trueならループ再生</param>
	/// <returns>成功したら0が返る</returns>
	int Play(const std::string& name, bool isLoop, bool isSE);

	/// <summary>
	/// 音の再生を止める
	/// </summary>
	/// <param name="name">再生を止めるファイルの名前</param>
	/// <returns>成功したら0が返る</returns>
	int Stop(const std::string& name);

	/// <summary>
	/// 音量を変更する
	/// </summary>
	/// <param name="name">音量を変える音の名前</param>
	/// <param name="volume">音量( 0.0f～1.0f )</param>
	/// <returns>成功したら0が返る</returns>
	int ChangeVolume(const std::string& name, float volume);

	/// <summary>
	///  解放処理
	/// </summary>
	void Release();
}
