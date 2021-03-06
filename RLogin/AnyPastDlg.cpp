// AnyPastDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "RLogin.h"
#include "MainFrm.h"
#include "RLoginDoc.h"
#include "RLoginView.h"
#include "AnyPastDlg.h"

// CAnyPastDlg ダイアログ

IMPLEMENT_DYNAMIC(CAnyPastDlg, CDialogExt)

CAnyPastDlg::CAnyPastDlg(CWnd* pParent /*=NULL*/)
	: CDialogExt(CAnyPastDlg::IDD, pParent)
{
	m_EditText.Empty();
	m_NoCheck = FALSE;
	m_bUpdateText = FALSE;
	m_bDelayPast  = FALSE;
	m_bUpdateEnable = FALSE;
	m_CtrlCode[0] = m_CtrlCode[1] = m_CtrlCode[2] = 0;
	m_pView = NULL;
	m_bCtrlView = FALSE;
}

CAnyPastDlg::~CAnyPastDlg()
{
}

void CAnyPastDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogExt::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICONBOX, m_IconBox);
	DDX_Check(pDX, IDC_CHECK1, m_NoCheck);
	DDX_Check(pDX, IDC_CHECK2, m_bDelayPast);
	DDX_Check(pDX, IDC_CHECK3, m_bUpdateText);
	DDX_Text(pDX, IDC_CTRLCODE1, m_CtrlCode[0]);
	DDX_Text(pDX, IDC_CTRLCODE2, m_CtrlCode[1]);
	DDX_Text(pDX, IDC_CTRLCODE3, m_CtrlCode[2]);
	DDX_Control(pDX, IDC_TEXTBOX, m_EditWnd);
}

BEGIN_MESSAGE_MAP(CAnyPastDlg, CDialogExt)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_EN_CHANGE(IDC_TEXTBOX, &CAnyPastDlg::OnUpdateEdit)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CTRLCONV1, IDC_CTRLCONV3, &CAnyPastDlg::OnCtrlConv)
	ON_BN_CLICKED(IDC_SHELLESC, &CAnyPastDlg::OnShellesc)
	ON_BN_CLICKED(IDC_ONELINE, &CAnyPastDlg::OnOneLine)
	ON_BN_CLICKED(IDC_CHECK4, &CAnyPastDlg::OnCtrlView)
END_MESSAGE_MAP()

static const INITDLGTAB ItemTab[] = {
	{ IDC_SHELLESC,		ITM_LEFT_MID | ITM_RIGHT_MID | ITM_TOP_BTM | ITM_BTM_BTM },
	{ IDC_ONELINE,		ITM_LEFT_MID | ITM_RIGHT_MID | ITM_TOP_BTM | ITM_BTM_BTM },
	{ IDOK,				ITM_LEFT_MID | ITM_RIGHT_MID | ITM_TOP_BTM | ITM_BTM_BTM },
	{ IDCANCEL,			ITM_LEFT_MID | ITM_RIGHT_MID | ITM_TOP_BTM | ITM_BTM_BTM },
	{ IDC_TEXTBOX,		ITM_RIGHT_RIGHT | ITM_BTM_BTM },
	{ IDC_CHECK1,		ITM_LEFT_RIGHT | ITM_RIGHT_RIGHT },
	{ IDC_CHECK2,		ITM_LEFT_RIGHT | ITM_RIGHT_RIGHT },
	{ IDC_CHECK3,		ITM_LEFT_RIGHT | ITM_RIGHT_RIGHT },
	{ IDC_CHECK4,		ITM_RIGHT_RIGHT| ITM_TOP_BTM | ITM_BTM_BTM },

	{ IDC_CTRLTITLE1,	0 },
	{ IDC_CTRLTITLE2,	0 },
	{ IDC_CTRLTITLE3,	0 },
	{ IDC_CTRLCODE1,	0 },
	{ IDC_CTRLCODE2,	0 },
	{ IDC_CTRLCODE3,	0 },
	{ IDC_CTRLCONV1,	0 },
	{ IDC_CTRLCONV2,	0 },
	{ IDC_CTRLCONV3,	0 },

	{ 0,	0 },
};

void CAnyPastDlg::CtrlCount()
{
	CWnd *pWnd;

	UpdateData(TRUE);
	m_CtrlCode[0] = m_CtrlCode[1] = m_CtrlCode[2] = 0;

	if ( m_bCtrlView ) {
		for ( LPCTSTR p = m_EditText ; *p != _T('\0') ; ) {
			if ( p[0] == _T('%') && p[1] == _T('0') && p[2] == _T('9') ) {
				m_CtrlCode[0] += 1;
				p += 3;
			} else if ( p[0] == _T('%') &&
				((p[1] >= _T('0') && p[1] <= _T('9')) || (p[1] >= _T('A') && p[1] <= _T('F')) || (p[1] >= _T('a') && p[1] <= _T('f'))) &&
				((p[2] >= _T('0') && p[2] <= _T('9')) || (p[2] >= _T('A') && p[2] <= _T('F')) || (p[2] >= _T('a') && p[2] <= _T('f'))) ) {
				m_CtrlCode[2] += 1;
				p += 3;
			} else if ( p[0] == _T('\r') && p[1] == _T('\n') ) {
				m_CtrlCode[1] += 1;
				p += 2;
			} else if ( *p == _T('\t') ) {
				m_CtrlCode[0] += 1;
				p++;
			} else if ( *p < _T(' ') ) {
				m_CtrlCode[2] += 1;
				p++;
			} else {
				p++;
			}
		}
	} else {
		for ( LPCTSTR p = m_EditText ; *p != _T('\0') ; p++ ) {
			if ( *p == _T('\t') )
				m_CtrlCode[0] += 1;
			else if ( *p == _T('\r') )
				m_CtrlCode[1] += 1;
			else if ( *p != _T('\n') && *p < _T(' ') )
				m_CtrlCode[2] += 1;
		}
	}

	UpdateData(FALSE);

	if ( (pWnd = GetDlgItem(IDC_CTRLCONV1)) != NULL )
		pWnd->EnableWindow(m_CtrlCode[0] == 0 ? FALSE : TRUE);

	if ( (pWnd = GetDlgItem(IDC_CTRLCONV2)) != NULL )
		pWnd->EnableWindow(m_CtrlCode[1] == 0 ? FALSE : TRUE);

	if ( (pWnd = GetDlgItem(IDC_CTRLCONV3)) != NULL )
		pWnd->EnableWindow(m_CtrlCode[2] == 0 ? FALSE : TRUE);

	if ( (pWnd = GetDlgItem(IDC_CHECK3)) != NULL )
		pWnd->EnableWindow(m_bUpdateEnable);

	if ( (pWnd = GetDlgItem(IDC_ONELINE)) != NULL )
		pWnd->EnableWindow(m_CtrlCode[1] == 0 ? FALSE : TRUE);
}
LPCTSTR CAnyPastDlg::CtrlStr(LPCTSTR str, BOOL bCtrl)
{
	TCHAR ch;
	CString fmt;

	m_ConvStr.Empty();

	if ( bCtrl ) {	// Encode Simple Text -> Ctrl View Text
		while ( *str != _T('\0') ) {
			if ( str[0] == _T('\r') && str[1] == _T('\n') ) {
				m_ConvStr += _T("\r\n");
				str += 2;
			} else if ( *str == _T('%') ) {
				m_ConvStr += _T("%%");
				str++;
			} else if ( *str < _T(' ') ) {
				fmt.Format(_T("%%%02X"), *str);
				m_ConvStr += fmt;
				str++;
			} else {
				m_ConvStr += *(str++);
			}
		}
	} else {		// Decode
		while ( *str != _T('\0') ) {
			if ( str[0] == _T('%') &&
				((str[1] >= _T('0') && str[1] <= _T('9')) || (str[1] >= _T('A') && str[1] <= _T('F')) || (str[1] >= _T('a') && str[1] <= _T('f'))) &&
				((str[2] >= _T('0') && str[2] <= _T('9')) || (str[2] >= _T('A') && str[2] <= _T('F')) || (str[2] >= _T('a') && str[2] <= _T('f'))) ) {
				if ( str[1] >= _T('0') && str[1] <= _T('9') )
					ch = str[1] - _T('0');
				else if ( str[1] >= _T('A') && str[1] <= _T('F') )
					ch = str[1] - _T('A') + 10;
				else
					ch = str[1] - _T('a') + 10;
				ch <<= 4;

				if ( str[2] >= _T('0') && str[2] <= _T('9') )
					ch += (str[2] - _T('0'));
				else if ( str[2] >= _T('A') && str[2] <= _T('F') )
					ch += (str[2] - _T('A') + 10);
				else
					ch += (str[2] - _T('a') + 10);

				m_ConvStr += ch;
				str += 3;
			} else if ( str[0] == _T('%') && str[1] == _T('%') ) {
				m_ConvStr += *str;
				str += 2;
			} else {
				m_ConvStr += *(str++);
			}
		}
	}

	return m_ConvStr;
}
BOOL CAnyPastDlg::OnInitDialog()
{
	CButton *pWnd;

	CDialogExt::OnInitDialog();

	InitItemOffset(ItemTab);

	m_IconBox.SetIcon(LoadIcon(NULL, IDI_QUESTION));
	m_bUpdateEnable = FALSE;
	UpdateData(FALSE);

	if ( m_bCtrlView ) {
		if ( (pWnd = (CButton *)GetDlgItem(IDC_CHECK4)) != NULL )
			pWnd->SetCheck(TRUE);
		m_EditText = CtrlStr(m_EditText, TRUE);
	}

	m_EditWnd.SetWindowText(m_EditText);
	CtrlCount();

	CRect rect;
	int cx, cy;

	GetWindowRect(rect);
	m_MinWidth = rect.Width();
	m_MinHeight = rect.Height();

	cx = AfxGetApp()->GetProfileInt(_T("AnyPastDlg"), _T("cx"), rect.Width());
	cy = AfxGetApp()->GetProfileInt(_T("AnyPastDlg"), _T("cy"), rect.Height());

	if ( cx < rect.Width() )
		cx = rect.Width();
	if ( cy < rect.Height() )
		cy = rect.Height();

	MoveWindow(rect.left, rect.top, cx, cy, FALSE);

	AddShortCutKey(0, VK_RETURN, MASK_CTRL, 0, IDOK);

	return TRUE;
}

void CAnyPastDlg::SaveWindowRect()
{
	if ( !IsIconic() ) {
		CRect rect;
		GetWindowRect(rect);
		AfxGetApp()->WriteProfileInt(_T("AnyPastDlg"), _T("cx"), MulDiv(rect.Width(), m_InitDpi.cx, m_NowDpi.cx));
		AfxGetApp()->WriteProfileInt(_T("AnyPastDlg"), _T("cy"), MulDiv(rect.Height(), m_InitDpi.cy, m_NowDpi.cy));
	}
}
void CAnyPastDlg::OnOK()
{
	UpdateData(TRUE);
	m_EditWnd.GetWindowText(m_EditText);
	if ( m_bCtrlView )
		m_EditText = CtrlStr(m_EditText, FALSE);
	SaveWindowRect();
	CDialogExt::OnOK();
}
void CAnyPastDlg::OnCancel()
{
	UpdateData(TRUE);
	SaveWindowRect();
	CDialogExt::OnCancel();
}

void CAnyPastDlg::OnUpdateEdit()
{
	m_bUpdateEnable = TRUE;
	m_EditWnd.GetWindowText(m_EditText);
	CtrlCount();
}

void CAnyPastDlg::OnSize(UINT nType, int cx, int cy)
{
	SetItemOffset(ItemTab, cx, cy);
	CDialogExt::OnSize(nType, cx, cy);
	Invalidate(FALSE);
}

void CAnyPastDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	int width  = MulDiv(m_MinWidth,  m_NowDpi.cx, m_InitDpi.cx);
	int height = MulDiv(m_MinHeight, m_NowDpi.cy, m_InitDpi.cy);

	if ( (pRect->right - pRect->left) < width ) {
		if ( fwSide == WMSZ_LEFT || fwSide == WMSZ_TOPLEFT || fwSide == WMSZ_BOTTOMLEFT )
			pRect->left = pRect->right - width;
		else
			pRect->right = pRect->left + width;
	}

	if ( (pRect->bottom - pRect->top) < height ) {
		if ( fwSide == WMSZ_TOP || fwSide == WMSZ_TOPLEFT || fwSide == WMSZ_TOPRIGHT )
			pRect->top = pRect->bottom - height;
		else
			pRect->bottom = pRect->top + height;
	}

	CDialogExt::OnSizing(fwSide, pRect);
}

void CAnyPastDlg::OnCtrlConv(UINT nID)
{
	CString tmp, str;

	m_EditWnd.GetWindowText(tmp);

	if ( m_bCtrlView )
		tmp = CtrlStr(tmp, FALSE);

	for ( LPCTSTR p = tmp ; *p != _T('\0') ; p++ ) {
		if ( *p == _T('\t') )
			str += (nID == IDC_CTRLCONV1 ? _T(' ') :  *p);
		else if ( *p == _T('\r') ) {
			if ( nID == IDC_CTRLCONV2 ) {
				if ( p[1] == _T('\n') )
					p++;
				if ( p[1] != _T('\0') )
					str += _T(' ');
			} else
				str += *p;
		} else if ( *p != _T('\n') && *p < _T(' ') )
			str += (nID == IDC_CTRLCONV3 ? _T(' ') :  *p);
		else
			str += *p;
	}

	if ( m_bCtrlView )
		str = CtrlStr(str, TRUE);

	m_bUpdateEnable = TRUE;
	m_EditWnd.SetSel(0, -1, FALSE);
	m_EditWnd.ReplaceSel(str, TRUE);
	m_EditWnd.SetFocus();
}

void CAnyPastDlg::OnShellesc()
{
	int st, ed;
	CString tmp, work, str;
	LPCTSTR p;

	m_EditWnd.GetSel(st, ed);

	if ( st == ed )
		return;

	m_EditWnd.GetWindowText(m_EditText);
	tmp = m_EditText.Mid(st, ed - st);
		
	if ( m_bCtrlView )
		tmp = CtrlStr(tmp, FALSE);

	for ( p = tmp ; *p != _T('\0') ; ) {
		if ( _tcschr(_T(" \"$@&'()^|[]{};*?<>`\\"), *p) != NULL ) {
			str += _T('\\');
			str += *(p++);
		} else if ( *p == _T('\t') ) {
			str += _T("\\t");
			p++;
		} else if ( *p != _T('\r') && *p != _T('\n') && *p < _T(' ') ) {
			work.Format(_T("\\%03o"), *p);
			str += work;
			p++;
		} else {
			str += *(p++);
		}
	}

	if ( m_bCtrlView )
		str = CtrlStr(str, TRUE);

	m_bUpdateEnable = TRUE;
	m_EditWnd.ReplaceSel(str, TRUE);
	m_EditWnd.SetFocus();
}

void CAnyPastDlg::OnOneLine()
{
	int st, ed, mx;
	LPCTSTR p;
	CBuffer tmp;
	CString str;

	UpdateData(TRUE);

	m_EditWnd.GetSel(st, ed);
	m_EditWnd.GetWindowText(m_EditText);
	p = m_EditText;
	mx = (int)_tcslen(p);

	if ( st == mx && ed == mx )
		st = ed = 0;

	if ( st == ed ) {
		// 範囲指定なしなら行頭・行末を検索のみ
		while ( st > 0 ) {
			if ( st >= 2 && p[st - 2] == _T('\r') && p[st - 1] == _T('\n') )
				break;
			st--;
		}
		while ( ed < mx ) {
			if ( ed >= 2 && p[ed - 2] == _T('\r') && p[ed - 1] == _T('\n') )
				break;
			ed++;
		}

	} else {
		// 範囲指定を送信
		if ( st < ed && m_pView != NULL ) {
			str = m_EditText.Mid(st, ed - st);

			if ( m_bCtrlView )
				str = CtrlStr(str, FALSE);

			for ( LPCTSTR s = str ; *s != _T('\0') ; ) {
				if ( s[0] == L'\r' && s[1] == L'\n' ) {
					// 改行は\rだけにする
					tmp.PutWord(*s);
					s += 2;
				} else
					tmp.PutWord(*(s++));
			}

			if ( tmp.GetSize() > 0 )
				m_pView->SendBuffer(tmp, FALSE, m_bDelayPast);
		}

		// 次行を範囲指定
		if ( ed < mx ) {
			st = ed++;
			ed++;
			while ( ed < mx ) {
				if ( ed >= 2 && p[ed - 2] == _T('\r') && p[ed - 1] == _T('\n') )
					break;
				ed++;
			}
		} else
			st = ed = mx;
	}

	m_EditWnd.SetSel(st, ed);
	m_EditWnd.SetFocus();
}

BOOL CAnyPastDlg::PreTranslateMessage(MSG* pMsg)
{
	if ( m_bCtrlView && (pMsg->message == WM_KEYDOWN || pMsg->message == WM_CHAR) && pMsg->hwnd == m_EditWnd.GetSafeHwnd() ) {
		if ( pMsg->message == WM_KEYDOWN ) {
			switch(pMsg->wParam) {
			case VK_CANCEL:
				pMsg->message = WM_CHAR;
				pMsg->wParam  = 0x03;
				break;
			case VK_TAB:
				pMsg->message = WM_CHAR;
				pMsg->wParam  = 0x09;
				break;
			case VK_CLEAR:
				pMsg->message = WM_CHAR;
				pMsg->wParam  = 0x0C;
				break;
			case VK_ESCAPE:
				pMsg->message = WM_CHAR;
				pMsg->wParam  = 0x1B;
				break;
			}
		}
		if ( pMsg->message == WM_CHAR && pMsg->wParam != _T('\x08') && pMsg->wParam != _T('\x0D') && pMsg->wParam < _T(' ') ) {
			CString tmp;
			tmp.Format(_T("%%%02X"), (int)pMsg->wParam);
			::SendMessage(m_EditWnd.GetSafeHwnd(), EM_REPLACESEL, (WPARAM)TRUE, (LPARAM)(LPCTSTR)tmp);
			return TRUE;
		}
	}

	return CDialogExt::PreTranslateMessage(pMsg);
}

void CAnyPastDlg::OnCtrlView()
{
	m_bCtrlView = (m_bCtrlView ? FALSE : TRUE);
	m_EditWnd.GetWindowText(m_EditText);
	m_EditText = CtrlStr(m_EditText, m_bCtrlView);
	m_EditWnd.SetWindowText(m_EditText);
}
