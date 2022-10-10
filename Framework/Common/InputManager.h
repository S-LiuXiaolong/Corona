#pragma once
#include "IRuntimeModule.h"

namespace Corona
{
    class InputManager : implements IRuntimeModule
    {
        public:
            virtual int Initialize();
            virtual void Finalize();
            virtual void Tick();

            void UpArrowKeyDown();
            void UpArrowKeyUp();
            void DownArrowKeyDown();
            void DownArrowKeyUp();
            void LeftArrowKeyDown();
            void LeftArrowKeyUp();
            void RightArrowKeyDown();
            void RightArrowKeyUp();

            // TODO: very dumb way
            void WKeyDown();
            void WKeyUp();
            void SKeyDown();
            void SKeyUp();
            void AKeyDown();
            void AKeyUp();
            void DKeyDown();
            void DKeyUp();
            void QKeyDown();
            void QKeyUp();
			void EKeyDown();
			void EKeyUp();

			void IKeyDown();
			void IKeyUp();
			void KKeyDown();
			void KKeyUp();
			void JKeyDown();
			void JKeyUp();
			void LKeyDown();
			void LKeyUp();

			void OnMouseDown(int x, int y);
			void OnMouseMoveL(int x, int y);
			void OnMouseMoveR(int x, int y);

            void OnMouseWheel(int delta);

    };

    extern InputManager* g_pInputManager;
}