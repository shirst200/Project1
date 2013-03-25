/*
=================
main.cpp
Main entry point for the Card application
=================
*/

#include "GameConstants.h"
#include "GameResources.h"
#include "cD3DManager.h"
#include "cD3DXSpriteMgr.h"
#include "cD3DXTexture.h"
#include "cSprite.h"
#include "cExplosion.h"
#include "cBalloon.h"
#include "cKnight.h"
#include <strsafe.h>

using namespace std;

HINSTANCE hInst; // global handle to hold the application instance
HWND wndHandle; // global variable to hold the window handle

// Get a reference to the DirectX Manager
static cD3DManager* d3dMgr = cD3DManager::getInstance();

// Get a reference to the DirectX Sprite renderer Manager 
static cD3DXSpriteMgr* d3dxSRMgr = cD3DXSpriteMgr::getInstance();

D3DXVECTOR2 BalloonTrans = D3DXVECTOR2(300,300);
D3DXVECTOR2 knightTrans = D3DXVECTOR2(300,300);
bool falling;
RECT clientBounds;

int BalloonPosition=clientBounds.right/2-200;
vector<cBalloon*> aBalloon;
vector<cBalloon*>::const_iterator iterRO;
vector<cBalloon*>::iterator iter;
vector<cBalloon*>::iterator index;

TCHAR szTempOutput[30];

bool gHit = false;
D3DXVECTOR3 expPos;
list<cExplosion*> gExplode;
list<cKnight*> gKnight; 

/*
==================================================================
* LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
* LPARAM lParam)
* The window procedure
==================================================================
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Check any available messages from the queue
	switch (message)
	{
		case WM_KEYDOWN:
			{
				if (wParam == VK_LEFT)
				{
					for(iter = aBalloon.begin(); iter != aBalloon.end(); ++iter)
					{
						if ((*iter)->getTranslation().x<10.0f)
						{
							(*iter)->setTranslation(D3DXVECTOR2(20.0f ,0.0f)); 
							BalloonPosition+=20;
						}
					}
				}
				if (wParam == VK_RIGHT)
				{
					for(iter = aBalloon.begin(); iter != aBalloon.end(); ++iter)
					{
						if ((*iter)->getTranslation().x>-10.0f)
						{
							(*iter)->setTranslation(D3DXVECTOR2(-20.0f ,0.0f)); 
							BalloonPosition-=20;
						}
					}
				}

				if (wParam == VK_UP)
				{
					falling=false;
					knightTrans.y -= 50.0f;
				}
				else {falling=true;}
				return 0;
			}


		case WM_KEYUP:
		{
			if (wParam ==VK_LEFT)
			{
					for(iter = aBalloon.begin(); iter != aBalloon.end(); ++iter)
					{
							(*iter)->setTranslation(D3DXVECTOR2(0.0f ,0.0f)); 
					}
			}
				if (wParam == VK_RIGHT)
				{
					for(iter = aBalloon.begin(); iter != aBalloon.end(); ++iter)
					{
						(*iter)->setTranslation(D3DXVECTOR2(0.0f ,0.0f)); 
					}
				}
			return 0;
		}

		case WM_LBUTTONDOWN:
			{
				POINT mouseXY;
				mouseXY.x = LOWORD(lParam);
				mouseXY.y = HIWORD(lParam);
				
				

				iter = aBalloon.begin();
				while (iter != aBalloon.end())
				{
					if ( (*iter)->insideRect((*iter)->getBoundingRect(),mouseXY))
					{
//						OutputDebugString("Hit!\n");
						iter = aBalloon.erase(iter);
						iter = aBalloon.end();
					}
					else
					{
						++iter;
					}
				}
			
				expPos = D3DXVECTOR3((float)mouseXY.x-25,(float)mouseXY.y-23, 0.0f);
				gExplode.push_back(new cExplosion(expPos,d3dMgr->getTheD3DDevice(),"Images\\explosion.png"));
				StringCchPrintf(szTempOutput, STRSAFE_MAX_CCH, TEXT("Mouse: lLastX=%d lLastY=%d\r\n"), LOWORD(lParam), HIWORD(lParam));
				OutputDebugString(szTempOutput);
				return 0;
			}

		case WM_CLOSE:
			{
			// Exit the Game
				PostQuitMessage(0);
				 return 0;
			}

		case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}
	}
	// Always return the message to the default window
	// procedure for further processing
	return DefWindowProc(hWnd, message, wParam, lParam);
}

/*
==================================================================
* bool initWindow( HINSTANCE hInstance )
* initWindow registers the window class for the application, creates the window
==================================================================
*/
bool initWindow( HINSTANCE hInstance )
{
	WNDCLASSEX wcex;
	// Fill in the WNDCLASSEX structure. This describes how the window
	// will look to the system
	wcex.cbSize = sizeof(WNDCLASSEX); // the size of the structure
	wcex.style = CS_HREDRAW | CS_VREDRAW; // the class style
	wcex.lpfnWndProc = (WNDPROC)WndProc; // the window procedure callback
	wcex.cbClsExtra = 0; // extra bytes to allocate for this class
	wcex.cbWndExtra = 0; // extra bytes to allocate for this instance
	wcex.hInstance = hInstance; // handle to the application instance
	wcex.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_MyWindowIcon)); // icon to associate with the application
	wcex.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_GUNSIGHT));// the default cursor
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); // the background color
	wcex.lpszMenuName = NULL; // the resource name for the menu
	wcex.lpszClassName = "Balloons"; // the class name being created
	wcex.hIconSm = LoadIcon(hInstance,"KnightSprite.ico"); // the handle to the small icon

	RegisterClassEx(&wcex);
	// Create the window
	wndHandle = CreateWindow("Balloons",			// the window class to use
							 "Knight Of The Moving Platforms",			// the title bar text
							WS_OVERLAPPEDWINDOW,	// the window style
							CW_USEDEFAULT, // the starting x coordinate
							CW_USEDEFAULT, // the starting y coordinate
							800, // the pixel width of the window
							600, // the pixel height of the window
							NULL, // the parent window; NULL for desktop
							NULL, // the menu for the application; NULL for none
							hInstance, // the handle to the application instance
							NULL); // no values passed to the window
	// Make sure that the window handle that is created is valid

	if (!wndHandle)
		return false;
	// Display the window on the screen
	ShowWindow(wndHandle, SW_SHOW);
	UpdateWindow(wndHandle);
	return true;
}

/*
==================================================================
// This is winmain, the main entry point for Windows applications
==================================================================
*/
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	// Initialize the window
	if ( !initWindow( hInstance ) )
		return false;
	// called after creating the window
	if ( !d3dMgr->initD3DManager(wndHandle) )
		return false;
	if ( !d3dxSRMgr->initD3DXSpriteMgr(d3dMgr->getTheD3DDevice()))
		return false;

	// Grab the frequency of the high def timer
	__int64 freq = 0;				// measured in counts per second;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	float sPC = 1.0f / (float)freq;			// number of seconds per count

	__int64 currentTime = 0;				// current time measured in counts per second;
	__int64 previousTime = 0;				// previous time measured in counts per second;

	float numFrames   = 0.0f;				// Used to hold the number of frames
	float timeElapsed = 0.0f;				// cumulative elapsed time

	GetClientRect(wndHandle,&clientBounds);
	float fpsRate = 1000.0f/25.0f;
	/* initialize random seed: */
	srand ( (unsigned int)time(NULL) );

	D3DXVECTOR3 BalloonPos;
	/* generate random number of Balloons */
	int numBalloons = (50);
	D3DXVECTOR3 knightPos;
	POINT pknightPos;
	knightPos = D3DXVECTOR3((float)clientBounds.right/2-100,clientBounds.bottom-360,0);
	gKnight.push_back(new cKnight(knightPos,d3dMgr->getTheD3DDevice(),"Images\\KnightBodytwo.png"));
	for(int loop = 0; loop < numBalloons; loop++)
	{
		BalloonPos = D3DXVECTOR3(clientBounds.right/2-200+(loop*400),(float)clientBounds.bottom-100,0);
		aBalloon.push_back(new cBalloon(BalloonPos,d3dMgr->getTheD3DDevice(),"Images\\Platform.png"));
		aBalloon[loop]->setTranslation(D3DXVECTOR2(0.0f,0.0f));
	}


	LPDIRECT3DSURFACE9 aSurface;				// the Direct3D surface
	LPDIRECT3DSURFACE9 theBackbuffer = NULL;  // This will hold the back buffer
	
	D3DXVECTOR3 aBalloonPos;

	MSG msg;
	ZeroMemory( &msg, sizeof( msg ) );

	// Create the background surface
	aSurface = d3dMgr->getD3DSurfaceFromFile("Images\\SkyBackgroundSmall.png");

	while( msg.message!=WM_QUIT )
	{
		// Check the message queue
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			// Game code goes here
			BalloonPos = D3DXVECTOR3(BalloonTrans.x,BalloonTrans.y,0);
			
			/*
			==============================================================
			| Update the postion of the Balloons and check for collisions
			==============================================================
			*/

			for(iter = aBalloon.begin(); iter != aBalloon.end(); ++iter)
			{
				
				(*iter)->update();			// update Balloon
				aBalloonPos = (*iter)->getSpritePos();  // get the position of the current Balloon
				/*
				==============================================================
				| Check for boundry collision and change Balloon direction
				==============================================================
				*/
				/*
				if ((*iter)->getTranslation().x>0 && aBalloonPos.x>(clientBounds.right - 60))
				{
					(*iter)->setTranslation(D3DXVECTOR2(-((*iter)->getTranslation().x), (*iter)->getTranslation().y ));

				}
				if((*iter)->getTranslation().x<0 && aBalloonPos.x<(clientBounds.left + 10))
				{
					(*iter)->setTranslation( D3DXVECTOR2(-((*iter)->getTranslation().x), (*iter)->getTranslation().y ));

				}
				if ((*iter)->getTranslation().y<0 && aBalloonPos.y<(clientBounds.top))
				{
					(*iter)->setTranslation(D3DXVECTOR2((*iter)->getTranslation().x ,-((*iter)->getTranslation().y )));

				}
				if((*iter)->getTranslation().y>0 && aBalloonPos.y>(clientBounds.bottom-60))
				{
					(*iter)->setTranslation(D3DXVECTOR2((*iter)->getTranslation().x ,-((*iter)->getTranslation().y )));
				}
				*/
				/*
				for(index = aBalloon.begin(); index != aBalloon.end(); ++index)
				{
					if ((*iter)->collidedWith((*iter)->getBoundingRect(),(*index)->getBoundingRect()))
					{
						// if a collision occurs change the direction of each Balloon that has collided
						//OutputDebugString("Collision!!");
						(*iter)->setTranslation((*iter)->getTranslation()*(-1));
						(*index)->setTranslation((*index)->getTranslation()*(-1));
					}
				}*/
			}


			QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
			float dt = (currentTime - previousTime)*sPC;

			// Accumulate how much time has passed.
			timeElapsed = dt+timeElapsed;
			if(timeElapsed > fpsRate)
			{
				d3dMgr->beginRender();
				theBackbuffer = d3dMgr->getTheBackBuffer();
				d3dMgr->updateTheSurface(aSurface, theBackbuffer);
				d3dMgr->releaseTheBackbuffer(theBackbuffer);

				d3dxSRMgr->beginDraw();
				for(iter = aBalloon.begin(); iter != aBalloon.end(); ++iter)
				{
					d3dxSRMgr->setTheTransform((*iter)->getSpriteTransformMatrix());  
					d3dxSRMgr->drawSprite((*iter)->getTexture(),NULL,NULL,NULL,0xFFFFFFFF);
				}
				d3dxSRMgr->endDraw();
					
				d3dxSRMgr->beginDraw();
				list<cKnight*>::iterator bite = gKnight.begin();
				int frog=0;
				while(bite != gKnight.end())
				{
						(*bite)->setSpritePos(knightPos);
						(*bite)->update(dt);
						d3dxSRMgr->setTheTransform((*bite)->getSpriteTransformMatrix());  
						d3dxSRMgr->drawSprite((*bite)->getTexture(),&((*bite)->getSourceRect()),NULL,NULL,0xFFFFFFFF);
						++bite;
						int sexy;
						int frisky;
						float gravity=315;
						pknightPos.x = knightTrans.x;
						pknightPos.y = knightTrans.y;
						for(iter = aBalloon.begin(); iter != aBalloon.end(); ++iter)
						{
							/*sexy=(clientBounds.right/2+(frog*400));
							frisky=(499+clientBounds.right/2+(frog*400));
							if (BalloonPosition>sexy&&BalloonPosition<frisky){gravity=0;}*/
							if(((*iter)->insideRect((*iter)->getBoundingRect(),pknightPos)))
							{
								gravity = 0;
							}
							frog++; 
						}
						if (knightPos.y<(clientBounds.bottom-gravity)){knightTrans.y += 30;}
						knightPos = D3DXVECTOR3(knightTrans.x,knightTrans.y,0);
						

				}
				list<cExplosion*>::iterator iter = gExplode.begin();
				while(iter != gExplode.end())
				{
					if((*iter)->isActive() == false)
					{
						iter = gExplode.erase(iter);
						}
					else
					{
						(*iter)->update(dt);
						d3dxSRMgr->setTheTransform((*iter)->getSpriteTransformMatrix());  
						d3dxSRMgr->drawSprite((*iter)->getTexture(),&((*iter)->getSourceRect()),NULL,NULL,0xFFFFFFFF);
						++iter;
					}

				}
				previousTime = currentTime;
				d3dxSRMgr->endDraw();
				
			}
			d3dMgr->endRender();
			
		}
	}
	d3dxSRMgr->cleanUp();
	d3dMgr->clean();
	return (int) msg.wParam;
}
