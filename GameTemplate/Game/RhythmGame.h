#pragma once
//リズムゲーム部分のクラス
class RhythmGame:public IGameObject
{
public:
	RhythmGame();
	~RhythmGame();

	bool Start();
	void Init();
	void Update();
	void Move();
	void Judgment(Vector3 nowPos);
	void Render(RenderContext& rc);
	SpriteRender m_singleNort;
	SpriteRender m_longNort;
	SpriteRender m_concurrentNort;
	//
	SpriteRender m_got;//動作確認ようスピらいて
	bool m_ok = false;
	SpriteRender m_lineSprite;
	//
	Vector3 m_nowNortPosR = Vector3::Zero;

};

