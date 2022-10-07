
#include "AppWindow.h"
#include <Windows.h>

#include "EngineTime.h"


struct vec3
{
	float x, y, z;
};

struct vertex
{
	vec3 position;
	vec3 position1;
	vec3 color;
	vec3 color1;
};


__declspec(align(16))
struct constant
{
	//float m_angle;
	float time;
};


AppWindow::AppWindow()
{
}


AppWindow::~AppWindow()
{
}

AppWindow* AppWindow::sharedInstance = nullptr;

AppWindow* AppWindow::getInstance()
{
	return sharedInstance;
}

void AppWindow::initialize()
{
	sharedInstance = new AppWindow();
}

void AppWindow::onCreate()
{
	Window::onCreate();
	GraphicsEngine::get()->init();
	m_swap_chain=GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	//vertex list[] = 
	//{
	//	//X - Y - Z
	//	{-0.6f,-0.75f,0.0f,    -0.32f,-0.11f,0.0f,  0.15f,0,0,  0,1,0 }, // POS1
	//	{-0.8f,0.5f,0.0f,     -0.11f,0.78f,0.0f,    1,0.85f,0,  1,1,0 }, // POS2
	//	{ 0.3f,-0.3f,0.0f,     0.75f,-0.73f,0.0f,   0,0,1,  1,0,0 },// POS2
	//	{ 0.2f,0.3f,0.0f,      0.88f,0.77f,0.0f,    1,1,1,  0,0,1 }
	//};

	vertex list[] =
	{
		//X - Y - Z
		{-0.85f,-0.9f,0.0f,    -0.32f,-0.11f,0.0f,  0.15f,0,0,  0,1,0 }, // POS1
		{-0.95f,0.4f,0.0f,     -0.11f,0.78f,0.0f,    1,0.85f,0,  1,1,0 }, // POS2
		{ 1.0f,-0.35f,0.0f,     0.0f,-0.73f,0.0f,	0,0,1,  1,0,0 },// POS2
		{-0.6f,-0.75f,0.0f,      0.88f,0.77f,0.0f,    1,1,1,  0,0,1 }
	};

	m_vb=GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	m_vs=GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();

	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	constant cc;
	cc.time = 0;

	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));

}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0.5f, 0.5f,0.5f, 1);
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	m_delta_time = static_cast<float>(EngineTime::getDeltaTime());

	m_speed_mod += m_delta_time;
	m_time += m_delta_time * (sinf(m_speed_mod) * 2.0f + 3.0f);

	m_time += m_delta_time;
	constant cc;
	cc.time = m_time;

	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);

	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);

	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertexList(), 0);
	m_swap_chain->present(true);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vb->release();
	m_swap_chain->release();
	m_vs->release();
	m_ps->release();
	GraphicsEngine::get()->release();
}
