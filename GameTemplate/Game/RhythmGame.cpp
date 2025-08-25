#include "stdafx.h"
#include "RhythmGame.h"

namespace {
	const Vector3 FARSTNORTPOS_R = Vector3(120.0f, 600.0f,0.0f);
}

RhythmGame::RhythmGame() {

}
RhythmGame::~RhythmGame() {

}

bool RhythmGame::Start() {
	return true;
}

void RhythmGame::Init() {
	m_singleNort.Init("Assets/sprite/Nozomi.DDS", 60.0f, 60.0f);
	m_nowNortPosR = FARSTNORTPOS_R;
	m_singleNort.SetPosition(m_nowNortPosR);
	m_got.Init("Assets/sprite/Nozomi.DDS", 1920.0f, 1080.0f);
	m_lineSprite.Init("Assets/sprite/yattabe.DDS", 60.0f, 60.0f);
	m_lineSprite.SetPosition(Vector3(120.0f, 100.0f, 0.0f));
}

void RhythmGame::Update() {
	Move();
	Judgment(m_nowNortPosR);
	m_lineSprite.Update();
}

void RhythmGame::Move()
{
	m_nowNortPosR.y -= 10.0f;
	m_singleNort.SetPosition(m_nowNortPosR);
	if (m_nowNortPosR.y < -600.0f) {
		m_nowNortPosR.y = 600.0f;
		m_ok = false;
	}
	m_singleNort.Update();
}


void RhythmGame::Judgment(Vector3 nowPos)//簡易版判定プログラム
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		if (m_nowNortPosR.y <= 100.0f && m_nowNortPosR.y > 80.0f) {//パフェ
			m_ok = true;
		}
	}
}

void RhythmGame::Render(RenderContext& rc) {
	m_singleNort.Draw(rc);
	m_lineSprite.Draw(rc);
	if (m_ok == true) {
		m_got.Draw(rc);
	}
}