#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"


void nsK2EngineLow::RenderingEngine::Init()
{
	InitMainRenderTarget();

	//2D(フォントとスプライト)の初期化。
	Init2DSprite();

	//メインレンダリングターゲットのカラーバッファの内容を
	//フレームバッファにコピーするためのスプライトを初期化する
	InitFinalSprite();

	//ブルームの初期化。
	m_bloom.Init(m_mainRenderingTarget);

	//シャドウマップの初期化。
	m_shadow.Init();

	//ライトの初期化。
	m_light.Init();
}
void nsK2EngineLow::RenderingEngine::InitMainRenderTarget()
{
	//メインレンダリングターゲットの作成
	float clearColor[4] = { 0.5f,0.5f,0.5f,1.0f };
	m_mainRenderingTarget.Create(
		g_graphicsEngine->GetFrameBufferWidth(),
		g_graphicsEngine->GetFrameBufferHeight(),
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);
}

void nsK2EngineLow::RenderingEngine::Init2DSprite()
{
	float clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };

	//2D用のターゲットの初期化。
	m_2DRenderTarget.Create(
		m_mainRenderingTarget.GetWidth(),
		m_mainRenderingTarget.GetHeight(),
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_UNKNOWN,
		clearColor
	);

	//最終合成用のスプライトを初期化する。
	SpriteInitData spriteInitData;
	//テクスチャは2Dレンダーターゲット。
	spriteInitData.m_textures[0] = &m_2DRenderTarget.GetRenderTargetTexture();
	// 解像度はmainRenderTargetの幅と高さ
	spriteInitData.m_width = m_mainRenderingTarget.GetWidth();
	spriteInitData.m_height = m_mainRenderingTarget.GetHeight();
	// 2D用のシェーダーを使用する
	spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
	spriteInitData.m_vsEntryPointFunc = "VSMain";
	spriteInitData.m_psEntryPoinFunc = "PSMain";
	//上書き。
	spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;
	//レンダリングターゲットのフォーマット。
	spriteInitData.m_colorBufferFormat[0] = m_mainRenderingTarget.GetColorBufferFormat();

	m_2DSprite.Init(spriteInitData);

	//テクスチャはメインレンダ―ターゲット。
	spriteInitData.m_textures[0] = &m_mainRenderingTarget.GetRenderTargetTexture();

	//解像度は2Dレンダ―ターゲットの幅と高さ
	spriteInitData.m_width = m_2DRenderTarget.GetWidth();
	spriteInitData.m_height = m_2DRenderTarget.GetHeight();
	spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
	spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;

	//レンダリングターゲットのフォーマット。
	spriteInitData.m_colorBufferFormat[0] = m_2DRenderTarget.GetColorBufferFormat();

	m_mainSprite.Init(spriteInitData);
}

//メインレンダリングターゲットのカラーバッファの内容を
	//フレームバッファにコピーするためのスプライトを初期化する
void nsK2EngineLow::RenderingEngine::InitFinalSprite()
{
	SpriteInitData spriteInitData;
	spriteInitData.m_textures[0] = &m_mainRenderingTarget.GetRenderTargetTexture();
	spriteInitData.m_width = m_mainRenderingTarget.GetWidth();
	spriteInitData.m_height = m_mainRenderingTarget.GetHeight();
	spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
	m_copyToframeBufferSprite.Init(spriteInitData);
}

void nsK2EngineLow::RenderingEngine::Execute(RenderContext& rc)
{
	//ライトカメラの更新。
	m_light.LightCameraUpdate();

	////影。
	m_shadow.Execute(rc, m_renderObjects);

	if (isResultFlag == false) {
		//モデルの描画。
		ModelDraw(rc);
	}

	//エフェクトの描画。
	EffectEngine::GetInstance()->Draw();

	//ブルームの適用。
	m_bloom.Render(rc, m_mainRenderingTarget);

	//画像と文字の描画。
	SpriteFontDraw(rc);
	//リザルト画面なうなら。
	if (isResultFlag == true) {
		ModelDraw(rc);
	}
	//メインレンダリングターゲットの絵をフレームバッファにコピー。
	CopyMainRenderTargetToFrameBuffer(rc);

	//クリア。
	m_renderObjects.clear();
}

void nsK2EngineLow::RenderingEngine::ModelDraw(RenderContext& rc)
{
	// メインのターゲットが使えるようになるまで待つ
	rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderingTarget);
	// ターゲットセット
	rc.SetRenderTargetAndViewport(m_mainRenderingTarget);
	// ターゲットのクリア
	rc.ClearRenderTargetView(m_mainRenderingTarget);

	// まとめてモデルレンダーを描画
	for (auto MobjData : m_renderObjects) {
		MobjData->OnRenderModel(rc);
	}

	// 描画されるまで待つ
	rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderingTarget);
}

void nsK2EngineLow::RenderingEngine::SpriteFontDraw(RenderContext& rc)
{
	//2D用のターゲットが使えるようになるまで待つ。
	rc.WaitUntilToPossibleSetRenderTarget(m_2DRenderTarget);
	//ターゲットのセット。
	rc.SetRenderTargetAndViewport(m_2DRenderTarget);
	//ターゲットのクリア。
	rc.ClearRenderTargetView(m_2DRenderTarget);

	m_mainSprite.Draw(rc);

	//スプライトと文字の描画。
	for (auto SobjData : m_renderObjects) {
		SobjData->OnRender2D(rc);
	}

	// 描画されるまで待つ
	rc.WaitUntilFinishDrawingToRenderTarget(m_2DRenderTarget);
	
	//ターゲットをメインに戻す
	rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderingTarget);
	// レンダリングターゲットをセット
	rc.SetRenderTargetAndViewport(m_mainRenderingTarget);
	//2Dの描画。
	m_2DSprite.Draw(rc);
	// mainRenderingTargetの描画が終わるまで待つ
	rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderingTarget);
}

void nsK2EngineLow::RenderingEngine::CopyMainRenderTargetToFrameBuffer(RenderContext& rc)
{
	//フレームバッファにセット。
	rc.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	//ビューポートとシザリング短形の設定
	rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());
	
	m_copyToframeBufferSprite.Draw(rc);
}

