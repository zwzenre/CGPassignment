    #include "Header/InputManager.h"

    InputManager::InputManager()
        : hWnd(nullptr), m_d3dDevice(nullptr), dInput(nullptr),
          dInputKeyboardDevice(nullptr), dInputMouseDevice(nullptr),
          m_cursor(nullptr), m_initialized(false),
          screenWidth(1280), screenHeight(720) {
        ZeroMemory(diKeys, sizeof(diKeys));
        ZeroMemory(prevDiKeys, sizeof(prevDiKeys));
        ZeroMemory(&mouseState, sizeof(mouseState));
        ZeroMemory(&prevMouseState, sizeof(prevMouseState));
    }

    InputManager::~InputManager() {
        Quit();
    }

    bool InputManager::Initialize(HWND hWnd, LPDIRECT3DDEVICE9 d3dDevice, int screenW, int screenH) {
        this->hWnd = hWnd;
        this->m_d3dDevice = d3dDevice;
        this->screenWidth = screenW;
        this->screenHeight = screenH;

        hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
                              IID_IDirectInput8, (void**)&dInput, NULL);
        if (FAILED(hr)) return false;

        // Keyboard
        hr = dInput->CreateDevice(GUID_SysKeyboard, &dInputKeyboardDevice, NULL);
        if (FAILED(hr)) return false;
        dInputKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
        dInputKeyboardDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
        dInputKeyboardDevice->Acquire();

        // Mouse
        hr = dInput->CreateDevice(GUID_SysMouse, &dInputMouseDevice, NULL);
        if (FAILED(hr)) return false;
        dInputMouseDevice->SetDataFormat(&c_dfDIMouse);
        dInputMouseDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
        dInputMouseDevice->Acquire();

        if (m_d3dDevice) {
            if (!CreateCursor("assets/cursor.png")) {
                MessageBox(NULL, "Failed to load cursor texture (assets/cursor.png)",
                           "Error", MB_OK | MB_ICONERROR);
            }
        }

        ShowCursor(FALSE);

        m_initialized = true;
        return true;
    }

void InputManager::Update() {
        // Store previous states
        memcpy(prevDiKeys, diKeys, sizeof(diKeys));
        prevMouseState = mouseState;

        // Update keyboard
        if (dInputKeyboardDevice) {
            hr = dInputKeyboardDevice->Acquire();
            if (SUCCEEDED(hr)) {
                hr = dInputKeyboardDevice->GetDeviceState(sizeof(diKeys), diKeys);
                if (FAILED(hr)) {
                    // If device lost, try to reacquire
                    dInputKeyboardDevice->Acquire();
                    ZeroMemory(diKeys, sizeof(diKeys));
                }
            }
        }

        // Update mouse
        if (dInputMouseDevice) {
            hr = dInputMouseDevice->Acquire();
            if (SUCCEEDED(hr)) {
                hr = dInputMouseDevice->GetDeviceState(sizeof(mouseState), &mouseState);
                if (FAILED(hr)) {
                    // If device lost, try to reacquire
                    dInputMouseDevice->Acquire();
                    ZeroMemory(&mouseState, sizeof(mouseState));
                }
            }
        }

        // Update cursor with DirectInput movement deltas
        if (m_cursor && m_initialized) {
            // Only update if we have valid mouse deltas
            if (mouseState.lX != 0 || mouseState.lY != 0) {
                m_cursor->UpdateDelta(mouseState.lX, mouseState.lY, screenWidth, screenHeight);
            }
        }

        // Debug output (remove in final version)
        static int debugCounter = 0;
        if (++debugCounter % 60 == 0 && m_cursor) { // Every ~60 frames
            D3DXVECTOR2 pos = m_cursor->GetPosition();
            char buffer[256];
            sprintf_s(buffer, "Cursor pos: (%.1f, %.1f), Mouse delta: (%d, %d)\n",
                     pos.x, pos.y, mouseState.lX, mouseState.lY);
            OutputDebugStringA(buffer);
        }
    }

    void InputManager::Render(LPD3DXSPRITE sprite) {
        if (m_cursor) {
            m_cursor->Render(sprite);
        }
    }

    void InputManager::Quit() {
        if (m_cursor) {
            m_cursor->Shutdown();
            delete m_cursor;
            m_cursor = nullptr;
        }
        if (dInputKeyboardDevice) {
            dInputKeyboardDevice->Unacquire();
            dInputKeyboardDevice->Release();
            dInputKeyboardDevice = nullptr;
        }
        if (dInputMouseDevice) {
            dInputMouseDevice->Unacquire();
            dInputMouseDevice->Release();
            dInputMouseDevice = nullptr;
        }
        if (dInput) {
            dInput->Release();
            dInput = nullptr;
        }
         m_initialized = false;
    }

    bool InputManager::IsKeyDown(int dik) const {
        return (diKeys[dik] & 0x80) != 0;
    }

    bool InputManager::IsMouseButtonDown(int button) const {
        return (mouseState.rgbButtons[button] & 0x80) != 0;
    }

    bool InputManager::IsMouseButtonPressed(int button) {
        return ((mouseState.rgbButtons[button] & 0x80) != 0) &&
               ((prevMouseState.rgbButtons[button] & 0x80) == 0);
    }

    bool InputManager::CreateCursor(const char* texturePath) {
        if (!m_d3dDevice) return false;

        if (m_cursor) {
            m_cursor->Shutdown();
            delete m_cursor;
        }

        m_cursor = new Cursor();
        if (!m_cursor->Initialize(m_d3dDevice, texturePath)) {
            delete m_cursor;
            m_cursor = nullptr;
            return false;
        }

        return true;
    }

    void InputManager::SetCursorVisible(bool visible) {
        if (m_cursor) {
            m_cursor->SetVisible(visible);
        }
    }

    bool InputManager::IsCursorVisible() const {
        return m_cursor ? m_cursor->IsVisible() : false;
    }