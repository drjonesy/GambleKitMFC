#pragma once

#include "Base\GkBase.h"
#include "Base\GkBaseReelMap.h"

#include "Game\GkGame.h"
#include "Game\GkGameEntity.h"
#include "Game\GkGamePage.h"
#include "Game\GkGameReelModel.h"
#include "Game\GkGameReelsCombinationGenerator.h"
#include "Game\GkGameTimer.h"
#include "Game\GkGameWinningPotentialsAnalyzer.h"
#include "Game\GkGameWinningsAnalyzer.h"
#include "Game\GkGameWinningsModel.h"

#include "Game\Entities\GkGameAnimation.h"
#include "Game\Entities\GkGameButton.h"
#include "Game\Entities\GkGameReels.h"
#include "Game\Entities\GkGameStreamingAnimation.h"
#include "Game\Entities\GkGameText.h"
#include "Game\Entities\GkGameWinningCounter.h"

#include "Game\Entities\Bindings\GkGameAnimationsSoundBinding.h"
#include "Game\Entities\Bindings\GkGameButtonsBinding.h"
#include "Game\Entities\Bindings\GkGameButtonsSoundBinding.h"
#include "Game\Entities\Bindings\GkGameTextsDateTimeBinding.h"
#include "Game\Entities\Bindings\GkGameTextsFormatBinding.h"
#include "Game\Entities\Bindings\GkGameTextsFormatIntBinding.h"
#include "Game\Entities\Bindings\GkGameTextsIntBinding.h"

#include "Game\Entities\Buttons\GkGameAnimationButton.h"
#include "Game\Entities\Buttons\GkGameCheckButton.h"
#include "Game\Entities\Buttons\GkGameClickButton.h"

#include "Game\ReelModels\GkGameReelRandomModel.h"
#include "Game\ReelModels\GkGameReelStripModel.h"

#include "Graphics\GkAnimatedFont.h"
#include "Graphics\GkAnimatedTexture.h"
#include "Graphics\GkGraphicsDevice.h"
#include "Graphics\GkImage.h"
#include "Graphics\GkTexture.h"

#include "OSSpecific\GkFileSystem.h"
#include "OSSpecific\GkOpenGLWindowsForm.h"
#include "OSSpecific\GkOSUtils.h"
#include "OSSpecific\GkWindowsForm.h"
#include "OSSpecific\GkWindowsMutex.h"
#include "OSSpecific\GkWindowsThread.h"

#include "Sound\GkSoundDevice.h"
#include "Sound\GkSoundSource.h"

#include "Utils\GkList.h"
#include "Utils\GkLogFile.h"
#include "Utils\GkMaths.h"
#include "Utils\GkStrings.h"