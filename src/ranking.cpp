#include "ranking.h"

using namespace Tess;

Ranking::Ranking()
{
	size = 10;
	scores = new int[size];
	lines = new int[size];
	names = new string[size];
	timerStarted = false;
	
	for(int i = 0; i < size; i++)
	{
		scores[i] = -1;
		lines[i] = -1;
		names[i] = "-";
	}
	
	bg = Support::getAlphaFrame();	
	font = Support::rankingFont;
	
	rankingFiles = new string[3];
	rankingFiles[0] = "cfgranke";
	rankingFiles[1] = "cfgrankm";
	rankingFiles[2] = "cfgrankh";
}

Ranking::~Ranking()
{
	delete bg;
}

void Ranking::load(int difficulty)
{
	for(int i = 0; i < size; i++)
	{
		scores[i] = -1;
		lines[i] = -1;
		names[i] = "-";
	}
	
	if(FileManipulator::exists(rankingFiles[difficulty]))
	{
		FileManipulator f(rankingFiles[difficulty]);
		f.openReader();
	
		if(f.ready())
		{
			for(int i = 0; i < size; i++)
			{
				string txt = f.readLine();
				if(f.end() || txt == "") break;
				
				names[i] = txt;
				scores[i] = Format::stringToInt(f.readLine());
				lines[i] = Format::stringToInt(f.readLine());
			}
		}
	
		f.closeReader();
	}
}

int Ranking::addEntry(string name, int score, int line)
{
	int position = -1;
	for(int i = 0; i < size; i++)
		if(score > scores[i])
		{
			position = i;
			break;
		}
		
	if(position > -1)
	{
		for(int i = size - 1; i > position; i--)
		{
			scores[i] = scores[i - 1];
			lines[i] = lines[i - 1];
			names[i] = names[i - 1];
		}
		
		scores[position] = score;
		lines[position] = line;
		names[position] = name;
	}
	
	return position;
}

void Ranking::clearAll()
{
	for(int i = DIFFICULTY_EASY; i <= DIFFICULTY_HARD; i++)
		if(FileManipulator::exists(rankingFiles[i]))
		{
			FileManipulator fm(rankingFiles[i]);
			fm.openWriter();
			fm.closeWriter();
		}
}

int Ranking::checkEntry(int score)
{
	int position = -1;
	for(int i = 0; i < size; i++)
		if(score > scores[i])
		{
			position = i;
			break;
		}
	
	return position;
}

void Ranking::save(int difficulty)
{
	FileManipulator f(rankingFiles[difficulty]);
	f.openWriter();
		
	for(int i = 0; i < size; i++)
	{
		f.writeLine(names[i]);
		
		char str1[6];
		sprintf(str1, "%d", scores[i]);		
		f.writeLine(str1);
		
		char str2[6];
		sprintf(str2, "%d", lines[i]);		
		f.writeLine(str2);
	}
	
	f.closeWriter();
}

void Ranking::show(Drawer* drawer)
{
	show(drawer, -1);
}

void Ranking::show(Drawer* drawer, int position)
{
	IntVector2 v(140, 180);
	show(drawer, position, v);
}

void Ranking::show(Drawer* drawer, int position, IntVector2 location)
{
	Color c = AGL_COLOR_GOLD;
	if(position > -1)
	{
		if(!timerStarted)
		{
			t = new Timer();
			t->start();
			timerStarted = true;
		}
		
		float time = t->stop();
		
		if((int) time % 2 == 0) // escurecendo
		{
			c = Graphics::lerp(AGL_COLOR_RED, AGL_COLOR_BLACK, time - ((int)time));
		}
		else // clareando
		{
			c = Graphics::lerp(AGL_COLOR_BLACK, AGL_COLOR_RED, time - ((int)time));
		}
	}

	int x = location.x;
	int y = location.y;	
	
	IntVector2 v2(0, 0);
	drawer->draw(bg, v2);
		
	IntVector2 v6(x + 40, y);		
	drawer->drawText(font, Lang::get(36), v6, AGL_COLOR_WHITE);
	IntVector2 v7(x + 370, y);		
	drawer->drawText(font, Lang::get(37), v7, AGL_COLOR_WHITE);
	IntVector2 v8(x + 470, y);		
	drawer->drawText(font, Lang::get(38), v8, AGL_COLOR_WHITE);
	
	for(int i = 0; i < size; i++)
	{
		if(scores[i] < 0) break;
		
		Color color = (position == i ? c : AGL_COLOR_GOLD);
		
		char txt3[6];
		sprintf(txt3, "%d.", i + 1);
		IntVector2 v5(x, y + (i + 1) * 30);		
		drawer->drawText(font, txt3, v5, color);		
		
		IntVector2 v(x + 40, y + (i + 1) * 30);		
		drawer->drawText(font, names[i].c_str(), v, color);
		
		char txt1[6];
		sprintf(txt1, "%d", scores[i]);
		IntVector2 v3(x + 370, y + (i + 1) * 30);		
		drawer->drawText(font, txt1, v3, color);
		
		char txt2[6];
		sprintf(txt2, "%d", lines[i]);
		IntVector2 v4(x + 470, y + (i + 1) * 30);		
		drawer->drawText(font, txt2, v4, color);
	}
}

