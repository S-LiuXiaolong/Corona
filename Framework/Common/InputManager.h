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

			void IKeyDown();
			void IKeyUp();
			void KKeyDown();
			void KKeyUp();
			void JKeyDown();
			void JKeyUp();
			void LKeyDown();
			void LKeyUp();

    };

    extern InputManager* g_pInputManager;
}