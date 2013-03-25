/*
=================
cKnight.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cKnight.h"
/*
=================
- Data constructor initializes the cBalloon to the data passed to 
- the constructor from the paramater sPosition.
- Is always called, thus ensures all sprite objects are in a consistent state.
=================
*/
cKnight::cKnight(): cSprite() 			// Default constructor
{
	mActive = true;
	mNoFrames = 8;
	mCurrentFrame = 0;
	mRate = 1000.0f/25.0f;
	SetRect(&mSourceRect,0,0,cKnight::getSTWidth(),cKnight::getSTHeight());
}
cKnight::cKnight(D3DXVECTOR3 sPosition, LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR theFilename): cSprite(sPosition,pd3dDevice,theFilename)  // Constructor
{
	mActive = true;
	mNoFrames = 8;
	mCurrentFrame = 0;
	mRate = 1000.0f/700.0f;
	SetRect(&mSourceRect,0,0,cKnight::getSTWidth(),cKnight::getSTHeight());
}
/*
=================
- update the position etc of the sprite.
=================
*/
void cKnight::update(float dt) 					// update the Balloons position based on the translation
{
	cSprite::update();
	
	if(dt < mRate)
	{
	
		if(mCurrentFrame > mNoFrames)
		{
			mCurrentFrame = 0;
		}
		else{mCurrentFrame++;}
		SetRect(&mSourceRect,mCurrentFrame*217,0,(mCurrentFrame*217)+217,250);
	}
	
}
/*
=================
- Return the current segment of the sprite sheet to draw.
=================
*/
RECT cKnight::getSourceRect() 					// Return the current segment of the sprite sheet to draw.
{
	return mSourceRect;
}
/*
=================
- Return the current frame.
=================
*/
int cKnight::getCurrentFrame() 					// Return the current frame.
{
	return mCurrentFrame;
}
/*
=================
- Set the sprite to active.
=================
*/
void cKnight::setActive(bool sActive) 			// Set the sprite to active.
{
	mActive = sActive;
}
/*
=================
- Determine if the sprite is active.
=================
*/
bool cKnight::isActive() 						// Determine if the sprite is active.
{
	return mActive;
}
