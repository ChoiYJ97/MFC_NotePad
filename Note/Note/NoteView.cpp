
// NoteView.cpp: CNoteView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Note.h"
#endif

#include "NoteDoc.h"
#include "NoteView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNoteView

IMPLEMENT_DYNCREATE(CNoteView, CEditView)

BEGIN_MESSAGE_MAP(CNoteView, CEditView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CNoteView 생성/소멸

CNoteView::CNoteView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CNoteView::~CNoteView()
{
}

BOOL CNoteView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// 자동 줄바꿈을 사용합니다.

	return bPreCreated;
}

void CNoteView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CNoteView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CNoteView 진단

#ifdef _DEBUG
void CNoteView::AssertValid() const
{
	CEditView::AssertValid();
}

void CNoteView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CNoteDoc* CNoteView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNoteDoc)));
	return (CNoteDoc*)m_pDocument;
}
#endif //_DEBUG


// CNoteView 메시지 처리기
