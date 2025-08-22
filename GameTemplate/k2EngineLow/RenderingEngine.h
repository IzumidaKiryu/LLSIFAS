#pragma once
#include "Bloom.h"
#include "Light.h"
#include "IRender.h"
#include "Shadow.h"

namespace nsK2EngineLow {

	class RenderingEngine :public Noncopyable
	{
	public:
		//初期化。
		void Init();
		//描画処理の実行。
		void Execute(RenderContext& rc);
		// レンダリングオブジェクトを追加する関数
		void AddRenderObject(IRender* renderObject)
		{
			m_renderObjects.push_back(renderObject);
		}
		//ディレクションライトの設定。
		void SetDirectionalLight(Vector3 direction,Vector3 color)
		{
			m_light.SetDirectionLight(direction,color);
		}

		//環境光の設定。
		void SetAmbientLight(Vector3 ambient)
		{
			m_light.SetAmbient(ambient);
		}

		//ポイントライトの設定
		void SetPointLight(int num, Vector3 position, Vector3 color, float range)
		{
			m_light.SetPointLight(num, position, color, range);
		}

		//スポットライトの設定。
		void SetSpotLight(int num, Vector3 position, Vector3 color, float range, Vector3 direction, float angle)
		{
			m_light.SetSpotLight(num, position, color, range, direction, angle);
		}
		//半球ライトの設定。
		void SetHemLight(Vector3 groundColor, Vector3 skyColor, Vector3 groundNormal)
		{
			m_light.SetHemLight(groundColor, skyColor, groundNormal);
		}

		void SetLightCameraTarget(Vector3 target)
		{
			m_light.SetLightCameraTarget(target);
		}

		RenderTarget& GetShadow()
		{
			return m_shadow.GetShadowTarget();
		}
		SceneLight& GetLight()
		{
			return m_light.GetLight();
		}

		Camera& GetLightCamera()
		{
			return m_light.GetLightCamera();
		}
		bool isResultFlag = false;//リザルト画面かどうか。
	private:
		void InitMainRenderTarget();
		void Init2DSprite();
		void InitFinalSprite();
		//モデルの描画。
		void ModelDraw(RenderContext& rc);
		void SpriteFontDraw(RenderContext& rc);
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);
		
		Light m_light;
		Bloom m_bloom;
		Shadow m_shadow;
		
		RenderTarget m_mainRenderingTarget;
		RenderTarget m_2DRenderTarget;
		Sprite m_mainSprite;
		Sprite m_2DSprite;
		Sprite m_copyToframeBufferSprite;
			
		std::vector<IRender*> m_renderObjects;	// すべてのレンダリングオブジェクトを格納するリスト

	};
}

