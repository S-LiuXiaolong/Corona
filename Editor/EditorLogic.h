#pragma once
#include "IGameLogic.h"
#include "geommath.h"

namespace Corona {
    class EditorLogic : implements IGameLogic
    {
    // overrides
        int Initialize() final;
        void Finalize() final;
        void Tick() final;
        
        void OnLeftKeyDown() final;
        void OnRightKeyDown() final;
        void OnUpKeyDown() final;
        void OnDownKeyDown() final;

        void OnAnalogStick(int id, float deltaX, float deltaY) final;
#ifdef _DEBUG
        void DrawDebugInfo() final;
#endif
    };
}