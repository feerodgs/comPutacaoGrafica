#include "Scene5.h"

CScene5::CScene5()
{
	pCamera = NULL;
	pTexto = NULL;
	pTextures = NULL;
	bIsWireframe = false;
	bIsCameraFPS = true;

	iFPS = 0;
	iFrames = 0;
	ulLastFPS = 0;
	szTitle[256] = 0;

	// Cria gerenciador de impress�o de texto na tela
	pTexto = new CTexto();

	// Cria camera
	pCamera = new CCamera(0.0f, 25.0f, 20.0f);

	// Cria o Timer
	pTimer = new CTimer();
	pTimer->Init();

	fTimerPosY = 0.0f;
	fRenderPosY = 0.0f;

	// Carrega todas as texturas
	pTextures = new CTexture();
	pTextures->CreateTextureClamp(0, "../Scene1/back.bmp");
	pTextures->CreateTextureClamp(1, "../Scene1/front.bmp");
	pTextures->CreateTextureClamp(2, "../Scene1/down.bmp");
	pTextures->CreateTextureClamp(3, "../Scene1/up.bmp");
	pTextures->CreateTextureClamp(4, "../Scene1/left.bmp");
	pTextures->CreateTextureClamp(5, "../Scene1/right.bmp");
	pTextures->CreateTextureLinear(6, "../Scene1/water.bmp");
	pTextures->CreateTextureTGA(7, "../Scene1/tree1.tga");

	// Carrega a casa
	pCasa = NULL;
	pCasa = new CModel_3DS();
	pCasa->Load("../Scene5/Casa2.3ds");

	pPonte = NULL;
	pPonte = new CModel_3DS();
	pPonte->Load("../Scene5/Ponte.3ds");

	// Carrega o terreno
	pTerreno = NULL;
	pTerreno = new CModel_3DS();
	pTerreno->Load("../Scene5/Terreno.3ds");

	pStatue = NULL;
	pStatue = new CModel_3DS();
	pStatue->Load("../Scene5/Statue.3ds");

	enabledFog = false;

	// Configura��o das vari�veis para FOG
	fFogDensity = 0.01f;
	iFogMode = 0;
	vFogColor[0] = 0.7f;
	vFogColor[1] = 0.7f;
	vFogColor[2] = 1.0f;
	vFogColor[3] = 1.0f;


	// Configurando a POINT LIGHT
	fPointLightPosX = 10.0f;
	fPointLightPosY = 60.0f;
	fPointLightPosZ = 10.0f;
	PointLightAmbient[0] = 1.0f;	PointLightAmbient[1] = 1.0f;	PointLightAmbient[2] = 1.0f;	PointLightAmbient[3] = 1.0f;
	PointLightDiffuse[0] = 1.0f;	PointLightDiffuse[1] = 1.0f;	PointLightDiffuse[2] = 1.0f;	PointLightDiffuse[3] = 1.0f;
	PointLightSpecular[0] = 1.0f;	PointLightSpecular[1] = 1.0f;	PointLightSpecular[2] = 1.0f;	PointLightSpecular[3] = 1.0f;
	PointLightPosition[0] = fPointLightPosX;
	PointLightPosition[1] = fPointLightPosY;
	PointLightPosition[2] = fPointLightPosZ;
	PointLightPosition[3] = 1.0f;

	// Configurando a SPOT LIGHT
	fSpotLightPosX = 10.0f;
	fSpotLightPosY = 60.0f;
	fSpotLightPosZ = 10.0f;
	SpotLightAmbient[0] = 1.0f;	SpotLightAmbient[1] = 1.0f;	SpotLightAmbient[2] = 1.0f;	SpotLightAmbient[3] = 1.0f;
	SpotLightDiffuse[0] = 1.0f;	SpotLightDiffuse[1] = 1.0f;	SpotLightDiffuse[2] = 1.0f;	SpotLightDiffuse[3] = 1.0f;
	SpotLightSpecular[0] = 1.0f; SpotLightSpecular[1] = 1.0f;	SpotLightSpecular[2] = 1.0f;	SpotLightSpecular[3] = 1.0f;
	SpotLightDirection[0] = 0.0f;	SpotLightDirection[1] = -1.0f;	SpotLightDirection[2] = 0.0f;	SpotLightDirection[3] = 1.0f;
	fSpotAttenuation = 10.0f;
	fSpotCutOff = 45.0f;
	SpotLightPosition[0] = fSpotLightPosX;
	SpotLightPosition[1] = fSpotLightPosY;
	SpotLightPosition[2] = fSpotLightPosZ;
	SpotLightPosition[3] = 1.0f;

	bIsSpotActive = false;
	fLightSpeed = 0.2f;

}

CScene5::~CScene5(void)
{
	if (pTexto)
	{
		delete pTexto;
		pTexto = NULL;
	}

	if (pTextures)
	{
		delete pTextures;
		pTextures = NULL;
	}

	if (pCamera)
	{
		delete pCamera;
		pCamera = NULL;
	}

	if (pTimer)
	{
		delete pTimer;
		pTimer = NULL;
	}

	if (pCasa)
	{
		delete pCasa;
		pCasa = NULL;
	}

	if (pPonte)
	{
		delete pPonte;
		pPonte = NULL;
	}

	if (pTerreno)
	{
		delete pTerreno;
		pTerreno = NULL;
	}

	if (pStatue)
	{
		delete pStatue;
		pStatue = NULL;
	}
}

int CScene5::DrawGLScene(void) // Fun��o que desenha a cena
{
	// Get FPS
	if (GetTickCount() - ulLastFPS >= 1000) // When A Second Has Passed...
	{
		ulLastFPS = GetTickCount(); // Update Our Time Variable
		iFPS = iFrames;				// Save The FPS
		iFrames = 0;				// Reset The FPS Counter
	}
	iFrames++; // FPS counter

	pTimer->Update(); // Atualiza o timer

	glClearColor(vFogColor[0], vFogColor[1], vFogColor[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpa a tela e o Depth Buffer
	glLoadIdentity();									// Inicializa a Modelview Matrix Atual

	// Seta as posi��es da c�mera
	pCamera->setView();

	// Modo FILL ou WIREFRAME (pressione barra de espa�o)
	if (bIsWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (IN�CIO)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	// Habilita mapeamento de texturas 2D
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	// Configura fun��o de Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (bIsSpotActive)
	{
		// Configura os parâmetros para a fonte de luz (Emissora)
		glLightfv(GL_LIGHT1, GL_AMBIENT, SpotLightAmbient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, SpotLightDiffuse);
		glLightfv(GL_LIGHT1, GL_SPECULAR, SpotLightSpecular);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, SpotLightDirection);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, fSpotAttenuation);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, fSpotCutOff);

		SpotLightPosition[0] = fSpotLightPosX;
		SpotLightPosition[1] = fSpotLightPosY;
		SpotLightPosition[2] = fSpotLightPosZ;
		SpotLightPosition[3] = 1.0f;
		glLightfv(GL_LIGHT1, GL_POSITION, SpotLightPosition);

		// Desenha a lâmpada
		glPushMatrix();
		glTranslatef(fSpotLightPosX, fSpotLightPosY, fSpotLightPosZ);
		glutSolidSphere(0.5, 10, 10);
		glPopMatrix();

		glEnable(GL_LIGHT1);	// Liga a Luz 0
	}
	else
	{
		// Configura os parâmetros para a fonte de luz (Emissora)
		glLightfv(GL_LIGHT0, GL_AMBIENT, PointLightAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, PointLightDiffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, PointLightSpecular);

		PointLightPosition[0] = fPointLightPosX;
		PointLightPosition[1] = fPointLightPosY;
		PointLightPosition[2] = fPointLightPosZ;
		PointLightPosition[3] = 1.0f;
		glLightfv(GL_LIGHT0, GL_POSITION, PointLightPosition);

		// Desenha a lâmpada
		glPushMatrix();
		glTranslatef(fPointLightPosX, fPointLightPosY, fPointLightPosZ);
		glutSolidSphere(0.5, 10, 10);
		glPopMatrix();

		glEnable(GL_LIGHT0);	// Liga a Luz 0
	}

	glEnable(GL_LIGHTING); // Habilita iluminação




	if (enabledFog == true)
	{
		glEnable(GL_FOG);				  // Habilita Neblina (FOG)
		glHint(GL_FOG_HINT, GL_NICEST);	  // Qualidade da FOG
		glFogfv(GL_FOG_COLOR, vFogColor); // Cor da FOG
		glFogf(GL_FOG_START, 10.0);		  // In�cio da FOG a partir da camera
		glFogf(GL_FOG_END, 1000.0);		  // Limite dos objetos vis�veis

		switch (iFogMode)
		{
		case 0:
			glFogi(GL_FOG_MODE, GL_LINEAR); // Tipo da FOG
			break;

		case 1:
			glFogi(GL_FOG_MODE, GL_EXP); // Tipo da FOG
			glFogf(GL_FOG_DENSITY, fFogDensity);
			break;

		case 2:
			glFogi(GL_FOG_MODE, GL_EXP2); // Tipo da FOG
			glFogf(GL_FOG_DENSITY, fFogDensity);
			break;

		default:
			break;
		}
	}
	else
	{
		// Desenha o SkyBox
		CreateSkyBox(0.0f, 300.0f, 0.0f,
					 2000.0f, 2000.0f, 2000.0f,
					 pTextures);
	}


	//  Desenha a casa
	glPushMatrix();
	glTranslatef(-45.0f, -2.0f, 5.0f);
	glRotatef(180.0f, 0.0f, 1.0f, 0.0f); 
	pCasa->Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 20.0f, -30.0f);
	pStatue->Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, -3.0f, 0.0f);
	pPonte->Draw();
	glPopMatrix();

	//  Desenha o terreno
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	pTerreno->Draw();
	glPopMatrix();

	// Habilita Blending


	DrawArvore(10.0f, 25.0f, -37.0f);
	DrawArvore(12.0f, 25.0f, -24.0f);
	DrawArvore(-13.0f, 25.0f, -21.0f);
	DrawArvore(-44.0f, 25.0f, -29.0f);
	DrawArvore(33.0f, 25.0f, -24.0f);
	DrawArvore(43.0f, 25.0f, -42.0f);
	DrawArvore(-32.0f, 25.0f, -24.0f);
	DrawArvore(-22.0f, 25.0f, -38.0f);
	DrawArvore(31.0f, 25.0f, -18.0f);

	DrawRio();

	if (enabledFog == true)
	{
		glDisable(GL_FOG);
	}

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	if (bIsSpotActive)
		glDisable(GL_LIGHT1);	// Desliga a Luz 1
	else
		glDisable(GL_LIGHT0);	// Desliga a Luz 0

	glDisable(GL_LIGHTING); // Desabilita iluminação

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (FIM)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	fTimerPosY = pTimer->GetTime() / 1000.0f;
	fRenderPosY += 0.0005f;

	// Impress�o de texto na tela...
	// Muda para modo de proje��o ortogonal 2D
	// OBS: Desabilite Texturas e Ilumina��o antes de entrar nesse modo de proje��o
	OrthoMode(0, 0, WIDTH, HEIGHT);

	glPushMatrix();
	glTranslatef(0.0f, HEIGHT - 100, 0.0f); // Move 1 unidade para dentro da tela (eixo Z)

	// Cor da fonte
	glColor3f(1.0f, 1.0f, 0.0f);

	glRasterPos2f(10.0f, 0.0f); // Posicionando o texto na tela

	if (enabledFog)
		pTexto->glPrint("[ENTER] FOG: TRUE - [F] FOG MODE: %d", iFogMode);
	else
		pTexto->glPrint("[ENTER] FOG: FALSE");

	if (enabledFog)
	{
		glRasterPos2f(10.0f, 120.0f);
		pTexto->glPrint("[+/-] FOG Density: %.3f", fFogDensity);
	}

	if (!bIsWireframe)
	{
		pTexto->glPrint("[TAB]  Modo LINE"); // Imprime texto na tela
	}
	else
	{
		pTexto->glPrint("[TAB]  Modo FILL");
	}

	//// Camera LookAt
	glRasterPos2f(10.0f, 40.0f);
	pTexto->glPrint("Player LookAt  : %f, %f, %f", pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);

	//// Posi��o do Player
	glRasterPos2f(10.0f, 60.0f);
	pTexto->glPrint("Player Position: %f, %f, %f", pCamera->Position[0], pCamera->Position[1], pCamera->Position[2]);

	//// Imprime o FPS da aplica��o e o Timer
	glRasterPos2f(10.0f, 80.0f);
	pTexto->glPrint("Frames-per-Second: %d ---- Timer: %.1f segundos", iFPS, (pTimer->GetTime() / 1000));

	glPopMatrix();

	// Muda para modo de proje��o perspectiva 3D
	PerspectiveMode();

	return true;
}

void CScene5::MouseMove(void) // Tratamento de movimento do mouse
{
	// Realiza os c�lculos de rota��o da vis�o do Player (atrav�s das coordenadas
	// X do mouse.
	POINT mousePos;
	int middleX = WIDTH >> 1;
	int middleY = HEIGHT >> 1;

	GetCursorPos(&mousePos);

	if ((mousePos.x == middleX) && (mousePos.y == middleY))
		return;

	SetCursorPos(middleX, middleY);

	fDeltaX = (float)((middleX - mousePos.x)) / 10;
	fDeltaY = (float)((middleY - mousePos.y)) / 10;

	// Rotaciona apenas a c�mera
	pCamera->rotateGlob(-fDeltaX, 0.0f, 1.0f, 0.0f);
	pCamera->rotateLoc(-fDeltaY, 1.0f, 0.0f, 0.0f);
}

void CScene5::KeyPressed(void) // Tratamento de teclas pressionadas
{

	// Verifica se a tecla 'W' foi pressionada e move o Player para frente
	if (GetKeyState('W') & 0x80)
	{
		pCamera->moveGlob(pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);
	}
	// Verifica se a tecla 'S' foi pressionada e move o Player para tras
	else if (GetKeyState('S') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Forward[0], -pCamera->Forward[1], -pCamera->Forward[2]);
	}
	// Verifica se a tecla 'A' foi pressionada e move o Player para esquerda
	else if (GetKeyState('A') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Right[0], -pCamera->Right[1], -pCamera->Right[2]);
	}
	// Verifica se a tecla 'D' foi pressionada e move o Player para direira
	else if (GetKeyState('D') & 0x80)
	{
		pCamera->moveGlob(pCamera->Right[0], pCamera->Right[1], pCamera->Right[2]);
	}
	else if (GetKeyState('Q') & 0x80)
	{
		pCamera->moveGlob(0.0f, -pCamera->Up[1], 0.0f);
	}
	else if (GetKeyState('E') & 0x80)
	{
		pCamera->moveGlob(0.0f, pCamera->Up[1], 0.0f);
	}
	// Sen�o, interrompe movimento do Player
	else
	{
	}

	if (GetKeyState(VK_UP) & 0x80)
	{
		if (bIsSpotActive)
			fSpotLightPosZ -= fLightSpeed;
		else
			fPointLightPosZ -= fLightSpeed;
	}
	if (GetKeyState(VK_DOWN) & 0x80)
	{
		if (bIsSpotActive)
			fSpotLightPosZ += fLightSpeed;
		else
			fPointLightPosZ += fLightSpeed;
	}
	if (GetKeyState(VK_LEFT) & 0x80)
	{
		if (bIsSpotActive)
			fSpotLightPosX -= fLightSpeed;
		else
			fPointLightPosX -= fLightSpeed;
	}
	if (GetKeyState(VK_RIGHT) & 0x80)
	{
		if (bIsSpotActive)
			fSpotLightPosX += fLightSpeed;
		else
			fPointLightPosX += fLightSpeed;
	}
	if (GetKeyState(VK_PRIOR) & 0x80)
	{
		if (bIsSpotActive)
			fSpotLightPosY += fLightSpeed;
		else
			fPointLightPosY += fLightSpeed;
	}
	if (GetKeyState(VK_NEXT) & 0x80)
	{
		if (bIsSpotActive)
			fSpotLightPosY -= fLightSpeed;
		else
			fPointLightPosY -= fLightSpeed;
	}

}

void CScene5::KeyDownPressed(WPARAM wParam) // Tratamento de teclas pressionadas
{
	switch (wParam)
	{
	case VK_TAB:
		bIsWireframe = !bIsWireframe;
		break;

	case VK_SPACE:
		pTimer->Init();
		break;

	case VK_F1:
		enabledFog = !enabledFog;
		break;

	case VK_F2:
		fFogDensity += 0.001f;
		break;

	case VK_F3:
		fFogDensity -= 0.001f;
		break;
	case 'F':
		iFogMode++;
		if (iFogMode > 2)
			iFogMode = 0;
		break;
	case VK_F5:
		bIsSpotActive = !bIsSpotActive;
		break;

	}
}



void CScene5::CreateSkyBox(float x, float y, float z,
						   float width, float height, float length,
						   CTexture *pTextures)
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glPushMatrix();

	// Centraliza o Skybox em torno da posi��o especificada(x, y, z)
	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;

	// Aplica a textura que representa a parte da frente do skybox (BACK map)
	pTextures->ApplyTexture(0);

	// Desenha face BACK do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x, y + height, z);
	glEnd();

	// Aplica a textura que representa a parte da frente do skybox (FRONT map)
	pTextures->ApplyTexture(1);

	// Desenha face FRONT do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x + width, y, z + length);
	glEnd();

	// Aplica a textura que representa a parte da frente do skybox (DOWN map)
	pTextures->ApplyTexture(2);

	// Desenha face BOTTOM do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x, y, z + length);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x + width, y, z);
	glEnd();

	// Aplica a textura que representa a parte da frente do skybox (UP map)
	pTextures->ApplyTexture(3);

	// Desenha face TOP do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x, y + height, z + length);
	glEnd();

	// Aplica a textura que representa a parte da frente do skybox (LEFT map)
	pTextures->ApplyTexture(4);

	// Desenha face LEFT do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x, y, z + length);
	glEnd();

	// Aplica a textura que representa a parte da frente do skybox (RIGHT map)
	pTextures->ApplyTexture(5);

	// Desenha face RIGHT do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x + width, y + height, z);
	glEnd();

	glPopMatrix();
}

void CScene5::DrawArvore(float x, float y, float z)
{

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5);
	pTextures->ApplyTexture(7);
	glPushMatrix();
	glTranslatef(x, y, z);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-10.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(10.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(10.0f, 15.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-10.0f, 15.0f, 0.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 10.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -10.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 15.0f, -10.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 15.0f, 10.0f);
	glEnd();
	glPopMatrix();
	glDisable(GL_ALPHA_TEST);
}

void CScene5::DrawRio()
{
	// Desenha o rio
	glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
	glPushAttrib(GL_TEXTURE_BIT);

	// Unidade de Textura 0
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	pTextures->ApplyTexture(6);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);

	// Unidade de Textura 1
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	pTextures->ApplyTexture(6);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);

	glPushMatrix();
	glTranslatef(0.0f, 20.0f, 0.0f);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);

	// Coordenadas de textura fixas (sem uso de fRenderPosY)
	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 0.0f);
	glVertex3f(-50.0f, 0.0f, 25.0f);

	glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 0.0f);
	glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 0.0f);
	glVertex3f(50.0f, 0.0f, 25.0f);

	glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 1.0f);
	glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 1.0f);
	glVertex3f(50.0f, 0.0f, -25.0f);

	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 1.0f);
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 1.0f);
	glVertex3f(-50.0f, 0.0f, -25.0f);
	glEnd();
	glPopMatrix();

	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);
	glPopAttrib();
}




