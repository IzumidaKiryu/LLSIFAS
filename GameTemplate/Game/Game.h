#pragma once
class GameCamera;
class RhythmGame;
class Game:public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void NewGOStartGameObject();//ゲーム開始時にオブジェクトを生成するときに使う関数。


	SkyCube* m_skyCube = nullptr;
	GameCamera* m_camera = nullptr;
	RhythmGame* m_rhythmGame = nullptr;
};

