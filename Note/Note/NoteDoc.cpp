
// NoteDoc.cpp: CNoteDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Note.h"
#endif

#include "NoteDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CNoteDoc

IMPLEMENT_DYNCREATE(CNoteDoc, CDocument)

BEGIN_MESSAGE_MAP(CNoteDoc, CDocument)
	ON_COMMAND(ID_EDIT_RED, &CNoteDoc::OnEditRed)
END_MESSAGE_MAP()


// CNoteDoc 생성/소멸

CNoteDoc::CNoteDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CNoteDoc::~CNoteDoc()
{
}

BOOL CNoteDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	if (!m_viewList.IsEmpty())
	{
		reinterpret_cast<CEditView*>(m_viewList.GetHead())->SetWindowText(nullptr);
	}

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CNoteDoc serialization

void CNoteDoc::Serialize(CArchive& ar)
{
	// CEditView에는 모든 serialization을 처리하는 edit 컨트롤이 들어 있습니다.
	if (!m_viewList.IsEmpty())
	{
		reinterpret_cast<CEditView*>(m_viewList.GetHead())->SerializeRaw(ar);
	}
#ifdef SHARED_HANDLERS

	if (m_viewList.IsEmpty() && ar.IsLoading())
	{
		CFile* pFile = ar.GetFile();
		pFile->Seek(0, FILE_BEGIN);
		ULONGLONG nFileSizeBytes = pFile->GetLength();
		ULONGLONG nFileSizeChars = nFileSizeBytes/sizeof(TCHAR);
		LPTSTR lpszText = (LPTSTR)malloc(((size_t)nFileSizeChars + 1) * sizeof(TCHAR));
		if (lpszText != nullptr)
		{
			ar.Read(lpszText, (UINT)nFileSizeBytes);
			lpszText[nFileSizeChars] = '\0';
			m_strThumbnailContent = lpszText;
			m_strSearchContent = lpszText;
		}
	}
#endif
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CNoteDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));
	pDC.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(m_strThumbnailContent, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
	
	CFont* pOldFont = pDC.SelectObject(&fontDraw);
	pDC.DrawText(m_strThumbnailContent, lprcBounds, DT_CENTER | DT_WORDBREAK);
	pDC.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CNoteDoc::InitializeSearchContent()
{
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 전체 텍스트 파일 콘텐츠를 검색 콘텐츠로 사용합니다.
	SetSearchContent(m_strSearchContent);
}

void CNoteDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CNoteDoc 진단

#ifdef _DEBUG
void CNoteDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNoteDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CNoteDoc 명령

void CNoteDoc::OnEditRed()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	pDC->SetTextColor(RGB(255, 0, 0));
}
