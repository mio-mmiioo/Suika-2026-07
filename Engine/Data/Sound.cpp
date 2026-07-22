#include "Sound.h"

#include <Audio.h>
#include <memory>
#include <unordered_map>
#include <filesystem>

namespace Sound
{
	float ClampVolume(float volume);

	std::unique_ptr<DirectX::AudioEngine> audioEngine;
	std::unordered_map<std::string, std::unique_ptr<DirectX::SoundEffect>> sounds;
	std::unordered_map<std::string, std::unique_ptr<DirectX::SoundEffectInstance>> activeInstances;
}

void Sound::Initialize()
{
	audioEngine = std::make_unique<DirectX::AudioEngine>();
}

void Sound::Update()
{
	if (audioEngine != nullptr)
	{
		audioEngine->Update();
	}

	for (auto it = activeInstances.begin(); it != activeInstances.end();)
	{
		if (it->second->GetState() == DirectX::SoundState::STOPPED)
		{
			it = activeInstances.erase(it);
		}
		else
		{
			it++;
		}
	}
}

int Sound::Load(const std::string& name)
{
	if (audioEngine == nullptr)
	{
		return -1;
	}
	std::filesystem::path path = "Assets/sound/" + name + ".wav";
	std::wstring wPath = path.wstring();
	sounds[name] = std::make_unique<DirectX::SoundEffect>(audioEngine.get(), wPath.c_str());
	return 0;
}

int Sound::Play(const std::string& name, bool isLoop)
{
	auto it = sounds.find(name);
	// 見つからなかった
	if (it == sounds.end())
	{
		return -1;
	}

	// すでに同じ名前の音が再生中の場合、一度止めて破棄する
	// 何体も敵を表示するのには向かない書き方
	auto instanceIt = activeInstances.find(name);
	if (instanceIt != activeInstances.end())
	{
		instanceIt->second->Stop(true);
		activeInstances.erase(instanceIt);
	}

	// インスタンスを作成して再生
	auto instance = it->second->CreateInstance();
	if (instance != nullptr)
	{
		instance->SetVolume(1.0f);
		instance->Play(isLoop);
		activeInstances[name] = std::move(instance);
	}
	return 0;
}

int Sound::Stop(const std::string& name)
{
	auto it = activeInstances.find(name);
	// 再生中のnameの音源が見つからない
	if (it == activeInstances.end())
	{
		return -1;
	}
	it->second->Stop(true); // 音を停止
	activeInstances.erase(it); // 再生中のリストからも削除する
	return 0;
}

int Sound::ChangeVolume(const std::string& name, float volume)
{
	// ボリュームを変更する処理
	auto it = activeInstances.find(name);
	// 音が再生されていない or 見つからない場合
	if (it == activeInstances.end())
	{
		return -1;
	}
	it->second->SetVolume(ClampVolume(volume));
	return 0;
}

void Sound::Release()
{
	// 再生中の音を停止してから解放する
	for (auto& active : activeInstances)
	{
		if (active.second != nullptr)
		{
			active.second->Stop(true);
		}
	}
	// スマートポインター( unique_ptr )で作成されたものは、clearすると自動でメモリも解放される
	activeInstances.clear();
	sounds.clear();
	if (audioEngine != nullptr)
	{
		audioEngine.reset();
	}
}

float Sound::ClampVolume(float volume)
{
	if (volume < 0.0f)
	{
		return 0.0f;
	}
	if (volume > 1.0f)
	{
		return 1.0f;
	}
	return volume;
}