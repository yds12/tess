#include "stage.h"

using namespace Tess;

Stage::Stage(Game* game, Drawer* drawer, bool multiplayer, bool netGame, bool cpu, int difficulty)
{
	this->cpu = cpu;
	this->difficulty = difficulty;
	this->game = game;
	this->drawer = drawer;
	this->multiplayer = multiplayer;
	this->netGame = netGame;
	
	blockClearingAnimations.clear();
	
	if(this->multiplayer)
	{	
		stageX = 25;
		stageY = 60;
		enemyStageX = 535;
		enemyStageY = 60;
		
		if(!netGame) ai = new AI();
	}
	else
	{
		stageX = 284;
		stageY = 66;
	}
	
	if(difficulty == DIFFICULTY_EASY)
	{
		pieceSpeed = 40;
		speedProgression = 4;
		linesToSpeedUp = 12;
	}
	else if(difficulty == DIFFICULTY_MODERATE)
	{
		pieceSpeed = 60;
		speedProgression = 6;
		linesToSpeedUp = 10;
	}
	else if(difficulty == DIFFICULTY_HARD)
	{
		pieceSpeed = 80;
		speedProgression = 10;
		linesToSpeedUp = 10;
	}
	
	win = false;
	pieceIsFalling = false;
	gameOver = false;
	end = false;
	lines = 0;
	score = 0;
	linesPanel = 0;
	scorePanel = 0;
	displayGrid = true;
	paused = false;
	quitConfirmation = false;
	inputName = false;
	scoreMultiplier = 1;
	timer = new Timer();
	totalEllapsedTime = 0;
	lastPieceTime = 0;
	timeToAnimateScorePanel = 4;
	itemChance = 0;
	
	currentPiece = NULL;
	nextPiece = NULL;
	
	stageMatrix = new int*[STAGE_WIDTH];
	for(int i = 0; i < STAGE_WIDTH; i++) 
	{
		stageMatrix[i] = new int[STAGE_HEIGHT];
		
		for(int j = 0; j < STAGE_HEIGHT; j++)
			stageMatrix[i][j] = 0;
	}
	
	// Skin	
	skin = new Skin(drawer, Support::settings.skinName, Support::settings.bgSkinName);
	drawActiveArea = false;

	defaultFont = Support::defaultFont;
	panelFont = Support::panelFont;
	tetrisFont = Support::tetrisFont;
	gridImg = Support::loadImage("grid");
	areaImg = Support::loadImage("area");
	alphaImg = Support::getAlphaFrame();
	
	if(multiplayer) fgImg = skin->getMultiplayerForeground();
	else fgImg = skin->getForeground();
	
	ranking = new Ranking();
	ranking->load(difficulty);
	
	if(multiplayer && !cpu)
	{
		enemyStage = new Stage(game, drawer, true, false, true, difficulty);
		enemyStage->enemyStage = this;
	}
	
	linesToSend = 0;
	item = NULL;
	activeItem = NULL;
	enemyItem = NULL;
}

Stage::~Stage()
{
	delete ranking;
	delete gridImg;
	delete areaImg;
	delete fgImg;
	delete skin;
	if(netGame) delete net;
	else if(multiplayer && !cpu) delete enemyStage;
}

int Stage::getLinesToSend()
{
	int num = linesToSend;
	linesToSend = 0;
	return num;
}

void Stage::update()
{
	update(NULL, NULL);
}

void Stage::update(Keyboard* kb, Mouse* mouse)
{	
	// sair do stage
	if(end) return;

	// painel numérico animado
	if(game->gameCounter % timeToAnimateScorePanel == 0)
	{	
		if(lines > linesPanel) linesPanel++;
		if(score > scorePanel) scorePanel++;
	}
		
	// nome para ranking
	if(inputName)
	{
		dialog->update();
		if(dialog->exit) 
		{
			inputName = false;
			ranking->addEntry(dialog->name, score, lines);
			ranking->save(difficulty);
		}
		return;
	}		
	else if(gameOver) // game over
	{
		if(cpu) return;

		if(kb->isKeyPressed(kb->getIndex(AGL::KEY_ENTER))
			|| mouse->isLeftButtonPressed()) end = true;
		return;
	}
	else // em jogo
	{		
		if(netGame)
		{
			net->handleMessages(); // atualiza mensagens de rede
		
			// inimigo da rede perdeu
			if(net->heLost)
			{
				gameOver = true;
				win = true;
			}
		}
		else if(multiplayer && !cpu)
		{
			enemyStage->update(); // atualiza cpu
		}
		
		if(multiplayer && !netGame)
		{
			// cpu ou player perdeu
			if(enemyStage->gameOver)
			{
				gameOver = true;
				win = true;
			}
		}
		
		// tela cheia
		if(gameIsOver()) 
		{		
			gameOver = true;
						
			if(netGame) // avisa player pela rede
			{
				net->sendMessage(MSG_GAMEOVER);
			}
			else if(!multiplayer) // coloca nome para ranking
			{
				rank = ranking->checkEntry(score);
							
				if(rank > -1) 
				{
					inputName = true;
					dialog = new NameDialog(drawer, mouse);
				}					
			}
		}
	}
	
	// pausar
	if(paused) 
	{
		if(quitConfirmation)
		{
			quitMenu->update();
			
			if(quitMenu->optionChosen)
			{
				if(quitMenu->selected == 0) 
				{
					quitConfirmation = false;
					delete quitMenu;
					quitMenu = NULL;
					pauseOrResume(kb, mouse);
				}
				else end = true;
			}
			return;
		}
		if(!cpu)
		{ 
			pauseMenu->update();
			
			if(pauseMenu->optionChosen)
			{
				if(pauseMenu->selected == 0) pauseOrResume(kb, mouse);
				else 
				{
					delete pauseMenu;
					pauseMenu = NULL;
					
					quitConfirmation = true;					
					string* options = new string[2];
					options[0] = Lang::get(40);
					options[1] = Lang::get(41);
					quitMenu = new Menu(options, 2, drawer, kb, mouse);
					quitMenu->set(stageX + 10, stageY + 240);
				}
			}
		}
		
		return;
	}
	if(!netGame && !cpu && kb->isKeyPressed(kb->getIndex(AGL::KEY_ENTER))) 
	{
		pauseOrResume(kb, mouse);
	}

	// atualizar skin
	skin->update();
	
	int animSize = blockClearingAnimations.size();
	for(int i = animSize - 1; i >= 0; i--)
	{
		blockClearingAnimations[i]->update();
		
		if(blockClearingAnimations[i]->finished)
			blockClearingAnimations.erase(blockClearingAnimations.begin() + i);
	}
	
	// item
	if(activeItem != NULL)
	{ 
		activeItem->update();
		if(activeItem->finished) finishItem();
	}
	
	if(enemyItem != NULL)
	{ 
		enemyItem->update();
		if(enemyItem->finished) 
		{
			delete enemyItem;
			enemyItem = NULL;
		}
	}
	
	if(netGame) 
	{
		int it = net->getEnemyItem();
		if(it > -1) enemyUseItem(it);
	}

	// peça parou
	if(!pieceIsFalling)
	{
		checkLines();				
		if(multiplayer) createItens();
		pieceIsFalling = true;		
		lastPieceTime = totalEllapsedTime;		
		timer->start();
		
		newPiece();
		if(multiplayer && !netGame) ai->calculate(stageMatrix, currentPiece);
	}
	else // peça caindo
	{
		float ellapsedTime = timer->stop();
		timer->start();
		totalEllapsedTime += ellapsedTime;
		
		if(!cpu)
		{
			if(kb->isKeyPressed(kb->getIndex(AGL::KEY_DOWN)) ||
				kb->isKeyDown(kb->getIndex(AGL::KEY_DOWN))) currentPiece->shift += BLOCK_SIZE / 2;
			if(kb->isKeyPressed(kb->getIndex(AGL::KEY_SPACE))) currentPiece->allocate(stageMatrix);
			if(kb->isKeyPressed(kb->getIndex(AGL::KEY_LEFT)) ||
				kb->isKeyHeld(kb->getIndex(AGL::KEY_LEFT))) currentPiece->tryMoveLeft(stageMatrix);
			if(kb->isKeyPressed(kb->getIndex(AGL::KEY_RIGHT)) ||
				kb->isKeyHeld(kb->getIndex(AGL::KEY_RIGHT))) currentPiece->tryMoveRight(stageMatrix);
			if(kb->isKeyPressed(kb->getIndex(AGL::KEY_UP)) ||
				kb->isKeyHeld(kb->getIndex(AGL::KEY_UP))) 
			{
				if(currentPiece->tryRotate(stageMatrix))
				{
					if(netGame) net->sendMessage(MSG_ROTATE);
				}
			}
			if(kb->isKeyPressed(kb->getIndex(AGL::KEY_W)) && item != NULL) useItem();
		}
		else
		{				
			if(totalEllapsedTime - lastPieceTime > ai->newPieceDelay)
			{
				// cpu movements
				int doSomething = ((rand() % 100) > 99 - ai->speed);
			
				if(doSomething)
				{
					if(item != NULL) 
					{
						bool recalculate = false;
						if(item->type == ITEM_BOMB || item->type == ITEM_THIEF) recalculate = true;
						useItem();
					
						if(recalculate) ai->calculate(stageMatrix, currentPiece);
					}
					if(ai->rotation != currentPiece->rotation && !currentPiece->item) 
						currentPiece->tryRotate(stageMatrix);
					else if(ai->hShift > currentPiece->hShift) 
						currentPiece->tryMoveRight(stageMatrix);
					else if(ai->hShift < currentPiece->hShift) 
						currentPiece->tryMoveLeft(stageMatrix);
					else if((rand() % 100) > 99 - ai->allocationSpeed)
					{
						currentPiece->allocate(stageMatrix);
					}
				}
			}
		}
		
		if(currentPiece->stopped)
		{
			if(activeItem != NULL) 
			{
				activeItem->stop(stageMatrix, (currentPiece->hShift), (currentPiece->shift / BLOCK_SIZE));
			}
			
			pieceIsFalling = false;
			
			if(netGame)
			{
				sendMatrix();
				insertLines(net->getLinesReceived());
			}
			else if(multiplayer)
			{
				insertLines(enemyStage->getLinesToSend());
			}			
		}
		else
		{			
			currentPiece->shift += ellapsedTime * pieceSpeed;		
			currentPiece->checkCollisions(stageMatrix);
			
			if(netGame)
			{
				char buf[20];
				sprintf(buf, "%s %d %d", MSG_PIECEPOS, currentPiece->hShift, (int)currentPiece->shift);
				net->sendMessage(buf);
			}
		}
	}
}

void Stage::pauseOrResume(Keyboard* keyboard, Mouse* mouse)
{
	if(paused) timer->start();
	else timer->stop();
	
	paused = !paused;
	
	if(!cpu)
	{
		if(multiplayer) enemyStage->pauseOrResume(NULL, NULL);
		
		if(paused)
		{
			string* options = new string[2];
			options[0] = Lang::get(55);
			options[1] = Lang::get(56);
			pauseMenu = new Menu(options, 2, drawer, keyboard, mouse);
			pauseMenu->set(stageX + 10, stageY + 200);
		}
		else
		{
			delete pauseMenu;
			pauseMenu = NULL;
		}
	}
}

void Stage::newPiece()
{	
	currentPiece = new Piece(skin->colors);
	if(nextPiece != NULL) currentPiece->generatePiece(nextPiece->type, nextPiece->color);
	
	nextPiece = new Piece(skin->colors);
	notifyPiece();	
}

void Stage::notifyPiece()
{
	if(netGame)
	{
		char buf[10];
		sprintf(buf, "%s %d %d %d", MSG_NEWPIECE, currentPiece->type, currentPiece->color, currentPiece->item);
		net->sendMessage(buf);
	}	
}

bool Stage::gameIsOver()
{
	for(int i = 0; i < STAGE_WIDTH; i++) 
		if(stageMatrix[i][3] > 0) return true;
		
	return false;
}

void Stage::sendMatrix()
{
	string matrix = "";
	
	for(int j = 0; j < STAGE_HEIGHT; j++)
		for(int i = 0; i < STAGE_WIDTH; i++)
		{
			char c[10];
			sprintf(c, "%d", stageMatrix[i][j]);
			matrix += c[0];
		}
			
	char buf[STAGE_WIDTH * STAGE_HEIGHT + 10];
	sprintf(buf, "%s %s", MSG_MATRIX, matrix.c_str());
					
	net->sendMessage(buf);
}

void Stage::createItens()
{
	if(rand() % 100 < (100 - itemChance)) return;
	int r = rand() % (STAGE_WIDTH * STAGE_HEIGHT);

	int count;
	int pieces;
	do
	{
		count = 0;
		pieces = 0;
		for(int j = STAGE_HEIGHT - 1; j >= 0; j--)
			for(int i = 0; i < STAGE_WIDTH; i++)
				if(stageMatrix[i][j] > 0)
				{
					pieces++;
					if(count == r)
					{
						stageMatrix[i][j] = 9;
						return;
					}
					count++;
				}
		
		if(pieces > 0) r = r % pieces;
	} while(pieces > 0);
}

void Stage::getItem()
{	
	item = new Item();
}

void Stage::useItem()
{	
	activeItem = item;
	item = NULL;	
	activeItem->use();
	
	switch(activeItem->type)
	{
		case ITEM_THIEF:
			currentPiece = new Piece();			
			if(netGame)	currentPiece->generatePiece(net->enemyPiece->type, net->enemyPiece->color);
			else currentPiece->generatePiece(enemyStage->currentPiece->type, enemyStage->currentPiece->color);			
			notifyItem();
			notifyPiece();
			break;
			
		case ITEM_TURTLE:
			pieceSpeed = pieceSpeed / 2;
			break;
			
		case ITEM_CLOUD:
			notifyItem();
			break;
			
		case ITEM_BOMB:
			currentPiece = new Piece();
			currentPiece->generateItemPiece();
			notifyItem();
			notifyPiece();
			break;
			
		default:
			break;
	}
}

void Stage::notifyItem()
{	
	if(netGame)
	{
		char buf[20];
		sprintf(buf, "%s %d", MSG_ITEM, activeItem->type);
		net->sendMessage(buf);
	}
	else
		enemyStage->enemyUseItem(activeItem->type);
}

void Stage::enemyUseItem(int type)
{
	switch(type)
	{			
		case ITEM_THIEF:
			newPiece();
			break;
			
		case ITEM_CLOUD:
			enemyItem = new Item(type);
			enemyItem->use();
			break;
			
		case ITEM_BOMB:
			enemyItem = new Item(type);
			enemyItem->use();
			break;
			
		default:
			break;
	}
}

void Stage::finishItem()
{	
	switch(activeItem->type)
	{			
		case ITEM_TURTLE:
			pieceSpeed = pieceSpeed * 2;
			break;
			
		default:
			break;
	}
	
	delete activeItem;
	activeItem = NULL;
}

void Stage::checkLines()
{
	int turnLines = 0;
	for(int j = STAGE_HEIGHT - 1; j >= 0; j--)
	{
		bool line = true;
		bool item = false;
		for(int i = 0; i < STAGE_WIDTH; i++)
		{
			if(stageMatrix[i][j] == 0) 
			{
				line = false;
				break;
			}
			if(stageMatrix[i][j] == 9) item = true;
		}
		
		if(line) 
		{
			if(item) getItem();
			turnLines++;
			clearLine(j);
			j++;
		}
	}
		
	int turnScore;
	
	switch(turnLines)
	{
		case 1: turnScore = 1; break;
		case 2: turnScore = 4; break;
		case 3: turnScore = 8; break;
		case 4: turnScore = 16; break;
		default: turnScore = 0; break;
	}
	
	int ls = (int)(turnScore / 4);		
	if(turnLines > 0 && scoreMultiplier > 1) ls++;
		
	if(ls > 0)
	{
		if(netGame)
		{
			char buf[10];
			sprintf(buf, "%s %d", MSG_LINE, ls);
			net->sendMessage(buf);
		}
		else if(multiplayer)
		{
			linesToSend += ls;
		}
	}
	
	score += turnScore * scoreMultiplier;
	
	if(turnScore > 0) scoreMultiplier += turnLines;
	else scoreMultiplier = 1;	
}

void Stage::clearLine(int line)
{
	animateLineClearing(line);
	lines++;
	if(lines % linesToSpeedUp == 0) pieceSpeed += speedProgression;

	for(int j = line; j > 0; j--)
		for(int i = 0; i < STAGE_WIDTH; i++)
			stageMatrix[i][j] = stageMatrix[i][j - 1];
	
	for(int i = 0; i < STAGE_WIDTH; i++) stageMatrix[i][0] = 0;
	
	if(netGame) sendMatrix();
}

void Stage::animateLineClearing(int line)
{	
	for(int i = 0; i < STAGE_WIDTH; i++)
	{
		IntVector2 v(stageX + i * BLOCK_SIZE, stageY + line * BLOCK_SIZE);
		blockClearingAnimations.push_back(skin->getBlockClearingAnimation(game, 0 /*stageMatrix[i][line]*/, v));
	}
}

void Stage::insertLines(int lines)
{
	if(lines == 0) return;
	if(lines > STAGE_HEIGHT - 1) lines = STAGE_HEIGHT - 1;
	
	for(int j = 0; j < STAGE_HEIGHT - lines; j++)
	{
		for(int i = 0; i < STAGE_WIDTH; i++)
			stageMatrix[i][j] = stageMatrix[i][j + lines];
	}
	
	for(int j = 0; j < lines; j++)
	{
		int r = rand() % STAGE_WIDTH;
		
		for(int i = 0; i < STAGE_WIDTH; i++)
			if(i != r) stageMatrix[i][STAGE_HEIGHT - j - 1] = 1;
			else stageMatrix[i][STAGE_HEIGHT - j - 1] = 0;
	}
	
	if(netGame) sendMatrix();
}

void Stage::draw()
{
	if(end) return;
	drawer->clearScreen(skin->bgColor);
	if(multiplayer) 
	{
		drawMultiplayer();
		return;
	}

	// grids
	if(displayGrid)
	{
		IntVector2 v1(stageX, stageY);
		drawer->draw(gridImg, v1);
	}

	// pieces
	if(pieceIsFalling)
	{
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				if(currentPiece->pieceMatrix[i][j] == 1)
				{
					IntVector2 v3(stageX + (currentPiece->hShift + i) * BLOCK_SIZE, 
									  (int)ROUND(currentPiece->shift) + j * BLOCK_SIZE - 96 + stageY);
					skin->draw(currentPiece->color, v3);					
				}
				else if(drawActiveArea)
				{
					IntVector2 v3(stageX + (currentPiece->hShift + i) * BLOCK_SIZE, 
									  (int)ROUND(currentPiece->shift) + j * BLOCK_SIZE - 96 + stageY);
					drawer->draw(areaImg, v3);
				}
	}
	
	// blocks
	for(int i = 0; i < STAGE_WIDTH; i++)
		for(int j = 0; j < STAGE_HEIGHT; j++)
			if(stageMatrix[i][j] > 0)
			{
				IntVector2 v7(stageX + i * BLOCK_SIZE, j * BLOCK_SIZE - 96 + stageY);
				skin->draw(stageMatrix[i][j], v7);
			}
	
	// clearing blocks
	for(int i = 0; i < blockClearingAnimations.size(); i++)
		blockClearingAnimations[i]->draw();
	
	// foreground
	IntVector2 v8(0, 0);
	drawer->draw(fgImg, v8);

	// information
	char text[50] = "";
	sprintf(text, "%d", linesPanel);
	char text2[50] = "";
	sprintf(text2, "%d", scorePanel);
	char text3[50] = "";
	sprintf(text3, "%dX", scoreMultiplier);
	
	IntVector2 v9(112, 146);
	drawer->drawText(defaultFont, Lang::get(38), v9, AGL_COLOR_BLACK);
	IntVector2 v15(112, 181);
	drawer->drawText(panelFont, text, v15, AGL_COLOR_WHITE);
	
	IntVector2 v10(112, 272);
	drawer->drawText(defaultFont, Lang::get(37), v10, AGL_COLOR_BLACK);
	IntVector2 v16(112, 307);
	drawer->drawText(panelFont, text2, v16, AGL_COLOR_WHITE);
	
	IntVector2 v14(620, 132);
	drawer->drawText(defaultFont, Lang::get(49), v14, AGL_COLOR_BLACK);
	
	if(nextPiece != NULL)
	{
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				if(nextPiece->pieceMatrix[i][j] == 1)
				{
					IntVector2 v17(622 + i * BLOCK_SIZE, 178 + j * BLOCK_SIZE);
					skin->draw(nextPiece->color, v17);	
				}
	}
	
	if(scoreMultiplier > 1)
	{
		IntVector2 v11(390, 300);
		Support::drawShadowedText(drawer, panelFont, text3, v11, AGL_COLOR_RED, AGL_COLOR_WHITE);
	}
	
	if(paused)
	{
		drawPauseMenu();
	}
	
	if(gameOver)
	{
		if(inputName) dialog->draw();
		else ranking->show(drawer, rank);
		
		IntVector2 v13(stageX - 35, stageY + 30);
		Support::drawShadowedText(drawer, tetrisFont, Lang::get(51), v13, AGL_COLOR_NAVY, AGL_COLOR_WHITE);		
	}
}

void Stage::drawMultiplayer()
{
	// grids
	IntVector2 stagePos(stageX, stageY);
	IntVector2 enemyStagePos(enemyStageX, enemyStageY);
	if(displayGrid)
	{
		drawer->draw(gridImg, stagePos);		
		drawer->draw(gridImg, enemyStagePos);		
	}

	// pieces
	if(pieceIsFalling)
	{
		if(currentPiece->item)
		{
			for(int i = 0; i < 4; i++)
				for(int j = 0; j < 4; j++)
					if(currentPiece->pieceMatrix[i][j] == 1)
					{
						IntVector2 v3(stageX + (currentPiece->hShift + i) * BLOCK_SIZE, 
										  (int)ROUND(currentPiece->shift) + j * BLOCK_SIZE - 96 + stageY);
						drawer->draw(activeItem->image, v3);					
					}
		}
		else
		{
			for(int i = 0; i < 4; i++)
				for(int j = 0; j < 4; j++)
					if(currentPiece->pieceMatrix[i][j] == 1)
					{
						IntVector2 v3(stageX + (currentPiece->hShift + i) * BLOCK_SIZE, 
										  (int)ROUND(currentPiece->shift) + j * BLOCK_SIZE - 96 + stageY);
						skin->draw(currentPiece->color, v3);					
					}
					else if(drawActiveArea)
					{
						IntVector2 v3(stageX + (currentPiece->hShift + i) * BLOCK_SIZE, 
										  (int)ROUND(currentPiece->shift) + j * BLOCK_SIZE - 96 + stageY);
						drawer->draw(areaImg, v3);
					}
		}
	}

	if(netGame)
	{
		if(net->enemyPiece != NULL)
		{
			if(net->enemyPiece->item && enemyItem != NULL)
			{
				for(int i = 0; i < 4; i++)
					for(int j = 0; j < 4; j++)
						if(net->enemyPiece->pieceMatrix[i][j] == 1)
						{
							IntVector2 v3(enemyStageX + (net->enemyPiece->hShift + i) * BLOCK_SIZE, 
											  (int)ROUND(net->enemyPiece->shift) + j * BLOCK_SIZE - 96 + enemyStageY);
							drawer->draw(enemyItem->image, v3);					
						}
			}
			else
			{
				for(int i = 0; i < 4; i++)
					for(int j = 0; j < 4; j++)
						if(net->enemyPiece->pieceMatrix[i][j] == 1)
						{
							IntVector2 v3(enemyStageX + (net->enemyPiece->hShift + i) * BLOCK_SIZE, 
											  (int)ROUND(net->enemyPiece->shift) + j * BLOCK_SIZE - 96 + enemyStageY);
							skin->draw(net->enemyPiece->color, v3);					
						}
			}
		}
	
		// enemy blocks
		for(int i = 0; i < STAGE_WIDTH; i++)
			for(int j = 0; j < STAGE_HEIGHT; j++)
				if(net->enemyMatrix[i][j] > 0)
				{
					IntVector2 v7(enemyStageX + i * BLOCK_SIZE, j * BLOCK_SIZE - 96 + enemyStageY);
					skin->draw(net->enemyMatrix[i][j], v7);
				}
	}
	else
	{
		if(enemyStage->pieceIsFalling)
		{
			for(int i = 0; i < 4; i++)
				for(int j = 0; j < 4; j++)
					if(enemyStage->currentPiece->pieceMatrix[i][j] == 1)
					{
						IntVector2 v3(enemyStageX + (enemyStage->currentPiece->hShift + i) * BLOCK_SIZE, 
										  (int)ROUND(enemyStage->currentPiece->shift) + j * BLOCK_SIZE - 96 + enemyStageY);
						skin->draw(enemyStage->currentPiece->color, v3);					
					}
					else if(drawActiveArea)
					{
						IntVector2 v3(enemyStageX + (enemyStage->currentPiece->hShift + i) * BLOCK_SIZE, 
										  (int)ROUND(enemyStage->currentPiece->shift) + j * BLOCK_SIZE - 96 + enemyStageY);
						drawer->draw(areaImg, v3);
					}
		}
	
		// enemy blocks
		for(int i = 0; i < STAGE_WIDTH; i++)
			for(int j = 0; j < STAGE_HEIGHT; j++)
				if(enemyStage->stageMatrix[i][j] > 0)
				{
					IntVector2 v7(enemyStageX + i * BLOCK_SIZE, j * BLOCK_SIZE - 96 + enemyStageY);
					skin->draw(enemyStage->stageMatrix[i][j], v7);
				}
	}

	// blocks
	for(int i = 0; i < STAGE_WIDTH; i++)
		for(int j = 0; j < STAGE_HEIGHT; j++)
			if(stageMatrix[i][j] > 0)
			{
				IntVector2 v7(stageX + i * BLOCK_SIZE, j * BLOCK_SIZE - 96 + stageY);
				skin->draw(stageMatrix[i][j], v7);
			}
					
	// item effect
	if(activeItem != NULL && !activeItem->finished)
	{
		activeItem->drawEffect(drawer, stagePos);
		activeItem->drawEnemyEffect(drawer, enemyStagePos);
	}
		
	if(enemyItem != NULL && !enemyItem->finished)
	{
		enemyItem->drawEffect(drawer, enemyStagePos);
		enemyItem->drawEnemyEffect(drawer, stagePos);
	}
						
	// foreground
	IntVector2 v8(0, 0);
	drawer->draw(fgImg, v8);
	
	// information
	char text[50] = "";
	sprintf(text, "%d", linesPanel);
	char text2[50] = "";
	sprintf(text2, "%d", scorePanel);
	char text3[50] = "";
	sprintf(text3, "%dX", scoreMultiplier);

	// lines
	IntVector2 v9(304, 442);
	drawer->drawText(defaultFont, Lang::get(38), v9, AGL_COLOR_BLACK);
	IntVector2 v15(304, 477);
	drawer->drawText(panelFont, text, v15, AGL_COLOR_WHITE);
	
	// score
	IntVector2 v10(410, 442);
	drawer->drawText(defaultFont, Lang::get(37), v10, AGL_COLOR_BLACK);
	IntVector2 v16(410, 477);
	drawer->drawText(panelFont, text2, v16, AGL_COLOR_WHITE);
	
	// item
	IntVector2 v18(304, 300);
	drawer->drawText(defaultFont, Lang::get(54), v18, AGL_COLOR_BLACK);
	
	if(item != NULL)
	{
		IntVector2 v19(307, 350);
		item->drawIcon(drawer, v19);
	}
	
	//next
	IntVector2 v14(304, 121);
	drawer->drawText(defaultFont, Lang::get(49), v14, AGL_COLOR_BLACK);

	if(nextPiece != NULL)
	{
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				if(nextPiece->pieceMatrix[i][j] == 1)
				{
					IntVector2 v17(310 + i * BLOCK_SIZE, 166 + j * BLOCK_SIZE);
					skin->draw(nextPiece->color, v17);	
				}
				else if(drawActiveArea)
				{
					IntVector2 v17(310 + i * BLOCK_SIZE, 166 + j * BLOCK_SIZE);
					drawer->draw(areaImg, v17);
				}
	}

	if(scoreMultiplier > 1)
	{
		IntVector2 v11(stageX + 100, 300);
		Support::drawShadowedText(drawer, panelFont, text3, v11, AGL_COLOR_RED, AGL_COLOR_WHITE);
	}
	
	if(paused)
	{
		drawPauseMenu();
	}
	
	if(gameOver)
	{
		const char* msg;
		
		if(win) msg = Lang::get(52);
		else msg = Lang::get(53);
		
		IntVector2 v13(stageX, stageY + 30);
		Support::drawShadowedText(drawer, tetrisFont, msg, v13, AGL_COLOR_NAVY, AGL_COLOR_WHITE);		
	}
}

void Stage::drawPauseMenu()
{
	IntVector2 zero(0, 0);
	drawer->draw(alphaImg, zero);
	
	IntVector2 v12(stageX + 10, stageY + 100);
	Support::drawShadowedText(drawer, tetrisFont, Lang::get(50), v12, AGL_COLOR_NAVY, AGL_COLOR_WHITE);
		
	if(quitConfirmation)
	{
		IntVector2 v13(stageX + 10, stageY + 200);
		drawer->drawText(Support::defaultFont, Lang::get(57), v13, Support::defaultTextColor);
		
		quitMenu->draw();
	}
	else
		pauseMenu->draw();
}

