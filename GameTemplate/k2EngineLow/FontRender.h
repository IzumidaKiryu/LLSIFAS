#pragma once
namespace nsK2EngineLow {
	class FontRender:public IRender
	{
	public:
		static const int MAX_TEXT_SIZE = 256;
		~FontRender()
		{
		}
		//テキストの設定。
		void SetText(const wchar_t* text)
		{
			swprintf_s(m_text, text);
		}
		//テキストを取得。
		const wchar_t* GetText() const
		{
			return m_text;
		}
		//ポジションの設定。
		void SetPosition(float x, float y, float z)
		{
			SetPosition({ x, y, z });
		}
	
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}
		// ポジションの取得。
		const Vector3& GetPosition() const
		{
			return m_position;
		}
		// スケールの設定。
		void SetScale(const float scale)
		{
			m_scale = scale;
		}
		// スケールの取得。
		const float GetScale() const
		{
			return m_scale;
		}
		// 色の設定。
		void SetColor(float r, float g, float b, float a)
		{
			SetColor({ r, g, b, a });
		}
		void SetColor(const Vector4& color)
		{
			m_color = color;
		}
		// 色の取得。
		const Vector4& GetColor() const
		{
			return m_color;
		}
		// 回転の設定。
		void SetRotation(const float rotation)
		{
			m_rotation = rotation;
		}
		// 回転の取得。
		const float GetRotation() const
		{
			return m_rotation;
		}
		// ピボットの設定。
		void SetPivot(float x, float y)
		{
			SetPivot({ x, y });
		}
		void SetPivot(const Vector2& pivot)
		{
			m_pivot = pivot;
		}
		// ピボットの取得。
		const Vector2& GetPivot() const
		{
			return m_pivot;
		}
		//影を描画するかどうかの設定。
		void SetisDrawShadow(bool isDrawShadow)
		{
			m_font.SetIsDrawShadow(isDrawShadow);
		}
		//描画。
		void Draw(RenderContext& rc);

		
		void SetShadowParam(bool isDrawShadow, float shadowOffset, const Vector4& shadowColor)
		{
			m_font.SetShadowParam(isDrawShadow, shadowOffset, shadowColor);
		}

	private:
		void OnRender2D(RenderContext& rc) override
		{
			if (m_text == nullptr)
			{
				return;
			}
			m_font.Begin(rc);
			m_font.Draw(m_text, Vector2(m_position.x, m_position.y), m_color, m_rotation, m_scale, m_pivot);
			m_font.End(rc);
		}
		Vector3							m_position = Vector3::Zero;				
		float							m_scale = 1.0f;						
		Vector4							m_color = g_vec4White;				
		float							m_rotation = 0.0f;					
		Vector2							m_pivot = Sprite::DEFAULT_PIVOT;	
		wchar_t							m_text[MAX_TEXT_SIZE];				
		Font							m_font;								
	};
}
