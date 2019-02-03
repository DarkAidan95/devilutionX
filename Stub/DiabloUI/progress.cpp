#include "../../types.h"

Art ArtPopupSm;
Art ArtProgBG;
Art ProgFil;
Art ButImage;
SDL_Surface *msgSurface;
SDL_Surface *cancleSurface;
int textWidth;

void progress_Loade(char *msg)
{
	LoadBackgroundArt("ui_art\\black.pcx");
	LoadArt("ui_art\\spopup.pcx", &ArtPopupSm);
	LoadArt("ui_art\\prog_bg.pcx", &ArtProgBG);
	LoadArt("ui_art\\prog_fil.pcx", &ProgFil);
	LoadArt("ui_art\\but_sml.pcx", &ButImage, 15);

	if (font != NULL) {
		SDL_Color color = { 243, 243, 243 };

		msgSurface = TTF_RenderUTF8_Solid(font, msg, color);
		cancleSurface = TTF_RenderUTF8_Solid(font, "Cancel", color);
		TTF_SizeUTF8(font, "Cancel", &textWidth, NULL);
	}
}

void progress_Free()
{
	mem_free_dbg(ArtBackground.data);
	ArtBackground.data = NULL;
	mem_free_dbg(ArtPopupSm.data);
	ArtPopupSm.data = NULL;
	mem_free_dbg(ArtProgBG.data);
	ArtProgBG.data = NULL;
	mem_free_dbg(ProgFil.data);
	ProgFil.data = NULL;
	mem_free_dbg(ButImage.data);
	ButImage.data = NULL;
	SDL_FreeSurface(msgSurface);
	msgSurface = NULL;
	SDL_FreeSurface(cancleSurface);
	cancleSurface = NULL;
}

void progress_Render(BYTE progress)
{
	DrawArt(0, 0, &ArtBackground);

	int x = GetCenterOffset(280);
	int y = GetCenterOffset(144, SCREEN_HEIGHT);

	DrawArt(x, y, &ArtPopupSm);
	DrawArt(GetCenterOffset(227), y + 52, &ArtProgBG, 0, 227);
	if (progress) {
		DrawArt(GetCenterOffset(227), y + 52, &ProgFil, 0, 227 * progress / 100);
	}
	DrawArt(GetCenterOffset(110), y + 99, &ButImage, 2, 110);

	if (msgSurface) {
		SDL_Rect dsc_rect = { 64 + x + 50, 160 + y + 8, SCREEN_WIDTH, SCREEN_HEIGHT };
		SDL_BlitSurface(msgSurface, NULL, pal_surface, &dsc_rect);
		dsc_rect.x = 64 + GetCenterOffset(textWidth) - 1;
		dsc_rect.y = 160 + y + 99 + 4;
		SDL_BlitSurface(cancleSurface, NULL, pal_surface, &dsc_rect);
	}
}

int __stdcall UiProgressDialog(HWND window, char *msg, int enable, int(__cdecl *fnfunc)(), int rate)
{
	progress_Loade(msg);

	bool endMenu = false;
	int progress = 0;

	SDL_Event event;
	while (!endMenu && progress < 100) {
		CapFPS();

		progress = fnfunc();

		progress_Render(progress);
		DrawMouse();
		SetFadeLevel(256);

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
			case SDL_MOUSEBUTTONDOWN:
				endMenu = true;
				break;
			case SDL_QUIT:
				exit(0);
			}
		}
	}
	BlackPalette();

	progress_Free();

	return progress == 100;
}
