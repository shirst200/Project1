/*
=================
cExplosion.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cExplosion.h"
/*
=================
- Data constructor initializes the cBalloon to the data passed to 
- the constructor from the paramater sPosition.
- Is always called, thus ensures all sprite objects are in a consistent state.
=================
*/
cExplosion::cExplosion(): cSprite() 			// Default constructor
{
	mActive = true;
	mNoFrames = 15;
	mCurrentFrame = 0;
	mRate = 1000.0f/25.0f;
	SetRect(&mSourceRect,0,0,cExplosion::getSTWidth(),cExplosion::getSTHeight());
}
cExplosion::cExplosion(D3DXVECTOR3 sPosition, LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR theFilename): cSprite(sPosition,pd3dDevice,theFilename)  // Constructor
{
	mActive = true;
	mNoFrames = 15;
	mCurrentFrame = 0;
	mRate = 1000.0f/25.0f;
	SetRect(&mSourceRect,0,0,cExplosion::getSTWidth(),cExplosion::getSTHeight());
}
/*
=================
- update the position etc of the sprite.
=================
*/
void cExplosion::update(float dt) 					// update the Balloons position based on the translation
{
	cSprite::update();
	
	if(dt < mRate)
	{
	
		if(mCurrentFrame > mNoFrames)
		{
			mCurrentFrame = 0;
			mActive = false;
		}
		else{mCurrentFrame++;}
		SetRect(&mSourceRect,mCurrentFrame*50,0,(mCurrentFrame*50)+50,50);
	}
	
}
/*
=================
- Return the current segment of the sprite sheet to draw.
=================
*/
RECT cExplosion::getSourceRect() 					// Return the current segment of the sprite sheet to draw.
{
	return mSourceRect;
}
/*
=================
- Return the current frame.
=================
*/
int cExplosion::getCurrentFrame() 					// Return the current frame.
{
	return mCurrentFrame;
}
/*
=================
- Set the sprite to active.
=================
*/
void cExplosion::setActive(bool sActive) 			// Set the sprite to active.
{
	mActive = sActive;
}
/*
=================
- Determine if the sprite is active.
=================
*/
bool cExplosion::isActive() 						// Determine if the sprite is active.
{
	return mActive;
}
