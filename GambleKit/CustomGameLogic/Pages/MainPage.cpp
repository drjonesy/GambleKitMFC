#include "MainPage.h"

#include "..\..\Engine\GambleKit.h"


MainPage::MainPage(CustomGame *game)
	: GkGamePage()
{
	// Initialize the variables
	
	this->bet = 1;
	this->lines = 1;
	this->winning = 0;
	this->credits = 100;

	// Add the background
	this->AddEntity(new GkGameAnimation(game->AcquireAnimatedTexture(L"Background.png")));
	
	// Create the Bet All , + 1, Lines and Start buttons
	//new GkGameAnimation(game->AcquireAnimatedTexture(L"btn_bet_default.png"), xPos, xPos),
	this->btnBetAll = (GkGameClickButton*)this->AddEntity(new GkGameClickButton(
		new GkGameAnimation(game->AcquireAnimatedTexture(L"btn_betAll_default.png"), 250, 528),
		new GkGameAnimation(game->AcquireAnimatedTexture(L"btn_betAll_down.png"), 250, 528),
		new GkGameAnimation(game->AcquireAnimatedTexture(L"btn_betAll_disabled.png"), 250, 528)));
	this->btnBet = (GkGameClickButton*)this->AddEntity(new GkGameClickButton(
		new GkGameAnimation(game->AcquireAnimatedTexture(L"btn_bet_default.png"), 380, 528),
		new GkGameAnimation(game->AcquireAnimatedTexture(L"btn_bet_down.png"), 380, 528),
		new GkGameAnimation(game->AcquireAnimatedTexture(L"btn_bet_disabled.png"), 380, 528)));
	this->btnLines = (GkGameClickButton*)this->AddEntity(new GkGameClickButton(
		new GkGameAnimation(game->AcquireAnimatedTexture(L"btn_lines_default.png"), 524, 528),
		new GkGameAnimation(game->AcquireAnimatedTexture(L"btn_lines_down.png"), 524, 528),
		new GkGameAnimation(game->AcquireAnimatedTexture(L"btn_lines_disabled.png"), 524, 528)));
	this->btnStart = (GkGameClickButton*)this->AddEntity(new GkGameClickButton(
		new GkGameAnimation(game->AcquireAnimatedTexture(L"btn_start_default.png"), 687, 450),
		new GkGameAnimation(game->AcquireAnimatedTexture(L"btn_start_hidden.png"), 687, 450),
		new GkGameAnimation(game->AcquireAnimatedTexture(L"btn_start_hidden.png"), 687, 450)));
	// new section --- Stop button
    this->btnStop = (GkGameClickButton*)this->AddEntity(new GkGameClickButton(
		new GkGameAnimation(game->AcquireAnimatedTexture(L"btn_stop_default.png"), 687, 520),
		new GkGameAnimation(game->AcquireAnimatedTexture(L"btn_stop_hidden.png"), 687, 520),
		new GkGameAnimation(game->AcquireAnimatedTexture(L"btn_stop_hidden.png"), 687, 520))); 

	// Create a special counter for the credits, to make them appear to increase gradually for better visual experience
	GkGameWinningCounter *creditsCounter = (GkGameWinningCounter*)this->AddEntity(new GkGameWinningCounter(&this->credits, 500.0f, 2.0f));

	// Create the Credits, Winning, Bet and Lines text counters and bind them to their respective values. Note that credits is bound to  the "creditsCounter"
	this->AddEntity(new GkGameTextsIntBinding(&creditsCounter->intDisplayValue, 1, this->AddEntity(new GkGameText(game->AcquireAnimatedFontFromMultipleFiles(L"font_main_{0}.png", L"0123456789,.-"), 55, 496, -3, GkGameText::GameTextAlignment_Center))));
	this->AddEntity(new GkGameTextsIntBinding(&this->winning, 1, this->AddEntity(new GkGameText(game->AcquireAnimatedFontFromMultipleFiles(L"font_main_{0}.png", L"0123456789,.-"), 180, 496, -3, GkGameText::GameTextAlignment_Center))));
	this->AddEntity(new GkGameTextsIntBinding(&this->bet, 1, this->AddEntity(new GkGameText(game->AcquireAnimatedFontFromMultipleFiles(L"font_main_{0}.png", L"0123456789,.-"), 380, 496, -3, GkGameText::GameTextAlignment_Center))));
	this->AddEntity(new GkGameTextsIntBinding(&this->lines, 1, this->AddEntity(new GkGameText(game->AcquireAnimatedFontFromMultipleFiles(L"font_main_{0}.png", L"0123456789,.-"), 590, 496, -3, GkGameText::GameTextAlignment_Center))));

	// Create the combinations generator. The combinations generator represents a model of collection of reels. Each of its reels can have a different model
	// The GkGameReelStripModel is a GkGameReelModel, representing a reel with a fixed sequence of finite number of items. Here, it is used for all the reels of the combination generator.
	this->combinationsGenerator = new GkGameReelsCombinationGenerator(5, 3);
	this->combinationsGenerator->SetReelModel(0, new GkGameReelStripModel(3, 6, ReelItem_Cherry, ReelItem_Melon, ReelItem_Cherry, ReelItem_Bell, ReelItem_Cherry, ReelItem_Melon));
	this->combinationsGenerator->SetReelModel(1, new GkGameReelStripModel(3, 6, ReelItem_Bell, ReelItem_Melon, ReelItem_Bell, ReelItem_Melon, ReelItem_Melon, ReelItem_Cherry));
	this->combinationsGenerator->SetReelModel(2, new GkGameReelStripModel(3, 6, ReelItem_Cherry, ReelItem_7, ReelItem_Melon, ReelItem_Cherry, ReelItem_7, ReelItem_Bell));
	this->combinationsGenerator->SetReelModel(3, new GkGameReelStripModel(3, 6, ReelItem_Cherry, ReelItem_Bell, ReelItem_Melon, ReelItem_Bell, ReelItem_Cherry, ReelItem_Bell));
	this->combinationsGenerator->SetReelModel(4, new GkGameReelStripModel(3, 6, ReelItem_Cherry, ReelItem_Melon, ReelItem_Bell, ReelItem_Cherry, ReelItem_Melon, ReelItem_Bell));

	// Create the visual object for the reels
	this->reels = (GkGameReels*)this->AddEntity(new GkGameReels(5, 3, 4, 670, 331));
	this->reels->symbols[ReelItem_Cherry] = game->AcquireAnimatedTexture(L"symbol_cherry.png");
	this->reels->symbols[ReelItem_Melon	] = game->AcquireAnimatedTexture(L"symbol_melon.png");
	this->reels->symbols[ReelItem_Bell	] = game->AcquireAnimatedTexture(L"symbol_bell.png");
	this->reels->symbols[ReelItem_7		] = game->AcquireAnimatedTexture(L"symbol_7.png");
	this->reels->x = 62;
	this->reels->y = 90;
	this->reels->ApplyReelMap();// (this->game->settings->playerSettings->currentMap);
	this->reels->SetItemGenerator(this->combinationsGenerator);

	// Load the images used to show a winning line
	this->lineAnimations[0] = (GkGameAnimation*)this->AddEntity(new GkGameAnimation(game->AcquireAnimatedTexture(L"line123.png"), 112, 255));
	this->lineAnimations[1] = (GkGameAnimation*)this->AddEntity(new GkGameAnimation(game->AcquireAnimatedTexture(L"line123.png"), 112, 139));
	this->lineAnimations[2] = (GkGameAnimation*)this->AddEntity(new GkGameAnimation(game->AcquireAnimatedTexture(L"line123.png"), 112, 371));
	this->lineAnimations[3] = (GkGameAnimation*)this->AddEntity(new GkGameAnimation(game->AcquireAnimatedTexture(L"line45.png"), 112, 139));
	this->lineAnimations[4] = (GkGameAnimation*)this->AddEntity(new GkGameAnimation(game->AcquireAnimatedTexture(L"line45.png"), 112, 139, true, true, true, false, true));

	// The winnings model defines two things. One is the properties of the symbols - their coefficients and special behaviour. Second - the shape of the winning lines.
	this->winningsModel = new GkGameWinningsModel(5, 3, 5, 4, 0);
	this->winningsModel->SetupSymbol(ReelItem_Cherry, 0, GkGameWinningsModel::SymbolFlag_Default,		GkGameWinningsModel::SymbolFlag_None,	0, 0, 10, 20, 80);
	this->winningsModel->SetupSymbol(ReelItem_Melon	, 0, GkGameWinningsModel::SymbolFlag_Default,		GkGameWinningsModel::SymbolFlag_None,	0, 0, 20, 40, 200);
	this->winningsModel->SetupSymbol(ReelItem_Bell	, 0, GkGameWinningsModel::SymbolFlag_Default,		GkGameWinningsModel::SymbolFlag_None,	0, 0, 50, 100, 500);
	this->winningsModel->SetupSymbol(ReelItem_7		, 0, GkGameWinningsModel::SymbolFlag_Wild,			GkGameWinningsModel::SymbolFlag_None,	0, 0, 0, 0, 0);
	this->winningsModel->SetupLine(0, 1, 1, 1, 1, 1);
	this->winningsModel->SetupLine(1, 0, 0, 0, 0, 0);
	this->winningsModel->SetupLine(2, 2, 2, 2, 2, 2);
	this->winningsModel->SetupLine(3, 0, 1, 2, 1, 0);
	this->winningsModel->SetupLine(4, 2, 1, 0, 1, 2);

	// The winnings analyzer is used to analyze a combination for winnings, based on a winning model. After an analisys, it holds all the winnings, if any, and detailed information on them.
	this->winningsAnalyzer = new GkGameWinningsAnalyzer(this->winningsModel);

	this->sndReels = game->AcquireSoundSource(L"reels.wav");
	this->sndReelStop = game->AcquireSoundSource(L"reel.wav");
}


MainPage::~MainPage(void)
{
	delete this->combinationsGenerator;
	delete this->winningsAnalyzer;
	delete this->winningsModel;
}

void MainPage::Process(GkGame *game)
{
	if (this->reels->state == Idle)
	{
		// Start button - hide the lines, generate a random combination, set it to appear on the reels after spinning, analyze its winning and start the reels
		if (this->btnStart->IsClicked())
		{
			for (int lineIndex = 0; lineIndex < this->winningsModel->numLines; lineIndex++)
				this->lineAnimations[lineIndex]->visible = false;

			this->credits -= this->bet * this->lines;

			this->combinationsGenerator->GenerateCombination();
			this->reels->SetItems(this->combinationsGenerator);
			this->winningsAnalyzer->AnalyzeWinnings(this->combinationsGenerator);
			this->winningsAnalyzer->MultiplyWinnings(this->bet);

			this->reels->Start();

			game->soundDevice->PlaySourceLooping(this->sndReels);
		}

		// Bet button - change the bet 
		if (this->btnBet->IsClicked())
		{
			this->bet = this->bet % 10 + 1;
		}
		// Adding if statement enables button Bet All
		if (this->btnBetAll->IsClicked())
		{
			this->bet = 10;
		}

		// Lines button - set which lines in the winnings model are active
		if (this->btnLines->IsClicked())
		{
			this->lines = this->lines % 5 + 1;
			
			for (int lineIndex = 0; lineIndex < this->winningsModel->numLines; lineIndex++)
				this->winningsModel->lines[lineIndex].active = lineIndex < this->lines;
		}
			
	}

	// Stop the spinning reels when the button is pressed
	if (this->reels->state == Spinning)
	{
		if (this->btnStop->IsClicked())
			this->reels->Stop();
	}

	// When reels' state HAS JUST CHANGED to Idle, then show any winning lines, if any, and update combination winning and credits
	if (this->reels->stateChange == Idle)
	{
		for (int winIndex = 0; winIndex < this->winningsAnalyzer->winnings.count; winIndex++)
			this->lineAnimations[this->winningsAnalyzer->winnings.win[winIndex].lineIndex]->visible = true;

		this->winning = this->winningsAnalyzer->totalWinning;
		this->credits += this->winning;

		game->soundDevice->PauseSource(this->sndReels);
	}

	// Sound on every reel stop
	for (int reelIndex = 0; reelIndex < this->reels->numReels; reelIndex++)
		if (this->reels->reels[reelIndex].stateChange == PostStopping)
			game->soundDevice->PlaySource(this->sndReelStop);

	GkGamePage::Process(game);
}