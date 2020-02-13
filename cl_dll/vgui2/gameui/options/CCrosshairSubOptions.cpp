#include <KeyValues.h>
#include <vgui/IImage.h>
#include <vgui/ISurface.h>
#include <vgui_controls/ImagePanel.h>
#include "CCrosshairSubOptions.h"
#include "vgui2/VGUI2Paths.h"
#include "CCvarCheckButton.h"
#include "CCvarTextEntry.h"
#include "CCvarColor.h"
#include "hud.h"

class CCrosshairImage : public vgui2::IImage
{
public:
	static constexpr int IMG_SIZE = 96;

	SDK_Color m_CrossColor = SDK_Color(0, 0, 0, 255);
	int m_iGap = 0;
	int m_iSize = 0;
	int m_iThickness = 0;
	int m_iOutline = 0;
	bool m_bDot = false;
	bool m_bT = false;

	CCrosshairImage()
	{
		m_wide = IMG_SIZE;
		m_tall = IMG_SIZE;

		if (m_sTexture == -1)
		{
			m_sTexture = vgui2::surface()->CreateNewTextureID(true);
			vgui2::surface()->DrawSetTextureFile(m_sTexture, UI_BASE_DIR "/gfx/cross_bg", false, false);
		}
	}

	// Call to Paint the image
	// Image will draw within the current panel context at the specified position
	virtual void Paint()
	{
		int posX = m_nX + m_offX;
		int posY = m_nY + m_offY;

		if (m_sTexture != -1)
		{
			vgui2::surface()->DrawSetTexture(m_sTexture);
			vgui2::surface()->DrawSetColor(m_Color);
			vgui2::surface()->DrawTexturedRect(posX, posY, posX + m_wide, posY + m_tall);
		}

		int cx = IMG_SIZE / 2;
		int cy = IMG_SIZE / 2;
		int gap = m_iGap / 2;
		int thick = m_iThickness;
		int size = m_iSize;
		bool t = m_bT;
		int outline = m_iOutline;

		// Draw outline
		if (outline > 0)
		{
			// Don't read that if you don't want eye cancer
			vgui2::surface()->DrawSetColor(0, 0, 0, 255);
			vgui2::surface()->DrawFilledRect(cx + gap - outline, cy - thick / 2 - outline,
				cx + gap - outline + size + outline * 2, cy - thick / 2 - outline + thick + outline * 2);
			vgui2::surface()->DrawFilledRect(cx - gap - size - outline, cy - thick / 2 - outline,
				cx - gap - size - outline + size + outline * 2, cy - thick / 2 - outline + thick + outline * 2);
			vgui2::surface()->DrawFilledRect(cx - thick / 2 - outline, cy + gap - outline,
				cx - thick / 2 - outline + thick + outline * 2, cy + gap - outline + size + outline * 2);
			vgui2::surface()->DrawFilledRect(cx - thick / 2 - outline, cy - gap - size - outline,
				cx - thick / 2 - outline + thick + outline * 2, cy - gap - size - outline + size + outline * 2);
		}
		vgui2::surface()->DrawSetColor(m_CrossColor);

		// Draw dot
		if (m_bDot)
		{
			vgui2::surface()->DrawFilledRect(cx - thick / 2, cy - thick / 2, thick + cx - thick / 2, thick + cy - thick / 2);
		}

		// Draw crosshair
		vgui2::surface()->DrawFilledRect(cx + gap, cy - thick / 2, cx + gap + size, cy - thick / 2 + thick);
		vgui2::surface()->DrawFilledRect(cx - gap - size, cy - thick / 2, cx - gap, cy - thick / 2 + thick);
		vgui2::surface()->DrawFilledRect(cx - thick / 2, cy + gap, cx - thick / 2 + thick, cy + gap + size);
		if (!t)
			vgui2::surface()->DrawFilledRect(cx - thick / 2, cy - gap - size, cx - thick / 2 + thick, cy - gap);
	}

	// Set the position of the image
	virtual void SetPos(int x, int y)
	{
		m_nX = x;
		m_nY = y;
	}

	virtual void SetOffset(int x, int y)
	{
		m_offX = x;
		m_offY = y;
	}

	// Gets the size of the content
	virtual void GetContentSize(int &wide, int &tall)
	{
		wide = m_wide;
		tall = m_tall;
	}

	// Get the size the image will actually draw in (usually defaults to the content size)
	virtual void GetSize(int &wide, int &tall)
	{
		GetContentSize(wide, tall);
	}

	// Sets the size of the image
	virtual void SetSize(int wide, int tall)
	{
		m_wide = wide;
		m_tall = tall;
	}

	// Set the draw color
	virtual void SetColor(SDK_Color col)
	{
		m_Color = col;
	}

	virtual int	GetWide()
	{
		return m_wide;
	}

	virtual int	GetTall()
	{
		return m_tall;
	}

protected:
	SDK_Color m_Color;
	int m_nX = 0, m_nY = 0;
	int m_wide = 0, m_tall = 0;
	int m_offX = 0, m_offY = 0;

	static int m_sTexture;
};

int CCrosshairImage::m_sTexture = -1;

CCrosshairSubOptions::CCrosshairSubOptions(vgui2::Panel *parent) :
	BaseClass(parent, nullptr)
{
	m_pEnableCvar = new CCvarCheckButton(this, "EnableCvar", "#BHL_AdvOptions_Cross_Enable", "cl_crosshair_custom");
	
	m_pColorLabel = new vgui2::Label(this, "ColorLabel", "#BHL_AdvOptions_Cross_Color");
	m_pColorCvar = new CCvarColor(this, "ColorCvar", nullptr, "#BHL_AdvOptions_Cross_Color_Title");
	
	m_pGapLabel = new vgui2::Label(this, "GapLabel", "#BHL_AdvOptions_Cross_Gap");
	m_pGapCvar = new CCvarTextEntry(this, "GapCvar", "cl_crosshair_gap", CCvarTextEntry::CvarType::Int);
	
	m_pSizeLabel = new vgui2::Label(this, "SizeLabel", "#BHL_AdvOptions_Cross_Size");
	m_pSizeCvar = new CCvarTextEntry(this, "SizeCvar", "cl_crosshair_size", CCvarTextEntry::CvarType::Int);
	
	m_pThicknessLabel = new vgui2::Label(this, "ThicknessLabel", "#BHL_AdvOptions_Cross_Thickness");
	m_pThicknessCvar = new CCvarTextEntry(this, "ThicknessCvar", "cl_crosshair_thickness", CCvarTextEntry::CvarType::Int);
	
	m_pOutlineThicknessLabel = new vgui2::Label(this, "OutlineThickness", "#BHL_AdvOptions_Cross_OutThickness");
	m_pOutlineThicknessCvar = new CCvarTextEntry(this, "OutlineThicknessCvar", "cl_crosshair_outline_thickness", CCvarTextEntry::CvarType::Int);
	
	m_pDotCvar = new CCvarCheckButton(this, "DotCvar", "#BHL_AdvOptions_Cross_Dot", "cl_crosshair_dot");
	
	m_pTCvar = new CCvarCheckButton(this, "TCvar", "#BHL_AdvOptions_Cross_T", "cl_crosshair_t");

	m_pColors[0] = gEngfuncs.pfnGetCvarPointer("cl_crosshair_red");
	m_pColors[1] = gEngfuncs.pfnGetCvarPointer("cl_crosshair_green");
	m_pColors[2] = gEngfuncs.pfnGetCvarPointer("cl_crosshair_blue");

	m_pPreviewImage = new CCrosshairImage();
	m_pPreviewPanel = new vgui2::ImagePanel(this, "PreviewPanel");
	m_pPreviewPanel->SetImage(m_pPreviewImage);

	LoadControlSettings(UI_RESOURCE_DIR "/options/CrosshairSubOptions.res");
}

CCrosshairSubOptions::~CCrosshairSubOptions()
{
	m_pPreviewPanel->SetImage(static_cast<vgui2::IImage *>(nullptr));
	delete m_pPreviewImage;
}

void CCrosshairSubOptions::ApplySchemeSettings(vgui2::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);
	m_pPreviewPanel->SetBorder(pScheme->GetBorder("DepressedBorder"));
}

void CCrosshairSubOptions::OnResetData()
{
	m_pEnableCvar->ResetData();
	m_pGapCvar->ResetData();
	m_pSizeCvar->ResetData();
	m_pThicknessCvar->ResetData();
	m_pOutlineThicknessCvar->ResetData();
	m_pDotCvar->ResetData();
	m_pTCvar->ResetData();

	SDK_Color color;
	color.SetColor(m_pColors[0]->value, m_pColors[1]->value, m_pColors[2]->value, 255);
	m_pColorCvar->SetInitialColor(color);

	// Reset crosshair preview
	m_pPreviewImage->m_CrossColor = m_pColorCvar->GetSelectedColor();
	m_pPreviewImage->m_iGap = m_pGapCvar->GetInt();
	m_pPreviewImage->m_iSize = m_pSizeCvar->GetInt();
	m_pPreviewImage->m_iThickness = m_pThicknessCvar->GetInt();
	m_pPreviewImage->m_iOutline = m_pOutlineThicknessCvar->GetInt();
	m_pPreviewImage->m_bDot = m_pDotCvar->IsSelected();
	m_pPreviewImage->m_bT = m_pTCvar->IsSelected();
}

void CCrosshairSubOptions::OnApplyChanges()
{
	m_pEnableCvar->ApplyChanges();
	m_pGapCvar->ApplyChanges();
	m_pSizeCvar->ApplyChanges();
	m_pThicknessCvar->ApplyChanges();
	m_pOutlineThicknessCvar->ApplyChanges();
	m_pDotCvar->ApplyChanges();
	m_pTCvar->ApplyChanges();

	char buf[128];
	SDK_Color color = m_pColorCvar->GetSelectedColor();
	m_pColorCvar->SetInitialColor(color);
	
	for (int i = 0; i < 3; i++)
	{
		snprintf(buf, sizeof(buf), "%s \"%d\"", m_pColors[i]->name, color[i]);
		gEngfuncs.pfnClientCmd(buf);
	}
}

void CCrosshairSubOptions::OnColorPicked(KeyValues *kv)
{
	void *panel = kv->GetPtr("panel");
	if (panel == m_pColorCvar)
		m_pPreviewImage->m_CrossColor.SetRawColor(kv->GetInt("color"));
}

void CCrosshairSubOptions::OnCvarTextChanged(KeyValues *kv)
{
	void *panel = kv->GetPtr("panel");
	if (panel == m_pGapCvar)
	{
		m_pPreviewImage->m_iGap = kv->GetInt("value");
	}
	else if (panel == m_pSizeCvar)
	{
		m_pPreviewImage->m_iSize = kv->GetInt("value");
	}
	else if (panel == m_pThicknessCvar)
	{
		m_pPreviewImage->m_iThickness = kv->GetInt("value");
	}
	else if (panel == m_pOutlineThicknessCvar)
	{
		m_pPreviewImage->m_iOutline = kv->GetInt("value");
	}
}

void CCrosshairSubOptions::OnCheckButtonChecked(KeyValues *kv)
{
	void *panel = kv->GetPtr("panel");
	if (panel == m_pDotCvar)
	{
		m_pPreviewImage->m_bDot = kv->GetInt("state");
	}
	else if (panel == m_pTCvar)
	{
		m_pPreviewImage->m_bT = kv->GetInt("state");
	}
}
